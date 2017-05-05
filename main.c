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

#include <stdio.h>
#include <stdlib.h>

#include "Universal.h"

int main(void) {
	uint32_t temperature;
				//printf("Test\r\n");
	init();

	Bootloader();
	
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
