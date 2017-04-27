#include "Universal.h"
#include "ISO7816.h"

uint8_t Card_running=0;

void Set_Activated() {
	Card_running=1;
}

void Set_Deactivated() {
	Card_running=1;
}

uint8_t Is_Card_Active() {
	return Card_running;
}

void Warm_Reset(void) {
	// reset low
		Set_RESET();
	// set recieve
		//UART_input();
		//UART_Enable();
	// keep VCC && CLK
		// wait
			Card_wait();
	// reset_high
		Clear_RESET();
	
	// if ok 
		//communicate
	// ekse deactivate
		//Card_Deactivate();
}

void Card_Activate(void) {
	Calc_Default_Baudrate();
	Set_Comm_Baudrate(7168);
	
	init_UART();
	//
	Segger_write_string("Activating\n");
		// wait
			Card_wait();
	// VCC
		Set_VCC();
	// set recieve
		//UART_input();
	UART_Enable();
		// wait
			Card_wait();
	// CLK clk
		Start_CLK();
		// wait
			Card_wait();
	// reset high
		Clear_RESET();
		
		Set_Activated();
}

void Card_Deactivate(void) {
	Segger_write_string("Deactivating\n\n");
	// reset low
		Set_RESET();
		// wait
			Card_wait();
	// clk low
		Stop_CLK();
		//wait
			Card_wait();
	// io low
		//Clear_UART();
		UART_Disable();
		// wait
			Card_wait();
	// vcc low
		Clear_VCC();
	
		Set_Deactivated();
}


void Card_Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}
