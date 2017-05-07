#include "nrf_delay.h"
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

	Global_Default_Security=DEFAULT_SECURITY;
	
	GLobal_Test_Mode_Active=false;
	
	// flush keys
	ReadButtons();
	if(ReadButtons() != 0x00) {
		while(!bootloader_exit) {

			key=ReadButtons();
			switch(key) {
				case 0x01: Global_Default_Security=MSG_UNSECURED;// no security
									break;
				case 0x02: Global_Default_Security=MSG_SW_SYMM;// SW AES
									break;
				case 0x04: Global_Default_Security=MSG_HW_SYMM;// HW AES
									break;
				case 0x10: GLobal_Test_Mode_Active=~GLobal_Test_Mode_Active;// test
									 nrf_delay_ms(150);
									break;
				case 0x40: 	BlinkLEDS(1);
										bootloader_exit=true;
										#ifdef COMPILE_HW
											Debug_Mode();// ISO7816_UART converter
										#endif
										return;
				case 0x80: // accept
									bootloader_exit=true;
									BlinkLEDS(1);
									return;
			}
			LED_State(Global_Default_Security, GLobal_Test_Mode_Active);
		}
	}
}
