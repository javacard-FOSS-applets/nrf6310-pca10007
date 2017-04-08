#include "nrf_delay.h"
#include "Universal.h"

uint8_t recieved_value=0;
uint8_t recieved_security=0;

int main(void) {
	init_CLK();
	init_RESET();
	
//	unsigned int tick=5000;
    
	while (true) {	
		Start_CLK();
		
		nrf_delay_ms(100);
		Clear_RESET();
		nrf_delay_ms(100);
		Set_RESET();
		
		/*tick=5000;

		while (tick--) {
			
		}
		Stop_CLK();

		tick=5000;
		
		while (tick--) {
			nrf_delay_ms(1);
		}*/
	}
}
