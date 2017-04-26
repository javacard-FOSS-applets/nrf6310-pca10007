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
uint8_t success=0;

uint8_t SC_Response[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_Response_Count=0;
uint8_t SC_APDU[SC_MAX_MEM_BUFFER]; // TODO buffer length
uint8_t SC_Temp_Buffer[SC_MAX_MEM_BUFFER]; // TODO buffer length

uint8_t SC_Header[]= {0x80, 0x10, 0x01, 0x02};

extern void SC_Recieve_ATR(void);
extern void SC_Check_Card(void);
extern void SC_Analyze_ATR(void);


void Calc_Default_Baudrate() {
	uint16_t ETU = Calc_Default_Cycles_ETU();
	uint32_t Baudrate = ISO7816_CLK / ETU ;
	Set_Default_Baudrate(Baudrate);
}

void Calc_Communication_Baudrate() {
	uint32_t Baudrate = ISO7816_CLK / ATR_ETU ;
	Set_Comm_Baudrate(Baudrate);
}

void SC_Recieve_ATR_And_Config() {
	SC_Recieve_ATR();
	Is_Valid_Message(1, ATR_count, ATR_Message);
	SC_Check_Card();
	SC_Analyze_ATR();
	
	Calc_Communication_Baudrate();
	reconfigure_UART();
	//and timing
}


void init_Card(void) {
	Test_Error_Database();
		
	//ATR_ETU=372;
	
	init_ISO7816_pins();

	test_Card();
	//Wait_For_Button_Press();

	nrf_delay_ms(500);

	UART_prepare_for_recieve();
	Card_Activate();

	SC_Recieve_ATR_And_Config();

	Card_Deactivate();
}
