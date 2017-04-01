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

uint8_t ReadRNG() {
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

uint32_t ReadTemperature() {
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

