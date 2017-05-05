#include "Universal.h"

void LED_State(uint8_t Security, uint8_t Test) {
	uint8_t value=0;
	
	value|=0x80;
	
	switch(Security) {
		case MSG_UNSECURED:  value|=0x01;
											break;
		case MSG_SW_SYMM: value|=0x02;
											break;
		case MSG_HW_SYMM: value|=0x04;
											break;
	}
	
	if(Test) {
		value|=0x10;
	}
	
	SetLEDS(value);
}
	
void Bootloader(void) {
	uint8_t bootloader_exit=false;
	uint8_t key;

	uint8_t security=DEFAULT_SECURITY;
	uint8_t run_test=false;
	
	// flush keys
	ReadButtons();
	if(ReadButtons() != 0x00) {
		while(!bootloader_exit) {

			key=ReadButtons();
			switch(key) {
				case 0x01: security=MSG_UNSECURED;// no security
									break;
				case 0x02: security=MSG_SW_SYMM;// SW AES
									break;
				case 0x04: security=MSG_HW_SYMM;// HW AES
									break;
				case 0x10: run_test=true;// test
									break;
				case 0x40: Debug_Mode();// ISO7816_UART converter
									break;
				case 0x80: // accept
									bootloader_exit=true;
									break;
			}
			LED_State(security, run_test);
		}
	}
}
