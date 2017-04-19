#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> 
#include "compiler_abstraction.h"
#include "nrf.h"
#include "nrf_delay.h"

#include "Universal.h"

#include "ISO7816_errors.h"

#define DELAY_ETU_CYCLES 40000
#define one_CLK_cycle 	 6

//todo Longer Messages than bytes

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0095a/ch03s02s07.html
//http://www.ruimtools.com/atr.php
//http://ludovic.rousseau.free.fr/softwares/pcsc-tools/smartcard_list.txt

//https://smartcard-atr.appspot.com/parse?ATR=3bea00008131fe450031c173c840000090007a
	//p5cn145
		//0x3bh 0xeah 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x00h 0x31h 0xc1h 0x73h 0xc8h 0x40h 0x00h 0x00h 0x90h 0x00h 0x7ah
		//3B EA 00 00 81 31 FE 45 00 31 C1 73 C8 40 00 00 90 00 7A

//https://smartcard-atr.appspot.com/parse?ATR=3bf81300008131fe454a434f5076323431b7
	//p5cn080
		//0x3bh 0xf8h 0x13h 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x4ah 0x43h 0x4fh 0x50h 0x76h 0x32h 0x34h 0x31h 0xb7h
		//3B F8 13 00 00 81 31 FR 45 4A 43 4F 50 76 32 34 31 B7

//apdu error list
	//https://www.eftlab.co.uk/index.php/site-map/knowledge-base/118-apdu-response-list
	//www.smartjac.biz/index.php/support/main-menu?view=kb&kbartid=3&tmpl=component&print=1

// CLA INS
	//http://techmeonline.com/most-used-smart-card-commands-apdu/
	//http://www.cardwerk.com/smartcards/smartcard_standard_iso7816-4_5_basic_organizations.aspx

//http://stackoverflow.com/questions/27600597/cant-select-aid-card-manager-when-testing-to-send-apdu
	//Locating Card Manager
	//https://www.eftlab.com.au/index.php/site-map/knowledge-base/212-emv-rid
	
	
	
//Loading APP
	//https://adywicaksono.wordpress.com/2008/01/05/installing-javacard-applet-into-real-smartcard/
	
void Card_Deactivate(void);
void Send_Negotiate_Block_Protocol_Alone(void);
void Send_Negotiate_Block_Protocol_APDU(void);
void Send_Negotiate_Block_Protocol_Block(void);
void Send_Test_Block_Frame(uint8_t Length, uint8_t* Payload);


uint16_t Concat_Bytes(uint8_t byte1, uint8_t byte2);

uint8_t Prepare_Standard_APDU(uint8_t Lenght, uint8_t * Payload);
uint8_t Prepare_Standard_Block(uint8_t Lenght, uint8_t * Payload);
uint8_t	Prepare_Standard_APDU_Block(uint8_t Lenght, uint8_t * Payload);

uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message);
uint8_t Send_Message_Recieve_Response(uint8_t * Message_Send, uint8_t send_count, uint8_t * Message_Recieved);


uint8_t ATR_Message[ISO_7816_MAX_ATR_BYTES+5];
uint8_t ATR_count=0;
uint8_t success=0;

