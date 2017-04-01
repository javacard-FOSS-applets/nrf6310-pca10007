#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "Universal.h"

//if SDK210 v2.0 is enabled  PPI chanels available 0-7
//													 PPI groups  available 0-1
//nedd to use Timer1 || Timer2. TIMER0 reserved for SoftDevice

void init_gpiote(void) {
    nrf_gpio_cfg_output(CLK);
    nrf_gpiote_task_config(GPIOTE_CHANNEL, CLK, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
}

void init_ppi(void) {
	NRF_PPI->CH[PPI_CHANNEL].EEP = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[0];					//at what event
	NRF_PPI->CH[PPI_CHANNEL].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[GPIOTE_CHANNEL];	//what task to activate

	NRF_PPI->CHEN = (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos);
}

void init_timer2(void) {

}

void Timer_Start() {
	NRF_TIMER2->TASKS_START = 1;    // Start event generation.
}

void Timer_Stop() {
	NRF_TIMER2->TASKS_STOP = 1;    // Stop event generation.
}
