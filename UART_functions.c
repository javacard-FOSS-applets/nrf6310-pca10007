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



/* https://devzone.nordicsemi.com/question/1181/uart-baudrate-register-values/?answer=1194#post-id-1194

The formula is: Baudrate = desired baudrate * 2^32 / 16000000
Example: Baudrate of 31250 should then be 8388608 decimal = 0x800000.
Note that you will have to round the number afterwards: rounded_value = (value + 0x800) & 0xFFFFF000
Since the baudrate generator will be sourced by the 16 M (Either RC or XOSC, depending on your configuration), then your error rate will be ~equal to the overall drift of the system clock and the accuracy of the baudrate generator towards your target baudrate.
Other way around: Check out this define in nrf51_bitfields.h line 5702:
UART_BAUDRATE_BAUDRATE_Baud115200 (0x01D7E000UL) !< 115200 baud. 
The actual baudrate set with the above define is:
baudrate_reg_val * 16M / 2^32 = 115203.86 baud.*/

uint32_t Calc_Baudrate_Setting(uint32_t baudrate) { return ((baudrate * 0x100000000ULL) / SYSCLK + 0x800) & 0xfffff000; } 

//2777777.778

uint32_t baud_rate_comm=		UART_BAUDRATE_BAUDRATE_Baud7168;
uint32_t baud_rate_default=	UART_BAUDRATE_BAUDRATE_Baud7168;
uint8_t  parity = 0x0e;

void Toggle_Parity(void) {
	if(parity) {
		parity=0x00;
	}
	else {
		parity=0x0e;
	}
	Segger_write_string_value("Parity changed to:", parity);
}

void Set_Comm_Settings(uint32_t new_comm_sett) {
	baud_rate_comm=new_comm_sett;
	Segger_write_string("Setting new COMM baud rate settings: ");
	Segger_write_one_hex_value_32(baud_rate_comm);
	Segger_write_string("\n");
}

void Set_Comm_Baudrate(uint32_t new_baud) {
	baud_rate_comm=Calc_Baudrate_Setting(new_baud);
	Segger_write_string_int("Calculating baud setting for baudrate:", new_baud);
	Segger_write_string("Setting new COMM baud rate settings: ");
	Segger_write_one_hex_value_32(baud_rate_comm);
	Segger_write_string("\n\n");
}

uint32_t Get_Comm_Baudrate(void) {
	return baud_rate_comm;
}


void Set_Default_Baudrate(uint32_t new_baud) {
	baud_rate_default=Calc_Baudrate_Setting(new_baud);
	Segger_write_string_int("Calculating baud setting for baudrate:", new_baud);
	Segger_write_string("Setting new DEFAULT baud rate settings: ");
	Segger_write_one_hex_value_32(baud_rate_default);
	Segger_write_string("\n\n");
}

uint32_t Get_Default_Baudrate(void) {
	return baud_rate_default;
}


void Start_TX(void) {
	NRF_UART0->TASKS_STARTTX=1;
}

void Stop_TX(void) {
	NRF_UART0->TASKS_STOPTX=1;
}

void Start_RX(void) {
	NRF_UART0->TASKS_STARTRX=1;
}

void Stop_RX(void) {
	NRF_UART0->TASKS_STOPRX=1;
}

void NRF_Clear_UART_Errors() {
	NRF_UART0->ERRORSRC=1;
	NRF_UART0->EVENTS_ERROR=0;
}

void NRF_Check_UART_Error() {
		if(NRF_UART0->EVENTS_ERROR) {
		Segger_write_string_value("UART ERROR: ", NRF_UART0->ERRORSRC);
			if(NRF_UART0->ERRORSRC & 1<<2) {
				//3 overrun
				Segger_write_string("\t Overrun error!\n");
			}
			if(NRF_UART0->ERRORSRC & 1<<1) {
				//2 parity
			Segger_write_string("\t Parity error!\n");
			}
			if(NRF_UART0->ERRORSRC & 1<<0) {
				//1 framing
			Segger_write_string("\t Framing error!\n");
			}
			
		//0 no error
		NRF_Clear_UART_Errors();
		Segger_write_string("\n");
	}
}


void UART_prepare_for_recieve() {
	NRF_UART0->EVENTS_RXDRDY=0;
	NRF_Clear_UART_Errors();
}

void UART_input() {
	nrf_gpio_cfg_input(PIN_RX, NRF_GPIO_PIN_NOPULL);
}

void UART_output() {
	nrf_gpio_cfg_output(PIN_TX);
}