uint8_t SC_Response[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_APDU[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_Temp_Buffer[SC_MAX_MEM_BUFFER]; // TODO buffer length

uint8_t SC_Header[]= {0x80, 0x10, 0x01, 0x02};

/*uint8_t InverseByte(uint8_t byte) {
	uint8_t value = 0;

	value  = (byte & 0x01) << 7;
	value |= (byte & 0x02) << 5;
	value |= (byte & 0x04) << 3;
	value |= (byte & 0x08) << 1;

	value |= (byte & 0x10) >> 1;
	value |= (byte & 0x20) >> 3;
	value |= (byte & 0x40) >> 5;
	value |= (byte & 0x80) >> 7;

	return value;
}*/

/*void ATR() {
	// 3F || 3B T=0 Direct one parity byte
	// MAX 4 bytes
}*/

void Locate_Error(uint8_t SW1, uint8_t SW2) {
	uint16_t SW_Response = Concat_Bytes(SW1, SW2);
	
	for(uint16_t i=0; i<SPECIFC_ERRORS_COUNT; i++) {
		if(SPECIFC_ERRORS[i].sw==SW_Response) {
			Segger_write_string("\t");
			Segger_write_string(SPECIFC_ERRORS[i].message);
			Segger_write_string("\n");
			return;
		}
	}
	
	for(uint16_t i=0; i<SPECIFC_ERRORS_COUNT; i++) {
		if(GENERAL_ERRORS[i].sw==SW1) {
			Segger_write_string("\t");
			Segger_write_string(GENERAL_ERRORS[i].message);
			Segger_write_string("\n");
			return;
		}
	}
}

void Copy_Mem(uint8_t Lenght, uint8_t * from, uint8_t * to) {
	for(uint8_t i=0; i<Lenght; i++) {
		to[i]=from[i];
	}
}

void Analyze_Content(uint8_t ProtocolType, uint8_t Lenght, uint8_t * Message) {
	
}


uint16_t Concat_Bytes(uint8_t byte1, uint8_t byte2)  {
	uint16_t value=0;
	//value = (uint16_t) (((uint16_t) byte1) << 8  | byte2);
	value = byte1 << 8  |  byte2;
	return value;
}

void Analyze_Status(uint8_t Lenght, uint8_t * Message) {
	Segger_write_string_value("\tSW1: ", Message[Lenght-3]);
	Segger_write_string_value("\tSW2: ", Message[Lenght-2]);
	Segger_write_string_value("\tLRC: ", Message[Lenght-1]);
		
	Locate_Error(Message[Lenght-3], Message[Lenght-2]);
	
	Segger_write_string("\n");
}

void Analyze_Message(uint8_t Lenght, uint8_t * Message) {
	//Analyze_Content(0, Lenght, Message);
	Analyze_Status(Lenght, Message);	
}

//5
//0 1 2  3  4
//0 0 90 00 90

uint8_t Is_Valid_Message(uint8_t offset, uint8_t Lenght, uint8_t * Message) {
	if(Lenght<3) {
		Segger_write_string("Wrong Length!\n");
		return 0;
	}
	
	if(Calc_XOR_Checksum(0x00, offset, Lenght, Message) == 0) {
		Segger_write_string("LRC OK!\n");
		return 1;
	}
	else {
		Segger_write_string("BAD LRC!\n");
		return 0;
	}
}

void init_ISO7816_pins(void) {
	Segger_write_string("Preparing UART!\n");
		init_UART();
	
	Segger_write_string("Preparing RESET pin!\n");
		init_RESET();
	Segger_write_string("Preparing CLK pin!\n");
		init_CLK();
	Segger_write_string("Preparing VCC pin!\n");
		init_VCC();	
	
	//Segger_write_string("Preparing UART!\n");
	//	init_UART();
	
	/*uint8_t temp=0;
	while(true) {
		if(ButtonsChanged()) {
			Send_UART(temp++);
		}
	}*/
}

void Card_wait() {
	nrf_delay_us(DELAY_ETU_CYCLES * one_CLK_cycle);
}

void Warm_Reset(void) {
	// reset low
	Set_RESET();
	// set recieve
	//UART_input();
	//UART_Enable();
	// keep VCC && CLK
		// wait
		Card_wait();
	// reset_high
	Clear_RESET();
	
	// if ok communicate
	// ekse deactivate
	//Card_Deactivate();
}

void Card_Activate(void) {
	Segger_write_string("Activating\n");
		// wait
		Card_wait();
	// VCC
	Set_VCC();
	// set recieve
	//UART_input();
	UART_Enable();
		// wait
		Card_wait();
	// CLK clk
	Start_CLK();
		// wait
		Card_wait();
	// reset high
	Clear_RESET();
}

void Card_Deactivate(void) {
	Segger_write_string("Deactivating\n\n");
	// reset low
	Set_RESET();
		// wait
		Card_wait();
	// clk low
	Stop_CLK();
		//wait
		Card_wait();
	// io low
	//Clear_UART();
	UART_Disable();
		// wait
		Card_wait();
	// vcc low
	Clear_VCC();
}


void Card_Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}

void SC_Recieve_ATR() {
	ATR_count=0;
	
	UART_prepare_for_recieve();
	
	for(uint8_t i=0; i<ISO_7816_MAX_ATR_BYTES; i++) {
		//value=InverseByte(value);
		success=0;
		
		ATR_Message[i]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		if(!success) {
			break;
		}
		
		Segger_write_one_hex_value(ATR_Message[i]);
		ATR_count++;
	}
	Segger_write_string("\n");
}


uint8_t SC_Get_Next_ATR_Content(T0 *t0) { 
	uint8_t skip=0;	
	
	if(t0->Bit_MAP_TA) {
		skip++;
	}
	if(t0->Bit_MAP_TB) {
		skip++;
	}
	if(t0->Bit_MAP_TC) {
		skip++;
	}
	if(t0->Bit_MAP_TD) {
		skip++;
	}
	
	return skip+1;
}

uint8_t SC_Analyze_ATR_Content(T0 *t0) { 
	uint8_t skip=0;	
	
	if(t0->Bit_MAP_TA) {
		Segger_write_string("\tTA present\n");
		skip++;
	}
	if(t0->Bit_MAP_TB) {
		Segger_write_string("\tTB present\n");
		skip++;
	}
	if(t0->Bit_MAP_TC) {
		Segger_write_string("\tTC present\n");
		skip++;
	}
	
	if(t0->Bit_MAP_TD) {
		Segger_write_string("\tTD present\n");
		skip++;
	}
	else {
		return 0;
	}
	
	return skip;
}



void SC_Analyze_ATR() {
	if(ATR_count==0) {
		return;
	}
	
	Segger_write_string_value("\tHistorical Bytes:", ((T0*) (ATR_Message + 1))->Historical_Bytes);
	
	Segger_write_string_value("T0: ", ATR_Message[1]);
	
	uint8_t next_td = SC_Analyze_ATR_Content((T0*)  (ATR_Message + 1 ));
	uint8_t now_td = 0;
	
	while(next_td != now_td) {
		now_td = next_td;
		
		Segger_write_string_value("TD: ", ATR_Message[1+next_td]);
		next_td = next_td + SC_Analyze_ATR_Content((T0*)  (ATR_Message + 1 + next_td ));
	}
	
	Segger_write_string("Historical bytes: \n\t");
	
	uint8_t  next_block = SC_Get_Next_ATR_Content((T0*)  (ATR_Message + 1 + next_td));
	
	for(uint8_t i=0; i<((T0*) (ATR_Message + 1))->Historical_Bytes; i++) {
		Segger_write_one_hex_value(ATR_Message[1+next_td+next_block+ i]);
	}
	
	Segger_write_string("\n\n");
	//Segger_write_string_value("TD1: ", ATR_Message[1]);
}

void SC_Send_Message(uint8_t Lenght) {
	Segger_write_string("Sending Message to SC!\n");
	
	for(uint8_t i=0; i<Lenght; i++) {
		Send_UART(SC_APDU[i]);
	}
	Segger_write_string("\n");
}

uint8_t Recieve_Response() {
	//UART_prepare_for_recieve();
	uint8_t Recieve_Count=0;
	Segger_write_string("\tRecieving response!\n\t");
			
	while(true) {
		success=0;
		SC_Response[Recieve_Count]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		
		if(!success) {
			break;
		}
		Segger_write_one_hex_value(SC_Response[Recieve_Count]);
		Recieve_Count++;
	}
	Segger_write_string("\n");
	
	return Recieve_Count;
}

uint8_t Recieve_And_Check_Response() {
	uint8_t value = Recieve_Response();
	
	if(value<3) {
		return 0;
	}
	
	if(Is_Valid_Message(1, value, SC_Response)) {
		Analyze_Message(value, SC_Response);
		return value;
	}
	
	return 0;
}

void test_Card(void ) {
	Segger_write_string("\nTesting card!\n");
	
	Card_Activate();
	SC_Recieve_ATR();

	Is_Valid_Message(1, ATR_count, ATR_Message);
	SC_Analyze_ATR();

	
	Send_Negotiate_Block_Protocol_Block();
	Recieve_And_Check_Response();
	Send_Negotiate_Block_Protocol_Alone();
	Recieve_And_Check_Response();
	Send_Negotiate_Block_Protocol_APDU();
	Recieve_And_Check_Response();

	SC_APDU[0]=0xFF;
	uint8_t count = Prepare_Standard_APDU_Block(1, SC_APDU);
	SC_Send_Message(count);
	
	Recieve_And_Check_Response();
	
	uint8_t message[2];
	message[0]= 0xFF;
	Send_Test_Block_Frame(1, message);
	Recieve_And_Check_Response();
	
	Card_wait();
	
	message[0]=0xFF;
	uint8_t value = Send_Message_Recieve_Response(message, 1, SC_Response);
		if(Is_Valid_Message(1, value, SC_Response)) {
		Analyze_Message(value, SC_Response);
	}
	
	Card_wait();
	Card_Deactivate();
	Card_wait();
}


void SC_Check_Card() {
	if(ATR_Message[0]==ISO_7816_DIRECT_CONV 
			|| ATR_Message[0]==ISO_7816_INVERSE_CONV) {
		Segger_write_string("Card CONNECTED succesfully!\n");
	}
	else {
		Segger_write_string("No Smart card connected!\n\n\n\n");
	}
}

/* //verify CRC
//calculate CRC
// add header
// 00
// 80
// 01
// 02
// length
// message
// CRC*/

uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message) {
	uint8_t value=0;
	value = init_value;
	
	for(uint8_t i=offset; i<lenght; i++) {
		value = value ^ message[i];
	}
	
	return value;
}

