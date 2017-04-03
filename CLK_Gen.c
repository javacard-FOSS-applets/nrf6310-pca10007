#include "nrf_delay.h"
#include "Universal.h"

uint8_t recieved_value=0;
uint8_t recieved_security=0;

int main(void) {
	init_GPIOTE();
	init_Timer2();
	init_PPI();
	
	unsigned int tick=5000;
    
	while (true) {	
		Timer_Start();
		/*tick=5000;

		while (tick--) {
			nrf_delay_ms(1);
		}
		Timer_Stop();

		tick=5000;
		
		while (tick--) {
			nrf_delay_ms(1);
		}*/
	}
}
