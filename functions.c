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

#define PIN 31
#define PORT0 0
#define PORT1 8
#define PORT2 16 //BUTTONS
#define PORT3 24 //LEDS

#define BUTTONS PORT2
#define    LEDS PORT1

uint8_t button_value=0;

void SetLEDS(uint8_t);

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
	
	NRF_GPIO->OUT = (value<<LEDS);
}

void BlinkOnce()
{
	SetLEDS(255);
	nrf_delay_ms(50);
	SetLEDS(0);
}

void BlinkLEDS(uint8_t count)
{
	while(count--)
	{
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

uint8_t dataready=0;

uint8_t ReadButtons()
{
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

void init() {
	SEGGER_RTT_WriteString(0, "Segger RTT Console 0, nrf51422 Debug.\n");
	PrepareLEDS();
	PrepareButtons();
	//PrepareTemp();
}

void write_hex_value(uint8_t value){
	static uint8_t previous_value;
	char str[20];
	int count;

	if( value != previous_value)
	{
		count = sprintf(str, "Value: 0x%02xh\n", value);
		if(count == 0 )
		{
			SEGGER_RTT_WriteString(0, "Couldnt write to string.\n");
		}
		else
		{
			SEGGER_RTT_WriteString(0, str);
		}
		previous_value = value;
	}
}

void write_one_hex_value(uint8_t value){
	char str[20];
	int count;

	count = sprintf(str, "0x%02xh ", value);
	if(count == 0 )
	{
		SEGGER_RTT_WriteString(0, "Couldnt write to string.\n");
	}
	else
	{
		SEGGER_RTT_WriteString(0, str);
	}
}

uint8_t ReadRNG(){
	NRF_RNG->TASKS_START = 1;
	
	NRF_RNG->EVENTS_VALRDY = 0;
	while (NRF_RNG->EVENTS_VALRDY == 0)
	{
		;
	}
	
	return (uint8_t) NRF_RNG->VALUE;
}

void PrepareTemp()
{
	nrf_temp_init();
}

uint32_t ReadTemperature()
{
	uint32_t counter=0;
	uint32_t value=~0;

  static uint32_t previous;
	char *string;
	
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
	
	value = nrf_temp_read();	//NRF_TEMP->TEMP;
	NRF_TEMP->TASKS_STOP=1;
	NRF_TEMP->POWER=0;
		
	if(previous != value)
	{
	 sprintf(string, "TEMP: %d\n", value);
	 SEGGER_RTT_WriteString(0, string);
	 previous = value;
	}
	
	return value;
}
