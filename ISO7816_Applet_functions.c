#include "Universal.h"
#include "ISO7816.h"

//Select Master File
void Select_MF() {
	Try_Locating_Card_Manager();
	
	if(Check_Succesfull_Execution_of_Instruction()) {
		Segger_write_string("D:    Succesfully EXECUTED!\n");
	}
	else {
		Segger_write_string("D:    ERROR occured!\n");
	}
}

//Create Instance
void Create_Applet_Instance() {
	SC_APDU[0]=0x80;
	SC_APDU[1]=0xb8;
	
	SC_APDU[2]=0x00;
	SC_APDU[3]=0x00;
	SC_APDU[4]=APLET_AID_LENGHT;
	//SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
	for(uint8_t i=0; i<APLET_AID_LENGHT; i++) {
		SC_APDU[5+i]=SCSC_APPLET_AID_RID_PIX[i];
	}

	if(SC_ATR_Get_Protocol_Type()==0) {
		//SC_Send_Message(5);
		//Recieve_And_Check_Response();
		Send_And_Recieve(15);
	}
	else {
		uint8_t count = Prepare_Standard_Block(15, SC_APDU);
		//SC_Send_Message(count);
		//Recieve_And_Check_Response();	
		Send_And_Recieve(count);
	}
	if(Check_Succesfull_Execution_of_Instruction()) {
		Segger_write_string("D:    Succesfully EXECUTED!\n");
	}
	else {
		Segger_write_string("D:    ERROR occured!\n");
	}
}

//Select applet
void Select_Applet() {
	//Try_Locating_Card_Manager();
	SC_APDU[0]=0x00;
	SC_APDU[1]=0xa4;
	
	SC_APDU[2]=0x04;
	SC_APDU[3]=0x00;
	SC_APDU[4]=APLET_AID_LENGHT;
	//SC_APDU[5]=Calc_XOR_Checksum(0, LRC_OFFSET_APDU, 5, SC_APDU);
	for(uint8_t i=0; i<APLET_AID_LENGHT; i++) {
		SC_APDU[5+i]=SCSC_APPLET_AID_RID_PIX[i];
	}

	if(SC_ATR_Get_Protocol_Type()==0) {
		//SC_Send_Message(5);
		//Recieve_And_Check_Response();
		Send_And_Recieve(5+APLET_AID_LENGHT );
	}
	else {
		uint8_t count = Prepare_Standard_Block(5+APLET_AID_LENGHT , SC_APDU);
		//SC_Send_Message(count);
		//Recieve_And_Check_Response();	
		Send_And_Recieve(count);
	}
	
	if(Check_Succesfull_Execution_of_Instruction()) {
		Segger_write_string("D:    Succesfully EXECUTED!\n");
	}
	else {
		Segger_write_string("D:    ERROR occured!\n");
	}
}

uint8_t temp_array[] = {0xaa, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};


//Test applet
void Test_Applet_Response() {
  	SC_APDU[0]=0xff;
//	uint8_t message[1] = {0xff};
	
	if(SC_ATR_Get_Protocol_Type()==0) {
		uint8_t count= Prepare_Standard_APDU(1, SC_APDU);
		Send_And_Recieve(count);
	}
	else {
		uint8_t count= Prepare_Standard_APDU_Block(1, SC_APDU);
		Send_And_Recieve(count);
	}
		
	if(Check_Succesfull_Execution_of_Instruction()) {
		if( Does_Response_Containg_Message(1, temp_array) ) {
			Segger_write_string("D:    Succesfully EXECUTED!\n");
		}
	}
	else {
		Segger_write_string("D:    ERROR occured!\n");
	}
}


uint8_t message[35];
void Select_Applet_Wrapper() {
	Select_MF();
	//Create_Applet_Instance();
		Card_wait();
	
	Select_Applet();
		Card_wait();
	
	Test_Applet_Response();
		
	HW_AES_Encode(message, 0x55);
	for(int8_t i=16; i>=0; i--) {
		message[i+1]=message[i];
	}
	HW_AES_Decode(message);
}