void Send_Negotiate_Block_Protocol_Alone() { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	//SC_APDU[2] = 0x00;	//PTS1 
	//SC_APDU[3] = 0x00;  //PTS2
	//SC_APDU[4] = 0x00;	//PTS3
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	

	SC_Send_Message(3);	
}


void Send_Negotiate_Block_Protocol_APDU()  { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	
	uint8_t value = Prepare_Standard_APDU(3, SC_APDU);
	SC_Send_Message(value);
}

void Send_Negotiate_Block_Protocol_Block() { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	
	uint8_t value = Prepare_Standard_Block(3, SC_APDU);
	SC_Send_Message(value);
}

// RFU reserved for future use
// ICC integrated circuit card
// IFD

void Send_Test_Block_Frame(uint8_t Length, uint8_t* Payload) {
	//SC_BlockFrame frame;
	
	/*frame.NAD=0x00;
 	frame.PCB=0x00;
 	frame.LEN=0x01;
 	frame.message[(uint8_t) 0]=0xFF;
	frame.LRC/CRC 
	*/
	
	uint8_t count = Prepare_Standard_Block(Length, Payload);
	SC_Send_Message(count);
	
	Recieve_And_Check_Response();
}

uint8_t Prepare_Standard_APDU(uint8_t Lenght, uint8_t * Payload) {
	Segger_write_string("Preparing APDU\n");
	Copy_Mem(Lenght, Payload, SC_Temp_Buffer);

	for(uint8_t i=0; i<4; i++) {
		SC_APDU[i] = SC_Header[i];
	}
	
	SC_APDU[4] = Lenght;
	
	for(uint8_t i=0; i<Lenght; i++) {
		SC_APDU[i+5] = SC_Temp_Buffer[i];
	}
	
	SC_APDU[Lenght+5] = Calc_XOR_Checksum(0, 1, Lenght+4+1, SC_APDU);
	
	return Lenght+6;
}

