#include "nrf51.h"
#include "nrf_gpio.h"
#include "Universal.h"

void Set_VCC() {
	nrf_gpio_pin_set(PIN_VCC);
}

void Clear_VCC() {
	nrf_gpio_pin_clear(PIN_VCC);
}

void init_VCC() {
	nrf_gpio_cfg_output(PIN_VCC);
	Clear_VCC();
}
