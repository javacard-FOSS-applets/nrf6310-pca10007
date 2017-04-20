#include <stdint.h>
#include "nrf_delay.h"

#include "Universal.h"
#include "ISO7816.h"

void Copy_Mem(uint8_t Lenght, uint8_t * from, uint8_t * to) {
	for(uint8_t i=0; i<Lenght; i++) {
		to[i]=from[i];
	}
}

uint16_t Concat_Bytes(uint8_t byte1, uint8_t byte2)  {
	uint16_t value=0;
	//value = (uint16_t) (((uint16_t) byte1) << 8  | byte2);
	value = byte1 << 8  |  byte2;
	return value;
}



void Card_wait() {
	nrf_delay_us(DELAY_ETU_CYCLES * one_CLK_cycle);
}

void Card_wait_ETU_cycles(uint32_t Cycles) {
	nrf_delay_us(Cycles * one_CLK_cycle);
}
