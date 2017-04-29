#include "stdint.h"

#include "Universal.h"
#include "ISO7816.h"

uint16_t CLA_DATA[] = {CLA_ISO7816, CLA_GP_PROP};
uint8_t  CLA_DATA_P2[] = {P2_DATA_ISSUER_NUMBER, P2_DATA_CARD_IMAGE,	P2_DATA_CARD_DATA,	P2_DATA_CONFIRMATION_COUNT,	P2_DATA_KEY_TEMPLATE,	P2_DATA_SEQ_COUNTER};
uint8_t  CLA_STATUS_P1[] = {0x80, 0x40, 0x20, 0x10};
uint8_t  CLA_CPLCD_P2[] = {0x70, 0x7f};

void Get_Response(uint8_t count) {
	SC_APDU[P_CLA]=0x00;
	SC_APDU[P_INS]=INS_GET_RESPONSE;
	SC_APDU[P_P1]=0x00;
	SC_APDU[P_P2]=0x00;
	SC_APDU[P_P2+1]=count;
	
	if(SC_ATR_Get_Protocol_Type()==0) {
			Send_And_Recieve(5);
	}
	else {
			uint8_t count = Prepare_Standard_Block(5, SC_APDU);
			Send_And_Recieve(count);
	}
	//SC_Send_Message(5);
	//Recieve_And_Check_Response();

}

void Try_DATA() {
	for(uint8_t cla=0; cla<2; cla++) {
		for(uint8_t param=0; param<6; param++) {
			SC_APDU[P_CLA]=CLA_DATA[cla];
			SC_APDU[P_INS]=INS_GET_DATA;
			SC_APDU[P_P1]=0x00;
			SC_APDU[P_P2]=CLA_DATA_P2[param];
			SC_APDU[P_P2+1]=0x00;
			
			if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(5);
				Recieve_And_Check_Response();
			}
			else {
				uint8_t count = Prepare_Standard_Block(5, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();
			}
		}
	}
}

//http://www.ttfn.net/techno/smartcards/iso7816_4.html#ss6_5
void Try_RECORDS() {
	for(uint8_t cla=0; cla<2; cla++) {
		SC_APDU[P_CLA]=CLA_DATA[cla];
		SC_APDU[P_INS]=INS_READ_RECORD;
		SC_APDU[P_P1]=0xEF;
		SC_APDU[P_P2]=0x00;
				
		if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(4);
				Recieve_And_Check_Response();
		}
		else {
				uint8_t count = Prepare_Standard_Block(4, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		}
	}
}

void Try_Card_Production_Life_Cycle_Data() {
	for(uint8_t cla=0; cla<2; cla++) {
		for(uint8_t param=0; param<2; param++) {
			SC_APDU[P_CLA]=CLA_DATA[cla];
			SC_APDU[P_INS]=INS_GET_DATA;
			SC_APDU[P_P1]=0x9f;
			SC_APDU[P_P2]=CLA_CPLCD_P2[param];
					
			if(SC_ATR_Get_Protocol_Type()==0) {
					SC_Send_Message(4);
					Recieve_And_Check_Response();
			}
			else {
					uint8_t count = Prepare_Standard_Block(4, SC_APDU);
					SC_Send_Message(count);
					Recieve_And_Check_Response();	
			}
		}
	}
}

void Try_STATUS() {
	for(uint8_t param=0; param<4; param++) {
		SC_APDU[P_CLA]=0x80;
		SC_APDU[P_INS]=INS_GET_STAT;
		SC_APDU[P_P1]=0x00;
		SC_APDU[P_P2]=CLA_STATUS_P1[param];
				
		if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(4);
				Recieve_And_Check_Response();
		}
		else {
				uint8_t count = Prepare_Standard_Block(4, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		}
	}
}

void Try_Get_Status() {
	Segger_write_string("Trying to get som status!\n");
	Try_DATA();
	Try_STATUS();
	Try_RECORDS();
}


