#include "nrf_delay.h"
#include "Universal.h"

uint8_t recieved_value=0;
uint8_t recieved_security=0;

int main(void) {
	init_gpiote();                  // Configure a GPIO to toggle on a GPIOTE task.
	init_timer2();                  // Use TIMER0 to generate events every 200 ms.
	init_ppi();                     // Use a PPI channel to connect the event to the task automatically.
	
	unsigned int tick=5000;
    
	while (true) {	
		Timer_Start();
		tick=5000;

		while (tick--) {
			nrf_delay_ms(1);
		}
		Timer_Stop();

		tick=5000;
		
		while (tick--) {
			nrf_delay_ms(1);
		}
	}
}
