#include "Universal.h"
#include "ISO7816.h"

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
	
	// if ok communicate
	// ekse deactivate
	//Card_Deactivate();
}

void Card_Activate(void) {
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
}


void Card_Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}