void Try_Locating_Classes() {
	Segger_write_string("Locating available classes\n");
	
	for(uint16_t cla=0x00; cla<0xff; cla++) {
		SC_APDU[0]=cla;
		SC_APDU[1]=0x00;
		SC_APDU[2]=0x00;
		SC_APDU[3]=0x00;
		SC_APDU[4]=0x00;
		
		SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
		
		if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(4);
				Recieve_And_Check_Response();
		}
		else {
				uint8_t count = Prepare_Standard_Block(4, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		}
	}
}

void Try_Locating_Instructions() {
	Segger_write_string("Locating available instructions\n");
	
	for(uint16_t ins=0x00; ins<0xff; ins++) {
		SC_APDU[0]=0x00;
		SC_APDU[1]=ins;
		SC_APDU[2]=0x00;
		SC_APDU[3]=0x00;
		SC_APDU[4]=0x00;
		
		//SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
				
		if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(4);
				Recieve_And_Check_Response();
		}
		else
		{
				uint8_t count = Prepare_Standard_Block(4, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();	
		}
	}
}

void Try_Locating_Card_Manager_Brute() {
	Segger_write_string("Bruteforcing card manared RID\n");
	
	for(uint16_t rid=0x00; rid<0xff; rid++) {
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
		
		//SC_APDU[10]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 10, SC_APDU);
		if(SC_ATR_Get_Protocol_Type()==0) {
				SC_Send_Message(10);
				Recieve_And_Check_Response();
		}
		else {
				uint8_t count = Prepare_Standard_Block(10, SC_APDU);
				SC_Send_Message(count);
				Recieve_And_Check_Response();
		}
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
	//SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);

	if(SC_ATR_Get_Protocol_Type()==0) {
		//SC_Send_Message(5);
		//Recieve_And_Check_Response();
		Send_And_Recieve(5);
	}
	else {
		uint8_t count = Prepare_Standard_Block(5, SC_APDU);
		//SC_Send_Message(count);
		//Recieve_And_Check_Response();	
		Send_And_Recieve(count);
	}
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

//	SC_Send_Message(count);
//	Recieve_And_Check_Response();
	Send_And_Recieve(count);
}

void Try_Single_Bytes() {
	for(uint16_t byte=0xa4; byte<0xa6; byte++) {
		SC_APDU[0]=byte;
		SC_Send_Message(1);
		//UART_prepare_for_recieve();
		Recieve_And_Check_Response();
	}
}

void Test(void) {
	//Try_Single_Bytes();
	
	/*SC_APDU[0]=0xc0;
	SC_APDU[1]=0x20;  //0x60;
	SC_APDU[2]=0x00;
	SC_APDU[3]=0x00;
	SC_APDU[4]=0x08;
	SC_APDU[5]=0x00;
	SC_APDU[6]=0x00;
	//SC_APDU[7]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 7, SC_APDU);
	//uint8_t count = Prepare_Standard_Block(7, SC_APDU);
	
	SC_Send_Message(7);
	Recieve_And_Check_Response();	*/
	
	/*SC_APDU[0]=0xc0;
	SC_APDU[1]=0xa4;  //0x60;
	SC_APDU[2]=0x00;
	SC_APDU[3]=0x00;
	SC_APDU[4]=0x02;
	SC_APDU[5]=0x3F;
	SC_APDU[6]=0x00;
	//SC_APDU[7]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 7, SC_APDU);
	SC_Send_Message(7);
	Recieve_And_Check_Response();*/
			
	//return;
	Try_Locating_Card_Manager();
	Try_Locating_Card_Manager_Brute();
	Try_Locating_Classes();
	Try_Locating_Instructions();
	//Wait_For_Button_Press();
}


void test_Card(void) {
	Segger_write_string("\nTesting card!\n");
	
	Card_Activate();
	SC_Recieve_ATR_And_Config();

	Test();
	
	Card_wait();
	Card_Deactivate();
	Card_wait();
	
	return;
/*	Send_Negotiate_Block_Protocol_Block();
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
	}*/
}
