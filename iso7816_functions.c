#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> 
#include "compiler_abstraction.h"
#include "nrf.h"
#include "nrf_delay.h"

#include "Universal.h"

#define DELAY_ETU_CYCLES 40000
#define one_CLK_cycle 	 6

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0095a/ch03s02s07.html

void Card_Deactivate(void);

void Card_wait() {
	nrf_delay_us(DELAY_ETU_CYCLES * one_CLK_cycle);
}

uint8_t InverseByte(uint8_t byte) {
	uint8_t value=0;
	return value;
}

void ATR() {
	// 3F || 3B T=0 Direct one parity byte
	// MAX 4 bytes
}


void Warm_Reset(void) {
	// reset low
	Set_RESET();
		// set recieve
		UART_input();
		// keep VCC && CLK
		// wait
	// reset_high
	Clear_RESET();
	
	// if ok communicate
	// ekse deactivate
	//Card_Deactivate();
}



void Card_Activate(void) {
		// wait
		Card_wait();
	// VCC
	Set_VCC();
	// set recieve
	UART_input();
		// wait
		Card_wait();
	// CLK clk
	Start_CLK();
		// wait
		Card_wait();
	// reset high
	Clear_RESET();
}


void Card_Deactivate(void) {
	// reset low
	Set_RESET();
		// wait
		Card_wait();
	// clk low
	Stop_CLK();
		//wait
		Card_wait();
	// io low
	Clear_UART();
		// wait
		Card_wait();
	// vcc low
	Clear_VCC();
}

void Card_Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}

void init_ISO7816_pins(void) {
	init_RESET();
	init_CLK();
	init_UART();
	init_VCC();
}

void init_Card(void) {
	/*init_RESET();
	//nrf_delay_ms(1);
	Clear_RESET();
	
	init_CLK();
	Start_CLK();*/
	
	init_ISO7816_pins();
	Card_Activate();
	
	while(true) {
		Warm_Reset();
		Card_wait();
	}
	
	//Card_Deactivate();
	//Set_VCC();
	return;
	Card_Cold_Reset();
	ATR();
}
