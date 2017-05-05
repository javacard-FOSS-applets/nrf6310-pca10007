#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "nrf51.h"

#include "nrf51422_peripherals.h"
#include "nrf51_bitfields.h"
#include "stdio.h"
#include "stdlib.h"
#include "nrf_gpio.h"
#include "SEGGER_RTT.h"
#include "nrf_temp.h"
#include <stdio.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "Universal.h"

#define H_SYNC 		0x01
#define H_MSGLNG 	0x01
#define H_MSGID 	0x01

MessageBuffer transmit;
MessageBuffer recieve;

//void HandleMessage(MessageBuffer recievee) {
//	recievee.count=0;
//}

uint8_t AddMessage(uint8_t* p_event_message_buffer){
	uint16_t segment_start;
	uint16_t segment_end;
	
	segment_start = p_event_message_buffer[H_SYNC+H_MSGLNG+H_MSGID];
	segment_end = p_event_message_buffer[H_SYNC+H_MSGLNG+H_MSGID+1];
	
	if(segment_end<segment_start)
		segment_end+=255;
	
	//ak je vacsie
	uint16_t thisCount = 0;
	int16_t count = segment_end-segment_start;

	//if(!recieve.ready){
	
	//if(recieve.ready!=1){
		for(int i=0; i<6 && count--; i++) {
			recieve.message[recieve.count++] = p_event_message_buffer[H_SYNC+H_MSGLNG+H_MSGID+i+2];
			thisCount++;
			recieve.length++;
		}
//	}
	
	//}
	
	if( (segment_start + thisCount) == segment_end) {
		if((segment_end-segment_start)==0) { //recieve.count==0
			//recieved empty packet
			recieve.ready=0;
			recieve.count=0;
			return 0;
		}
			
		if(recieve.count!=0) {
			//spracuj message
			//data ready
			
			recieve.ready=1;
//			HandleMessage(recieve);
			recieve.count=0;
			return 1;
		}
	}
	else {
		if(recieve.length>=120) {
			#ifdef DEBUG_SEGMENTER_MESSAGES
				Segger_write_string("Error recieveing segment!\n");
			#endif
			recieve.count=0;
			recieve.length=0;
			recieve.count=0;
			recieve.ready=0;
			return 0;
		}
		
		//data not ready
		recieve.ready=0;
		return 0;
	}
	return 0;
}

/*
uint8_t message1[] ={	0xaa, 0x08, 0x01,
						0x00, 0x06,
							0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
								0x11};

uint8_t message11[] ={	0xaa, 0x08, 0x01,
						0x00, 0x03,
							0xff, 0xfc, 0xfb, 0x00, 0x00, 0x00,
								0x11};

uint8_t message2[] ={	0xaa, 0x08, 0x01,
						0x00, 0x08,
							0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
								0x11};
uint8_t message3[] ={	0xaa, 0x08, 0x01,
						0x06, 0x08,
							0x06, 0x07, 0x02, 0x03, 0x04, 0x05,
								0x11};


uint8_t message4[] ={	0xaa, 0x08, 0x01,
						0x00, 0x00,
							0x06, 0x07, 0x02, 0x03, 0x04, 0x05,
								0x11};
uint8_t message5[] ={	0xaa, 0x08, 0x01,
						0x01, 0x01,
							0x06, 0x07, 0x02, 0x03, 0x04, 0x05,
								0x11};

uint8_t message6[] ={	0xaa, 0x08, 0x01,
						0xf0, 0x02,
							0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
								0x11};
uint8_t message7[] ={	0xaa, 0x08, 0x01,
						0xf6, 0x02,
							0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
								0x11};
uint8_t message8[] ={	0xaa, 0x08, 0x01,
						0xfb, 0x02,
							0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
								0x11};

uint8_t sender[]= {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0xff};
uint8_t sender2[]= {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0xff,
				   0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0xff};

*/

static uint8_t send_counter=0;

void FillSendData(uint16_t count, uint8_t* message) {
	if(transmit.ready==1){
		transmit.ready=0;
		transmit.count=0;
		transmit.length=count;

		for(uint16_t i=0; i<count; i++) {
			transmit.message[i]=message[i];
		}
	}
}


uint8_t delay_in_send_cycles=0;

void SendData(uint8_t* messagebuffer, uint8_t Data_was_ready) {
	//uint8_t buffer[12];
	if (!Data_was_ready) {
		messagebuffer[0]=send_counter;
		messagebuffer[1]=send_counter;
	}
	else if (recieved_security==MSG_HW_SYMM && delay_in_send_cycles < 5) {
		messagebuffer[0]=send_counter;
		messagebuffer[1]=send_counter;
	}
	else {
		messagebuffer[0]=send_counter;
		messagebuffer[1]=send_counter + transmit.length - transmit.count;
	}
		 
	if (!Data_was_ready) {
		messagebuffer[2]=0; 
		
		for(int i=0; i<6 && i<6; i++) {
			messagebuffer[2+i] = 0;
		}
		send_counter++;
		#ifdef DEBUG_SEGMENTER_MESSAGES
			Segger_write_string("Sending empty.\n");
		#endif
	}
	else if (recieved_security==MSG_HW_SYMM && delay_in_send_cycles++ < 5) { //Global_Default_Security
		Segger_write_string("\t\t\t\tSkippppppppping!!!!!!");
		send_counter++;
	}
	else if((transmit.length==0 && transmit.count==0)) { //empty 
		messagebuffer[2]=0; 
		
		for(int i=0; i<6 && i<6; i++) {
			messagebuffer[2+i] = 0;
		}
		send_counter++;
		#ifdef DEBUG_SEGMENTER_MESSAGES
			Segger_write_string("Sending empty.\n");
		#endif
	}
	else {
		for(int i=0; i<6 && transmit.count<transmit.length; i++) {
			messagebuffer[2+i] = transmit.message[transmit.count++];
			send_counter++;
		}
	}

	if (!Data_was_ready) {
		
	}
	else if(transmit.count==transmit.length) {
		transmit.count=0;
		transmit.length=0;
		transmit.ready=1;
		delay_in_send_cycles=0;
		Global_Data_Ready_For_Transfer=false;
		#ifdef DEBUG_SEGMENTER_MESSAGES
			Segger_write_string("FINISHED.\n");
		#endif
	}
	else {
		transmit.ready=0;
		#ifdef DEBUG_SEGMENTER_MESSAGES
			Segger_write_string("Fragmenting.\n");
		#endif
	}

	//AddMessage(buffer);
}

void init_RF_segmenter() {
	delay_in_send_cycles=0;
	Global_Data_Ready_For_Transfer=false;
	transmit.ready=1;
	recieve.ready=0;
}
/*
int main(){

	AddMessage(message1);

	AddMessage(message11);

	AddMessage(message2);
	AddMessage(message3);

	AddMessage(message4);

	AddMessage(message5);

	AddMessage(message6);
	AddMessage(message7);
	AddMessage(message8);
	
	FillSendData(16, sender);
	while(!transmit.ready)
		SendData();
		
	SendData();
	SendData();

	FillSendData(32, sender2);
	while(!transmit.ready)
		SendData();
	
	SendData();

	return 0;
}*/