void Clear_UART() {
	nrf_gpio_pin_clear(PIN_TX);
}

void init_PINS_UART() {
	nrf_gpio_cfg_input(PIN_RX, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_output(PIN_TX);
	nrf_gpio_pin_set(PIN_TX);
}

//void init_UART_interupt() {
//}

void UART_Warm_Disable() {
	NRF_UART0->TASKS_STOPTX=1;
	NRF_UART0->TASKS_STOPRX=1;
	
	NRF_UART0->ENABLE=0;
}

void UART_Disable() {
	NRF_UART0->TASKS_STOPTX=1;
	NRF_UART0->TASKS_STOPRX=1;
	
	NRF_UART0->ENABLE=0;
	
	NRF_UART0->PSELRXD=0xFF;
	NRF_UART0->PSELTXD=0xFF;
	
	Clear_UART();
}

void UART_Enable() {
	init_UART();
}

void UART_Strategy(uint8_t strat) {
		init_PINS_UART();
	
	NRF_UART0->PSELRXD=PIN_RX;
	NRF_UART0->PSELTXD=PIN_TX;
	
	if(strat==0) {
		NRF_UART0->BAUDRATE=Get_Default_Baudrate();
	}
	else {
		NRF_UART0->BAUDRATE=Get_Comm_Baudrate();
	}
				
		Segger_write_string("Actual baudrate settings=");
		Segger_write_one_hex_value_32(NRF_UART0->BAUDRATE);
		Segger_write_string("\n");
	
	NRF_UART0->POWER=1;	
	NRF_UART0->ENABLE=4;
		
	Stop_TX();//NRF_UART0->TASKS_STOPTX=1;
	Stop_RX();//NRF_UART0->TASKS_STOPRX=1;
	
	NRF_UART0->EVENTS_RXDRDY=0;
	//NRF_UART0->EVENTS_TXDRDY=0;
			
		//NRF_UART0->TASKS_STARTTX=1;
		//NRF_UART0->TASKS_STARTRX=1;
	
	NRF_UART0->CONFIG= parity; // Parity enabled
	
	NRF_UART0->ERRORSRC=1;
	NRF_UART0->EVENTS_ERROR=0;
}

void reconfigure_UART() {
	UART_Warm_Disable();
	UART_Strategy(0x01);
}

void init_UART() {
	UART_Strategy(0x00);
}

void Send_UART(uint8_t byte) {
	//!!!! Segger_write_one_hex_value(byte);
	
	NRF_Clear_UART_Errors();
	
	Start_TX();//NRF_UART0->TASKS_STARTTX=1;
	NRF_UART0->TXD=byte;
		
	while(NRF_UART0->EVENTS_TXDRDY != 1 ) {
		;
	}
	
	NRF_UART0->EVENTS_TXDRDY=0;
	Stop_TX();//NRF_UART0->TASKS_STOPTX=1;
	
	//NRF_Check_UART_Error();
}


uint8_t Recieve_UART_timeout(uint32_t delay, uint8_t * success) {
	uint32_t timeout=0;
	uint8_t value=0;
	*success=0;
	timeout=0;
	
	NRF_Clear_UART_Errors();
	//NRF_UART0->EVENTS_RXDRDY=0;
	//UART_prepare_for_recieve();
	
	Start_RX();
			
	while(NRF_UART0->EVENTS_RXDRDY != 1 && timeout<delay) {
		timeout++;
		nrf_delay_us(0x01);
	}
	
	if(NRF_UART0->EVENTS_RXDRDY!=1) {
		*success=0;
		return 0;
	}
	
	if(NRF_UART0->EVENTS_RXDRDY) {
		value = NRF_UART0->RXD;
		NRF_UART0->EVENTS_RXDRDY=0;
	}
	
	NRF_Check_UART_Error();
	
	Stop_RX();
	
	*success=1;
	return value;
}

uint8_t Recieve_UART(void) {
	uint8_t value=0;
	
	NRF_Clear_UART_Errors();
	//UART_prepare_for_recieve();	
	
	Start_RX(); //NRF_UART0->TASKS_STARTRX=1;
		
	while(NRF_UART0->EVENTS_RXDRDY != 1 ) {
		;
	}
	
	if(NRF_UART0->EVENTS_RXDRDY) {
		value = NRF_UART0->RXD;
		NRF_UART0->EVENTS_RXDRDY=0;
	}
	
	NRF_Check_UART_Error();
	
	Stop_RX(); //NRF_UART0->TASKS_STOPRX=1;
	return value;
}
