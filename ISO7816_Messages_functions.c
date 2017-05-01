#include <stdint.h>

#include "Universal.h"
#include "ISO7816.h"

/*void Analyze_Content(uint8_t ProtocolType, uint8_t Lenght, uint8_t * Message) {
	
}*/

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
	
	return Lenght+5;
}

static uint8_t APDU_EXTRA_LRC=0x00;
void Toggle_APDU_EXTRA_LRC(void) {
	if(APDU_EXTRA_LRC) {
		APDU_EXTRA_LRC=0;
	}
	else {
		APDU_EXTRA_LRC=1;
	}
	Segger_write_string_value("Extra LRC in APDU in Block changed to:", APDU_EXTRA_LRC);
}
uint8_t Get_APDU_EXTRA_LRC(void) {
	return APDU_EXTRA_LRC;
}

uint8_t Toggle_PCB_Send_Bit() {
	static uint8_t byte=0x40;
	
	if(byte & 0x40) {
		byte=byte & ~0x40;
	}
	else {
		byte= byte | 0x40;
	}
	
	return byte;
}

uint8_t Prepare_Standard_Block(uint8_t Lenght, uint8_t * Payload) {
	Segger_write_string("Preparing Block\n");
	Copy_Mem(Lenght, Payload, SC_Temp_Buffer);

	SC_APDU[0]=0x00;
	SC_APDU[1]=Toggle_PCB_Send_Bit();
	
	SC_APDU[2]=Lenght;
	if(Get_APDU_EXTRA_LRC() && false) {
		for(uint8_t i=0; i<Lenght; i++) {
			SC_APDU[3+i]=SC_Temp_Buffer[i];
		}
		SC_APDU[3+Lenght]=Calc_XOR_Checksum(0, 3, 3+Lenght, SC_APDU); 
		SC_APDU[4+Lenght]=Calc_XOR_Checksum(0, LRC_OFFSET_BLOCK, 4+Lenght, SC_APDU); 

		return (5+Lenght);
	}
	else {
		for(uint8_t i=0; i<Lenght; i++) {
			SC_APDU[3+i]=SC_Temp_Buffer[i];
		}
		SC_APDU[3+Lenght]=Calc_XOR_Checksum(0, LRC_OFFSET_BLOCK, 3+Lenght, SC_APDU); 

		return (4+Lenght);
	}
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
