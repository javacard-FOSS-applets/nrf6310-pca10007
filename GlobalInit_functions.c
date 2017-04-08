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

uint8_t button_value=0;

void SetLEDS(uint8_t);
void ENC_DEC_Test(void);

void PrepareLEDS(void);
void PrepareButtons(void);

void init() {
	SEGGER_RTT_WriteString(0, "Segger RTT Console 0, nrf51422 Debug.\n");
	PrepareLEDS();
	PrepareButtons();
	
	init_RF_segmenter();

	//init_ISO7816_pins();
	init_Card();
	
	//PrepareTemp();
	
	// Debug strings Segger
	SEGGER_RTT_WriteString(0, "PSK: ");
	for(int i=0; i<16; i++)
		write_one_hex_value(PSK[i]);
	SEGGER_RTT_WriteString(0, "\n");
	
	SEGGER_RTT_WriteString(0, "VECTOR: ");
	for(int i=0; i<16; i++)
		write_one_hex_value(IVECTOR[i]);
	SEGGER_RTT_WriteString(0, "\n");
	
	// Test Encryption/Decryption
	ENC_DEC_Test();
	ENC_DEC_Test();
}
