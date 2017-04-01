#include "nrf_delay.h"
#include "Universal.h"

void PrepareLEDS() {
//__asm("ADD r1, r0, #1\n"
//      "MOV r0, r1\n");
		
	NRF_GPIO->DIR=(uint32_t) NRF_GPIO->DIR | ((uint32_t)0xFF<<LEDS);
	//NRF_GPIO->PIN_CNF[31]=0x01;
	
	SetLEDS(0xFF);
	nrf_delay_ms(500);
	SetLEDS(0x00);
	nrf_delay_ms(500);
	
	/*NRF_GPIO->OUT=((uint32_t)0xFF << LEDS);
	NRF_GPIO->OUT=0x00000000;*/
}

void SetLEDS(uint8_t value) {
	uint32_t zeros;
	uint32_t ones;
	
	zeros = 0;
	ones = ~zeros;
	
	NRF_GPIO->OUT = (ones  & (0x00 << LEDS));
	NRF_GPIO->OUT = (zeros | (value << LEDS));
	
	NRF_GPIO->OUT = (value<<LEDS);
}

void BlinkOnce() {
	SetLEDS(255);
	nrf_delay_ms(50);
	SetLEDS(0);
}



void BlinkLEDS(uint8_t count) {
	while(count--) {
		SetLEDS(255);
		nrf_delay_ms(100);
		SetLEDS(0);
		nrf_delay_ms(100);
	}
}


void PrepareButtons() {
	uint8_t counter=0;
	int value = 0;
	
	for(counter=0; counter<8; counter++)
	{
		value = (PORT2+counter);
		NRF_GPIO->PIN_CNF[value] = 0;
	}
}

uint8_t dataready=0;

uint8_t ReadButtons() {
	uint8_t value;
	static uint8_t previous;
	char string[80];

	value = ~((( NRF_GPIO->IN )>> BUTTONS) & 0xFF);

	if(previous != value)
	{
	 sprintf(string, "BUTTONS read: 0x%02xh\n", value);
	 SEGGER_RTT_WriteString(0, string);
	 previous = value;
	 dataready=1;
	}
	return value;
}
