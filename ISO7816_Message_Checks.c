#include "stdint.h"

#include "Universal.h"
#include "ISO7816.h"

uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message) {
	uint8_t value=0;
	value = init_value;
	
	for(uint8_t i=offset; i<lenght; i++) {
		value = value ^ message[i];
	}
	
	return value;
}

uint8_t Is_Valid_Message(uint8_t offset, uint8_t Lenght, uint8_t * Message) {
	if(Lenght==2) {
		Segger_write_string("RESPONSE ONLY !?\n");
		return 1;
	}
	
	if(Lenght<2) {
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

void SC_Check_Card(void) {
	if(ATR_Message[0]==ISO_7816_DIRECT_CONV 
			|| ATR_Message[0]==ISO_7816_INVERSE_CONV) {
		Segger_write_string("Card CONNECTED succesfully!\n");
	}
	else {
		Segger_write_string("No Smart card connected!\n\n\n\n");
	}
}
