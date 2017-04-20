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


void SC_Analyze_ATR(void) {
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

void SC_Check_Card(void) {
	if(ATR_Message[0]==ISO_7816_DIRECT_CONV 
			|| ATR_Message[0]==ISO_7816_INVERSE_CONV) {
		Segger_write_string("Card CONNECTED succesfully!\n");
	}
	else {
		Segger_write_string("No Smart card connected!\n\n\n\n");
	}
}
