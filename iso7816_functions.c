#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"

#include "Universal.h"

#define DELAY_ETU_CYCLES 45000
#define one_CLK_cycle 	 0.6

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0095a/ch03s02s07.html

void wait() {
	nrf_delay_ms(DELAY_ETU_CYCLES * one_CLK_cycle);
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
		// set recieve
		// keep VCC && CLK
		// wait
	// reset_high
	
	// if ok communicate
	// ekse deactivate
}



void Card_Activate(void) {
		// wait
	// VCC
	// set recieve
		// wait
	// CLK clk
		// wait
	// reset high
}


void Card_Deactivate(void) {
	// reset low
		// wait
	// clk low
		//wait
	// io low
		// wait
	// vcc low
}

void Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}

void init_Card(void) {
	Card_Activate();
	Cold_Reset();
	ATR();
}
