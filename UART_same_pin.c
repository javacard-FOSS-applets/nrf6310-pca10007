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

void Set_Pin_Output() {
	nrf_gpio_cfg_output(PIN_TX_RX);
}

void Set_Pin_Input() {
	nrf_gpio_cfg_input(PIN_TX_RX, NRF_GPIO_PIN_NOPULL);
}

void init_UART() {
	Set_Pin_Output();
	Set_Pin_Input();
	
	NRF_UART0->PSELRXD=PIN_RX;
	NRF_UART0->PSELTXD=PIN_TX;
	
	NRF_UART0->BAUDRATE=UART_BAUDRATE_BAUDRATE_Baud9600;
				
	NRF_UART0->POWER=1;
	NRF_UART0->ENABLE=4;
	
	NRF_UART0->EVENTS_RXDRDY=0;
	
	NRF_UART0->TASKS_STARTTX=1;
	NRF_UART0->TASKS_STARTRX=1;
}


void Send_UART(uint8_t byte) {
	Set_Pin_Output();
	
	NRF_UART0->TXD=byte;
	
	while(NRF_UART0->EVENTS_TXDRDY == 0 ) {
		;
	}
		
	NRF_UART0->EVENTS_TXDRDY=0;
}

uint8_t Recieve_UART(void) {
	Set_Pin_Input();
	
	uint8_t value=0;
	
	while(NRF_UART0->EVENTS_RXDRDY == 0 ) {
		;
	}
	
	if(NRF_UART0->EVENTS_RXDRDY) {
		value = NRF_UART0->RXD;
		NRF_UART0->EVENTS_RXDRDY=0;
	}
	
	return value;
}


