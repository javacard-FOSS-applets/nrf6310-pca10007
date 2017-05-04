#include "nrf_delay.h"
#include "Universal.h"

uint8_t recieved_value=0;
uint8_t recieved_security=0;

int main(void) {
	init_CLK();
	init_RESET();
	
	unsigned int tick=5000;
	Segger_write_string("Testing GPIO CLK");
    
	while (true) {	
				
		/*nrf_delay_ms(100);
		Clear_RESET();
		nrf_delay_ms(100);
		Set_RESET();*/
		
		tick=1000;

		Start_CLK();
		Clear_RESET();
		while (tick--) {
			nrf_delay_ms(1);
		}
		Stop_CLK();
		Set_RESET();
		tick=1000;
		
		while (tick--) {
			nrf_delay_ms(1);
		}
	}
}
