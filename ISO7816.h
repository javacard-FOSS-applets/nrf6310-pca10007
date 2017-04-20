#include <stdint.h>



#define DELAY_ETU_CYCLES 45000
#define one_CLK_cycle 	 6

#define APDU
#define TEST 						 1


void Card_Activate(void);
void Card_Deactivate(void);
void Card_Cold_Reset(void);
void Warm_Reset(void);

void Try_Locating_Card_Manager(void);
void Try_Locating_Instructions(void);
void Try_Locating_Card_Manager_Brute(void);

void Send_Negotiate_Block_Protocol_Alone(void);
void Send_Negotiate_Block_Protocol_APDU(void);
void Send_Negotiate_Block_Protocol_Block(void);
void Send_Test_Block_Frame(uint8_t Length, uint8_t* Payload);

void Card_wait(void);
void Card_wait_ETU_cycles(uint32_t Cycles);
void Copy_Mem(uint8_t Lenght, uint8_t * from, uint8_t * to);

void Test_Error_Database(void);
void Locate_Error(uint8_t SW1, uint8_t SW2);
void test_Card(void);

uint16_t Concat_Bytes(uint8_t byte1, uint8_t byte2);

void		Analyze_Message(uint8_t Lenght, uint8_t * Message);
uint8_t Is_Valid_Message(uint8_t offset, uint8_t Lenght, uint8_t * Message);

uint8_t Prepare_Standard_APDU(uint8_t Lenght, uint8_t * Payload);
uint8_t Prepare_Standard_Block(uint8_t Lenght, uint8_t * Payload);
uint8_t	Prepare_Standard_APDU_Block(uint8_t Lenght, uint8_t * Payload);


void 		SC_Send_Message(uint8_t Lenght);
uint8_t Recieve_Response(void);
uint8_t Recieve_And_Check_Response(void);
uint8_t Send_Message_Recieve_Response(uint8_t * Payload, uint8_t send_count, uint8_t * Message_Recieved);

uint8_t SC_Analyze_ATR_Content(T0 *t0);
uint8_t SC_Get_Next_ATR_Content(T0 *t0);

uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message);
uint8_t Send_Message_Recieve_Response(uint8_t * Message_Send, uint8_t send_count, uint8_t * Message_Recieved);

void SC_Check_Card(void);
void SC_Analyze_ATR(void);
void SC_Recieve_ATR(void);



extern uint8_t ATR_Message[];
extern uint8_t ATR_count;

extern uint8_t SC_Response[];
extern uint8_t SC_Response_Count;
extern uint8_t SC_APDU[];
extern uint8_t SC_Temp_Buffer[];

extern uint8_t SC_Header[];

// Books
// 	https://www.globalplatform.org/specificationscard.asp
//	http://www.win.tue.nl/pinpasjc/docs/GPCardSpec_v2.2.pdf

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

//https://smartcard-atr.appspot.com/parse?ATR=3b9e96801fc78031e073fe211b66d0017b100f0065
	//t-mobile sim card
		//0x3bh 0x9eh 0x96h 0x80h 0x1fh 0xc7h 0x80h 0x31h 0xe0h 0x73h 0xfeh 0x21h 0x1bh 0x66h 0xd0h 0x01h 0x7bh 0x10h 0x0fh 0x00h 0x65h
		//3B 9E 96 80 1F C7 80 31 E0 73 FE 21 1B 66 D0 01 7B 10 0F 00 65

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
	

//http://stackoverflow.com/questions/27600597/cant-select-aid-card-manager-when-testing-to-send-apdu

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

//5
//0 1 2  3  4
//0 0 90 00 90

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

// RFU reserved for future use
// ICC integrated circuit card
// IFD

/*CLA INS P1 P2    L Data LRC
class
	instruction
		instruction parameter 1, 2

http://www.oracle.com/technetwork/java/javacard/javacard1-139251.html
/*Table 1. ISO 7816 CLA Values
CLA Value 	Instruction Class
0x0n, 0x1n 	ISO 7816-4 card instructions, such as for file access and security operations
20 to 0x7F 	Reserved
0x8n or 0x9n 	ISO/IEC 7816-4 format you can use for your application-specific instructions, interpreting 'X' according to the standard
0xAn 	Application- or vendor-specific instructions
B0 to CF 	ISO/IEC 7816-4 format you can use for application-specific instructions
D0 to FE 	Application- or vendor-specific instructions
FF 	Reserved for protocol type selection*/

/*http://techmeonline.com/most-used-smart-card-commands-apdu/

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
E2 	Append Record*/
