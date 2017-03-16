/*
0xA000 0x37000
0x20000900  0x3700

0x10000 0x30000
0x20000800 0x3800
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

void init(void);
void BlinkLEDS(uint8_t);
void SetLEDS(uint8_t);
uint8_t ReadButtons(void);
void write_hex_value(uint8_t value);

int main(void)
{
	uint32_t temperature;
				//printf("Test\r\n");
	init();

	BlinkLEDS(10);
	while(1==1)
	{
		SetLEDS( ReadButtons() );
		
		//SetLEDS ( iterator++ );
		//nrf_delay_ms(1000);
		
		//temperature = ReadTemperature();
	}
	//return 0;
}