uint8_t Prepare_Standard_Block(uint8_t Lenght, uint8_t * Payload) {
	Segger_write_string("Preparing Block\n");
	Copy_Mem(Lenght, Payload, SC_Temp_Buffer);

	SC_APDU[0]=0x00;
	SC_APDU[1]=0x00;
	
	SC_APDU[2]=Lenght;
	
	for(uint8_t i=0; i<Lenght; i++) {
		SC_APDU[3+i]=SC_Temp_Buffer[i];
	}
	
	SC_APDU[3+Lenght]=Calc_XOR_Checksum(0, 0, 4, SC_APDU); 
	
	return (4+Lenght);
}

uint8_t	Prepare_Standard_APDU_Block(uint8_t Lenght, uint8_t * Payload) {
	Segger_write_string("Preparing Message in APDU in Block\n");
	
	uint8_t count = Prepare_Standard_APDU(Lenght, Payload);
	count = Prepare_Standard_Block(count, SC_APDU);
	//SC_Send_Message(count);
	return count;
}

/*
CLA INS P1 P2    L Data LRC
class
	instruction
		instruction parameter 1, 2

INS Value
		Command Description
0E 	Erase Binary
20 	Verify
70 	Manage Channel
82 	External Authenticate
84 	Get Challenge
88 	Internal Authenticate
A4 	Select File
B0 	Read Binary
B2 	Read Record(s)
C0 	Get Response
C2 	Envelope
CA 	Get Data
D0 	Write Binary
D2 	Write Record
D6 	Update Binary
DA 	Put Data
DC 	Update Record
E2 	Append Record
*/

