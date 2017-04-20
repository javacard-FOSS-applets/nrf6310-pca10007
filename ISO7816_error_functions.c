#include <stdint.h>
#include "Universal.h"

#include "ISO7816.h"
#include "ISO7816_errors.h"


void Test_Error_Database() {
	Segger_write_string_value("Entrys in Specific Error database: ", SPECIFC_ERRORS_COUNT);
	Segger_write_string_value("Entrys in General  Error database: ", GENERAL_ERRORS_COUNT);
}

void Locate_Error(uint8_t SW1, uint8_t SW2) {
	uint16_t SW_Response = Concat_Bytes(SW1, SW2);
	
	for(uint16_t i=0; i<SPECIFC_ERRORS_COUNT; i++) {
		if(SPECIFC_ERRORS[i].sw==SW_Response) {
			Segger_write_string("\t");
			Segger_write_string(SPECIFC_ERRORS[i].message);
			Segger_write_string("\n");
			return;
		}
	}
	
	for(uint16_t i=0; i<GENERAL_ERRORS_COUNT; i++) {
		if(GENERAL_ERRORS[i].sw==SW1) {
			Segger_write_string("\t");
			Segger_write_string(GENERAL_ERRORS[i].message);
			Segger_write_string("\n");
			return;
		}
	}
}
