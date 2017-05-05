#include "nrf_delay.h"

#include "Universal.h"

// simply send what you recieved, basic test
/*int main(void) {

	uint8_t value=0;
	
	init_UART();
	
	while(true) {
	
		SEGGER_RTT_WriteString(0, "Recieving!\n");
		value = Recieve_UART();
		write_one_hex_value(value);
		SEGGER_RTT_WriteString(0, "Sending!\n");
		Send_UART(value);
	}
}
*/

/*
int main(void) {

	uint8_t value=0;
	
	init_UART();
	
	while(true) {
		SEGGER_RTT_WriteString(0, "Recieving!\n");
		value = Recieve_UART();
		write_one_hex_value(value);
	}
}*/

int main(void) {

	uint8_t value=0;
	init_UART();
	
	while(true) {
		SEGGER_RTT_WriteString(0, "Sending!\n");
		Send_UART(value);
		value++;
		nrf_delay_ms(1000);
	}
}
