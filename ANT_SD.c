#include "nrf51.h"

#include "nrf51422_peripherals.h"
#include "nrf51_bitfields.h"
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "Universal.h"

void Wait_until_not_transmitting(void) {
	uint8_t Pending_Transmit=1;
	uint32_t err_code;
	
	while(Pending_Transmit) {
		err_code = sd_ant_pending_transmit(NORDIC_CHANNEL, &Pending_Transmit);
		APP_ERROR_CHECK(err_code);
	}
}
