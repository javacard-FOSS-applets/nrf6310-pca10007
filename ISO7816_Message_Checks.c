#include "stdint.h"

#include "Universal.h"
#include "ISO7816.h"


uint8_t Compare_Arrays(uint8_t Lenght, uint8_t * Array_A, uint8_t * Array_B) {
	for(uint8_t i=0; i<Lenght; i++) {
		if( Array_A[i] != Array_B[i] ) {
			return 0;
		}
	}

	return 1;
}



uint8_t Check_Succesfull_Execution_of_Instruction(void) {
	if(SC_Response_Count<2) {
		return 0;
	}
	
	//int8_t offset=0;
	if(SC_ATR_Get_Protocol_Type()==0x01) {
		//offset=-1;
		if(SC_Response[SC_Response_Count-3] == 0x90 &&
			 SC_Response[SC_Response_Count-2] == 0x00) {
				return 1;
		}
	}
	else {
		//offset=0;
		if(SC_Response[SC_Response_Count-2] == 0x90 &&
			 SC_Response[SC_Response_Count-1] == 0x00) {
				return 1;
		}
	}
		
	/*if(SC_Response[SC_Response_Count-2] == 0x90 &&
  	 SC_Response[SC_Response_Count-1] == 0x00) {
		return 1;
	}*/
	
	return 0;
}

uint8_t Does_Response_Containg_Message(uint8_t Lenght, uint8_t * Message) {
	if(SC_Response_Count<Lenght) {
		return 0;
	}
	
	if(Compare_Arrays(Lenght, SC_Response+SC_Response_Count-Lenght-2, Message)) {
		return 1;
	}
	
	return 0;
}



uint8_t Validate_Valid_PPS_Response() {
	if(SC_Send_Count != SC_Response_Count) {
		return 0; //NOT valid
	}
	
	if(Compare_Arrays(SC_Response_Count, SC_APDU, SC_Response)) {
		return 1; // VALID
	}
	return 0; // NOT valid
}



uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message) {
	uint8_t value=0;
	value = init_value;
	
	for(uint8_t i=offset; i<lenght; i++) {
		value = value ^ message[i];
	}
	
	return value;
}

uint8_t Is_Valid_Message(uint8_t offset, uint8_t Lenght, uint8_t * Message) {
	if(SC_ATR_Get_Protocol_Type()==0) {
		return 1;
	}
	if(SC_ATR_Get_Protocol_Type()==1) {
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
	return 0;
}

void SC_Check_Card(void) {
	if(ATR_Message[0]==ISO_7816_DIRECT_CONV 
			|| ATR_Message[0]==ISO_7816_INVERSE_CONV) {
		Segger_write_string("Card CONNECTED succesfully!\n\n");
	}
	else {
		Segger_write_string("No Smart card connected!\n\n\n\n");
	}
}
