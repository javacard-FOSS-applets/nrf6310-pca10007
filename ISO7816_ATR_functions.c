#include <stdint.h>
#include "Universal.h"
#include "ISO7816.h"


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

void SC_Recieve_ATR(void) {
	ATR_count=0;
	
	UART_prepare_for_recieve();
	
	for(uint8_t i=0; i<ISO_7816_MAX_ATR_BYTES; i++) {
		//value=InverseByte(value);
		uint8_t success=0;
		
		ATR_Message[i]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		if(!success) {
			break;
		}
		
		Segger_write_one_hex_value(ATR_Message[i]);
		ATR_count++;
	}
	Segger_write_string("\n");
}
