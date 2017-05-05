#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> 
#include "compiler_abstraction.h"
#include "nrf.h"
#include "nrf_delay.h"

#include "Universal.h"
#include "ISO7816.h"

uint8_t ATR_Message[ISO_7816_MAX_ATR_BYTES+5];
uint8_t ATR_count=0;

uint16_t ATR_ETU=0;

uint8_t SC_Response[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_Response_Count=0;

uint8_t SC_APDU[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_Send_Count=0;
uint8_t SC_Temp_Buffer[SC_MAX_MEM_BUFFER]; // TODO buffer length

uint8_t SC_Header[]= {0x80, 0x10, 0x01, 0x02};

void init_Card(void) {
	Test_Error_Database();
	init_ISO7816_pins();
	test_Card();
	
	Card_Deactivate();
	//Wait_For_Button_Press();

	nrf_delay_ms(500);

	UART_prepare_for_recieve();
	Card_Activate();
	SC_Recieve_ATR_And_Config();
}
