#include "Universal.h"

void init_ISO7816_pins(void) {
	Segger_write_string("Preparing ISO7816 timing andd UART timing variables.\n");
	
	Segger_write_string("Preparing UART!\n");
		init_UART();
	
	Segger_write_string("Preparing RESET pin!\n");
		init_RESET();
	Segger_write_string("Preparing CLK pin!\n");
		init_CLK();
	Segger_write_string("Preparing VCC pin!\n\n");
		init_VCC();	
	
	//Segger_write_string("Preparing UART!\n");
	//	init_UART();
	
	/*uint8_t temp=0;
	while(true) {
		if(ButtonsChanged()) {
			Send_UART(temp++);
		}
	}*/
}
