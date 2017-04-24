#include <stdint.h>

#include "Universal.h"
#include "ISO7816.h"

void Analyze_Content(uint8_t ProtocolType, uint8_t Lenght, uint8_t * Message) {
	
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
	
	SC_APDU[Lenght+5] = Calc_XOR_Checksum(0, LRC_OFFSET_APDU, Lenght+4+1, SC_APDU);
	
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
	
	SC_APDU[3+Lenght]=Calc_XOR_Checksum(0, LRC_OFFSET_BLOCK, 4, SC_APDU); 
	
	return (4+Lenght);
}

uint8_t	Prepare_Standard_APDU_Block(uint8_t Lenght, uint8_t * Payload) {
	Segger_write_string("Preparing Message in APDU in Block\n");
	
	uint8_t count = Prepare_Standard_APDU(Lenght, Payload);
	count = Prepare_Standard_Block(count, SC_APDU);
	//SC_Send_Message(count);
	return count;
}


void Analyze_Status(uint8_t Lenght, uint8_t * Message) {
	if(SC_ATR_Get_Protocol_Type()==0) {
		Segger_write_string_value("\tSW1: ", Message[Lenght-2]);
		Segger_write_string_value("\tSW2: ", Message[Lenght-1]);
		Locate_Error(Message[Lenght-2], Message[Lenght-1]);
		Segger_write_string("\n");
	}
	else if (SC_ATR_Get_Protocol_Type()==1) {
		Segger_write_string_value("\tSW1: ", Message[Lenght-3]);
		Segger_write_string_value("\tSW2: ", Message[Lenght-2]);
		Segger_write_string_value("\tLRC: ", Message[Lenght-1]);
			
		Locate_Error(Message[Lenght-3], Message[Lenght-2]);
		
		Segger_write_string("\n");
	}
}

void Analyze_Message(uint8_t Lenght, uint8_t * Message) {
	//Analyze_Content(0, Lenght, Message);
	Analyze_Status(Lenght, Message);	
}
