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
	nrf_delay_us(ISO_WAIT_TIME);
}

void Card_wait_ETU_cycles(uint32_t Cycles) {
	nrf_delay_us(Cycles * one_CLK_cycle);
}



void Calc_Default_Baudrate(void) {
	uint16_t ETU = Calc_Default_Cycles_ETU();
	uint32_t Baudrate = ISO7816_CLK / ETU ;
	Set_Default_Baudrate(Baudrate);
}

void Calc_Communication_Baudrate(void) {
	uint32_t Baudrate = ISO7816_CLK / ATR_ETU ;
	Set_Comm_Baudrate(Baudrate);
}
