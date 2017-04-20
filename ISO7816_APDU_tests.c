#include "stdint.h"

#include "Universal.h"
#include "ISO7816.h"

void Try_Locating_Classes() {
	Segger_write_string("Locating available classes\n");
	
	for(uint16_t cla=0xa4; cla<0xa5; cla++) {
		SC_APDU[0]=cla;
		SC_APDU[1]=0x00;
		SC_APDU[2]=0x00;
		SC_APDU[3]=0x00;
		SC_APDU[4]=0x00;
		
		SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
		
		#ifdef APDU
				SC_Send_Message(5+TEST);
				Recieve_And_Check_Response();
		#else
				uint8_t count = Prepare_Standard_Block(5+TEST, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		#endif
	}
}


void Try_Locating_Instructions() {
	Segger_write_string("Locating available instructions\n");
	
	for(uint16_t ins=0xf2; ins<0xf3; ins++) {
		SC_APDU[0]=0x00;
		SC_APDU[1]=ins;
		SC_APDU[2]=0x00;
		SC_APDU[3]=0x00;
		SC_APDU[4]=0x00;
		
		SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
				
		#ifdef APDU
				SC_Send_Message(5+TEST);
				Recieve_And_Check_Response();
		#else
				uint8_t count = Prepare_Standard_Block(5+TEST, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		#endif
	}
}

void Try_Locating_Card_Manager_Brute() {
	Segger_write_string("Bruteforcing card manared RID\n");
	
	for(uint16_t rid=0x03; rid<0x04; rid++) {
		SC_APDU[0]=0x00;
		SC_APDU[1]=0xa4;
		SC_APDU[2]=0x40;
		SC_APDU[3]=0x00;
		SC_APDU[4]=0x05;
		
		SC_APDU[5]=0xa0;
		SC_APDU[6]=0x00;
		SC_APDU[7]=0x00;
		SC_APDU[8]=0x00;
		SC_APDU[9]=rid;
		
		SC_APDU[10]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 10, SC_APDU);
		#ifdef APDU
				SC_Send_Message(10+TEST);
				Recieve_And_Check_Response();
		#else
				uint8_t count = Prepare_Standard_Block(10+TEST, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();
		#endif
	}
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
	SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);

	#ifdef APDU
		SC_Send_Message(5+TEST);
		Recieve_And_Check_Response();
	#else
		uint8_t count = Prepare_Standard_Block(5+TEST, SC_APDU);
		SC_Send_Message(count);
		Recieve_And_Check_Response();	
	#endif
}

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

void Try_Single_Bytes() {
	for(uint16_t byte=0; byte<2; byte++) {
		SC_APDU[0]=byte;
		SC_Send_Message(1);
		UART_prepare_for_recieve();
		Recieve_Response();
	}
}


void Test(void) {
	
	Try_Single_Bytes();
	//Wait_For_Button_Press();
	
	SC_APDU[0]=0xa4;
	SC_Send_Message(1);
	Recieve_And_Check_Response();
	
	SC_APDU[0]=0xc0;
	SC_APDU[1]=0x20;  //0x60;
	SC_APDU[2]=0x00;
	SC_APDU[3]=0x00;
	SC_APDU[4]=0x08;
	SC_APDU[5]=0x00;
	SC_APDU[6]=0x00;
	SC_APDU[7]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 7, SC_APDU);
	
	uint8_t count = Prepare_Standard_Block(8, SC_APDU);
	SC_Send_Message(count);
	Recieve_And_Check_Response();	
	
	SC_APDU[0]=0xc0;
	SC_APDU[1]=0xa4;  //0x60;
	SC_APDU[2]=0x00;
	SC_APDU[3]=0x00;
	SC_APDU[4]=0x02;
	SC_APDU[5]=0x3F;
	SC_APDU[6]=0x00;
	SC_APDU[7]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 7, SC_APDU);
	
	SC_Send_Message(8);
	Recieve_And_Check_Response();
	
	//Wait_For_Button_Press();
		
	//return;
	Try_Locating_Card_Manager();
	Try_Locating_Instructions();
	Try_Locating_Card_Manager_Brute();
	Wait_For_Button_Press();
}


void test_Card(void ) {
	Segger_write_string("\nTesting card!\n");
	
	Card_Activate();
	SC_Recieve_ATR();

	Is_Valid_Message(1, ATR_count, ATR_Message);
	SC_Analyze_ATR();

	Test();
	
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