//6985 Conditions of use not satisfied
//8100 timout
//8200 write succesfull
//9200 
//6700 

void Test_Error_Database() {
	Segger_write_string_value("Entrys in Specific Error database: ", SPECIFC_ERRORS_COUNT);
	Segger_write_string_value("Entrys in General  Error database: ", GENERAL_ERRORS_COUNT);
}

void Try_Locating_Card_Manager() {
	Segger_write_string("Locating card manager\n");
	//AID
	//RID A0 00 00 00 03
	//RID A0 00 00 01 51
	SC_APDU[0]=0x00;
	SC_APDU[1]=0xa4;
	
	SC_APDU[2]=0x04;
	SC_APDU[3]=0x00;
	SC_APDU[4]=0x00;
	SC_APDU[5]=Calc_XOR_Checksum(0, 0, 5, SC_APDU);
	SC_Send_Message(6);
	Recieve_And_Check_Response();
	
	uint8_t count = Prepare_Standard_Block(5, SC_APDU);
	SC_Send_Message(count);
	Recieve_And_Check_Response();	
	
	
	//
}

uint8_t Send_Message_Recieve_Response(uint8_t * Payload, uint8_t send_count, uint8_t * Message_Recieved) {
	Segger_write_string("Send-recieve!\n");

	uint8_t APDU_Length = Prepare_Standard_APDU(send_count, Payload);
	
	SC_Send_Message(APDU_Length);
	
	UART_prepare_for_recieve();
	uint8_t recieved_count = Recieve_And_Check_Response();
	
	return recieved_count;
}

void init_Card(void) {
	init_ISO7816_pins();
	
	test_Card();
	Wait_For_Button_Press();
	nrf_delay_ms(500);
	
	UART_prepare_for_recieve();
	Card_Activate();
		
	SC_Recieve_ATR();
	SC_Check_Card();
	SC_Analyze_ATR();
	
	Try_Locating_Card_Manager();
	
	Card_Deactivate();
	Test_Error_Database();
}

