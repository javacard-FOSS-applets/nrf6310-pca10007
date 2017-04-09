#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> 
#include "compiler_abstraction.h"
#include "nrf.h"
#include "nrf_delay.h"

#include "Universal.h"

#define DELAY_ETU_CYCLES 40000
#define one_CLK_cycle 	 6

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0095a/ch03s02s07.html
//http://www.ruimtools.com/atr.php
//http://ludovic.rousseau.free.fr/softwares/pcsc-tools/smartcard_list.txt

//https://smartcard-atr.appspot.com/parse?ATR=3bea00008131fe450031c173c840000090007a
	//p5cn145
		//0x3bh 0xeah 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x00h 0x31h 0xc1h 0x73h 0xc8h 0x40h 0x00h 0x00h 0x90h 0x00h 0x7ah
		//3B EA 00 00 81 31 FE 45 00 31 C1 73 C8 40 00 00 90 00 7A

//https://smartcard-atr.appspot.com/parse?ATR=3bf81300008131fe454a434f5076323431b7
	//p5cn080
		//0x3bh 0xf8h 0x13h 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x4ah 0x43h 0x4fh 0x50h 0x76h 0x32h 0x34h 0x31h 0xb7h
		//3B F8 13 00 00 81 31 FR 45 4A 43 4F 50 76 32 34 31 B7


uint8_t ATR_Message[35];
uint8_t ATR_count=0;
uint8_t success=0;
uint8_t SC_Response[255]; // TODO buffer length
uint8_t SC_APDU[255]; // TODO buffer length

void Card_Deactivate(void);
uint8_t Send_Message_Recieve_Response(uint8_t * Message_Send, uint8_t send_count, uint8_t * Message_Recieved);

void Card_wait() {
	nrf_delay_us(DELAY_ETU_CYCLES * one_CLK_cycle);
}

uint8_t InverseByte(uint8_t byte) {
	uint8_t value = 0;

	value  = (byte & 0x01) << 7;
	value |= (byte & 0x02) << 5;
	value |= (byte & 0x04) << 3;
	value |= (byte & 0x08) << 1;

	value |= (byte & 0x10) >> 1;
	value |= (byte & 0x20) >> 3;
	value |= (byte & 0x40) >> 5;
	value |= (byte & 0x80) >> 7;

	return value;
}

void ATR() {
	// 3F || 3B T=0 Direct one parity byte
	// MAX 4 bytes
}


void Warm_Reset(void) {
	// reset low
	Set_RESET();
		// set recieve
		UART_input();
		// keep VCC && CLK
		// wait
	// reset_high
	Clear_RESET();
	
	// if ok communicate
	// ekse deactivate
	//Card_Deactivate();
}



void Card_Activate(void) {
		// wait
		Card_wait();
	// VCC
	Set_VCC();
	// set recieve
	UART_input();
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
	// reset low
	Set_RESET();
		// wait
		Card_wait();
	// clk low
	Stop_CLK();
		//wait
		Card_wait();
	// io low
	Clear_UART();
		// wait
		Card_wait();
	// vcc low
	Clear_VCC();
}

void Card_Cold_Reset(void) {
	Card_Deactivate();
	Card_Activate();
}

void init_ISO7816_pins(void) {
	Segger_write_string("Preparing RESET pin!\n");
		init_RESET();
	Segger_write_string("Preparing CLK pin!\n");
		init_CLK();
	Segger_write_string("Preparing UART!\n");
		init_UART();
	Segger_write_string("Preparing VCC pin!\n");
		init_VCC();
}

void test_Card(void ){
	ATR_count=0;

	Card_Activate();
	Warm_Reset();

	for(uint8_t i=0;i<25;i++) {
		success=0;
		ATR_Message[i]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		if(!success) {
			break;
		}
		Segger_write_one_hex_value(ATR_Message[i]);
		ATR_count++;
	}
	
	//Card_wait();
	Segger_write_string("\n");
	SC_APDU[0]=0xFF;
	Send_Message_Recieve_Response(SC_APDU, 1, SC_Response);
	
	Card_wait();
	Card_Deactivate();
	Card_wait();
}

void init_Card(void) {
	init_ISO7816_pins();
	
	Segger_write_string("Testing card!\n");
		test_Card();
	
	Card_Activate();
	//Card_Cold_Reset();
	//ATR();
	Warm_Reset();
	
	for(uint8_t i=0;i<25;i++) {
		//value=InverseByte(value);
		success=0;
		ATR_Message[i]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		if(!success) {
			break;
		}
		Segger_write_one_hex_value(ATR_Message[i]);
		ATR_count++;
	}
	
	Card_wait();
	
	Segger_write_string("\n");
	
	if(ATR_Message[0]==0x3B || ATR_Message[0]==0x3F) {
		Segger_write_string("Card CONNECTED succesfully!\n");
	}
	else {
		Segger_write_string("No Smart card connected!\n");
	}
}

//verify CRC

//calculate CRC

// add header
// 00
// 80
// 01
// 02
// length
// message
// CRC

uint8_t SC_Header[]= {0x80, 0x10, 0x01, 0x02};

uint8_t Send_Message_Recieve_Response(uint8_t * Message_Send, uint8_t send_count, uint8_t * Message_Recieved) {
	uint16_t Recieve_Count=0;
	Recieve_Count=0;

	//
	Segger_write_string("Sending data to SC!\n");
	
	for(uint8_t i; i<4; i++) {
		Send_UART(SC_Header[i]);
	}
	Send_UART(send_count);
	for(uint8_t i=0; i<send_count; i++) {
		Send_UART(Message_Send[i]);
	}
	Send_UART(0x7F);

	Segger_write_string("Sending data to SC!\n");
	
	UART_input();
	
	while(true) {
		success=0;
		SC_Response[Recieve_Count]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		
		if(!success) {
			break;
		}
		Segger_write_one_hex_value(SC_Response[Recieve_Count]);
		Recieve_Count++;
	}
	
	Segger_write_string("\n");
	return Recieve_Count;
}
