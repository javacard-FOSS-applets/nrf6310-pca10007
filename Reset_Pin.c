#include "nrf51.h"
#include "nrf_gpio.h"
#include "Universal.h"

void Set_RESET() {
	nrf_gpio_pin_set(RESET_PIN);
}

void Clear_RESET() {
	nrf_gpio_pin_clear(RESET_PIN);
}

void init_RESET() {
	nrf_gpio_cfg_output(RESET_PIN);
	Set_RESET();
}
