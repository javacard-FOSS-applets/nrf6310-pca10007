/*
0xA000 0x37000
0x20000900  0x3700

0x10000 0x30000
0x20000800 0x3800

todo :	set  orr 00000FF0000
				clear andd FFFF00ffffff

*/

//#pragma ARM

#include "nrf51.h"
#include "nrf51422_peripherals.h"
#include "nrf51_bitfields.h"

#include "stdio.h"
#include "stdlib.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "SEGGER_RTT.h"

#include "nrf_temp.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define PIN 31
#define PORT0 0
#define PORT1 8
#define PORT2 16 //BUTTONS
#define PORT3 24 //LEDS

#define BUTTONS PORT2
#define    LEDS PORT1

void SetLEDS(uint8_t);

uint32_t ReadTemperature()
{
	uint32_t counter=0;
	uint32_t value=~0;
	NRF_TEMP->POWER=1;
	NRF_TEMP->TASKS_START=1;
		
	while(NRF_TEMP->EVENTS_DATARDY==0 && counter<35000)
	{
			counter++;
	}
	if(NRF_TEMP->EVENTS_DATARDY==0)
	{
		return ~0x0;
	}
	
	NRF_TEMP->EVENTS_DATARDY=0;
	
	value = NRF_TEMP->TEMP;
	NRF_TEMP->TASKS_STOP=1;
	NRF_TEMP->POWER=0;
	return value;
}

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

void SetLEDS(uint8_t value)
{
	uint32_t zeros;
	uint32_t ones;
	
	zeros = 0;
	ones = ~zeros;
	
	NRF_GPIO->OUT = (ones  & (0x00 << LEDS));
	NRF_GPIO->OUT = (zeros | (value << LEDS));
	
	//NRF_GPIO->OUT = (value<<LEDS);
}

void BlinkLEDS()
{
	uint8_t counter=20;
	
	while(counter--){
		SetLEDS(255);
		nrf_delay_ms(100);
		SetLEDS(0);
		nrf_delay_ms(100);
	}
}

void PrepareButtons()
{
	uint8_t counter=0;
	int value = 0;
	
	for(counter=0; counter<8; counter++)
	{
		value = (PORT2+counter);
		NRF_GPIO->PIN_CNF[value] = 0;
	}
}

uint8_t ReadButtons()
{
	 uint8_t value;
	 static uint8_t previous;
	 char *string;

	 value = ~((( NRF_GPIO->IN )>> BUTTONS) & 0xFF);

	 if(previous != value)
	 {
		 sprintf(string, "0x%02xh\n", value);
		 SEGGER_RTT_WriteString(0, string);
		 previous = value;
	 }
	 return value;
}



void JozefovaImplementacia()
{
	
	for(;;)
	{
		nrf_gpio_cfg_output(PIN);
		nrf_gpio_pin_set(PIN);
		nrf_delay_ms(500);
		nrf_gpio_pin_clear(PIN);
		nrf_delay_ms(500);
	}
}

int main(void)
{
	int value;
	uint32_t temperature;
	//JozefovaImplementacia();
	//printf("Test\r\n");
	SEGGER_RTT_WriteString(0, "Hello World!\n");
	
	PrepareLEDS();
	PrepareButtons();
		
	nrf_temp_init();
	value = nrf_temp_read();
	value=value;
	BlinkLEDS();
	
	while(1==1)
	{
		SetLEDS(ReadButtons() );
		temperature = ReadTemperature();
	}
	
	//return 0;
}
