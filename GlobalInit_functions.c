#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "nrf51.h"

#include "nrf51422_peripherals.h"
#include "nrf51_bitfields.h"
#include "stdio.h"
#include "stdlib.h"
#include "nrf_gpio.h"
#include "SEGGER_RTT.h"
#include "nrf_temp.h"
#include <stdio.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "PSK.h"

#include "Universal.h"
#include "ISO7816.h"

uint8_t button_value=0;

void SetLEDS(uint8_t);
void ENC_DEC_Test(void);

void PrepareLEDS(void);
void PrepareButtons(void);

void init() {
	Segger_write_string("Segger RTT Console, nrf51422 Debug.\n");
	
	Segger_write_string("Preparing LEDS!\n");
		PrepareLEDS();
	Segger_write_string("Preparing Buttons!\n");
		PrepareButtons();
	
	Bootloader();
	
	Segger_write_string("Preparing Message segmenter!\n");
		init_RF_segmenter();
			
	#ifdef COMPILE_HW
		//init_ISO7816_pins();
		Segger_write_string("Preparing Smart Card interface!\n");
			init_Card();
			Select_Applet_Wrapper();
	#endif

	
	//PrepareTemp();
	
	// Debug strings Segger
	Segger_write_string("PSK: ");
	for(int i=0; i<16; i++)
		Segger_write_one_hex_value(PSK[i]);
	Segger_write_string("\n");
	
	Segger_write_string("VECTOR: ");
	for(int i=0; i<16; i++)
		Segger_write_one_hex_value(IVECTOR[i]);
	Segger_write_string("\n");
	
	// Test Encryption/Decryption
	
	#ifdef COMPILE_SW
		ENC_DEC_Test();
		ENC_DEC_Test();
	#endif
			
	Segger_write_string("\n");
}
