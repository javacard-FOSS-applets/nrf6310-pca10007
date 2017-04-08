#include "nrf51.h"
#include "nrf_gpio.h"
#include "Universal.h"

void Set_RESET() {
	nrf_gpio_pin_clear(PIN_RESET);
}

void Clear_RESET() {
	nrf_gpio_pin_set(PIN_RESET);
}

void init_RESET() {
	nrf_gpio_cfg_output(PIN_RESET);
	Clear_RESET();
}
