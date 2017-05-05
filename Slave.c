/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "SEGGER_RTT.h"
#include <string.h>
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
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "Universal.h"

// Channel configuration. 
#define CHANNEL_0                       0x00 /**< ANT Channel 0. */
//#define CHANNEL_0_TX_CHANNEL_PERIOD     8192u                /**< Channel period 4 Hz. */
#define CHANNEL_0_ANT_EXT_ASSIGN        0x00 /**< ANT Ext Assign. */

// Channel ID configuration. 
#define CHANNEL_0_CHAN_ID_DEV_TYPE      0x00 /**< Device type *wildcard*. */
#define CHANNEL_0_CHAN_ID_DEV_NUM       0x00 /**< Device number *wildcard*. */
#define CHANNEL_0_CHAN_ID_TRANS_TYPE    0x00 /**< Transmission type *wildcard*. */

// Miscellaneous defines. 
#define ANT_CHANNEL_DEFAULT_NETWORK     0x00 /**< ANT Channel Network. */
#define ANT_MSG_IDX_ID                  1u   /**< ANT message ID index. */
#define ANT_EVENT_MSG_BUFFER_MIN_SIZE   32u  /**< Minimum size of ANT event message buffer. */
#define BROADCAST_DATA_BUFFER_SIZE      8u   /**< Size of the broadcast data buffer. */

// Static variables and buffers. 
static uint8_t m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE]; /**< Primary data transmit buffer. */
//static uint8_t m_counter = 1u;                               /**< Counter to increment the ANT broadcast data payload. */

#define DEVICEID 0xaaaa

/**@brief Function for handling an error. 
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the error occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
  SEGGER_RTT_WriteString(0, "App error!\n");  
	for (;;) {
	}
}

/**@brief Function for setting up the ANT module to be ready for RX broadcast.
 *
 * The following commands are issued in this order:
 * - assign channel
 * - set channel ID
 * - open channel
 */
static void ant_channel_slave_broadcast_setup(void) {
	uint32_t err_code;

	// Set Channel Number.
	err_code = sd_ant_channel_assign(CHANNEL_0, 
																	 CHANNEL_TYPE_SLAVE, 
																	 ANT_CHANNEL_DEFAULT_NETWORK, 
																	 CHANNEL_0_ANT_EXT_ASSIGN);
	APP_ERROR_CHECK(err_code);

	// Set Channel ID.
	err_code = sd_ant_channel_id_set(CHANNEL_0, 
																	 CHANNEL_0_CHAN_ID_DEV_NUM, 
																	 CHANNEL_0_CHAN_ID_DEV_TYPE, 
																	 CHANNEL_0_CHAN_ID_TRANS_TYPE);
	APP_ERROR_CHECK(err_code);

	// Open channel.
	err_code = sd_ant_channel_open(CHANNEL_0);
	APP_ERROR_CHECK(err_code);
}

static void send_reverse_data() {
	uint32_t err_code;

	//m_broadcast_data[2] = ; //this does the job
	
//						SEGGER
	#ifdef  DEBUG_SEGMENTER_MESSAGES
		Segger_write_string("Dataready: ");
		Segger_write_one_hex_value(dataready);
		Segger_write_string(" Transmitready: ");
		Segger_write_one_hex_value(transmit.ready);
		Segger_write_string("\n");
	#endif

	if(dataready==1 && transmit.ready==1) {
		//transmit.ready=0;
		//uint8_t message[1];
		//message[0] = recieved_value;
		//FillSendData(1, message);
		//SEGGER_RTT_WriteString(0, "Event.\n");
		
		EnCode(recieved_security, recieved_value);
		
		//recieve.length=0;
		dataready=0; //!!!!
	}

	SendData(m_broadcast_data);
	
	// Broadcast the data. 
	err_code = sd_ant_broadcast_message_tx(CHANNEL_0, 
																				 BROADCAST_DATA_BUFFER_SIZE, 
																				 m_broadcast_data);
	APP_ERROR_CHECK(err_code);

	Segger_write_string("Sending values:");
	for(uint16_t i=0; i<8; i++)
		Segger_write_one_hex_value(m_broadcast_data[i]);
	Segger_write_string("\n");
}

/**@brief Function for handling ANT TX channel events. 
 *
 * @param[in] event The received ANT event to handle.
 */
static void channel_event_handle_transmit(uint32_t event) {
//    uint32_t err_code;
    
	switch (event) {
		case EVENT_TX:
				Segger_write_string("Event TX.\n")
				/*// Assign a new value to the broadcast data. 
				m_broadcast_data[2] = recieved_value; //BROADCAST_DATA_BUFFER_SIZE - 1
				
				// Broadcast the data. 
				err_code = sd_ant_broadcast_message_tx(CHANNEL_0, 
																							 BROADCAST_DATA_BUFFER_SIZE, 
																							 m_broadcast_data);
				APP_ERROR_CHECK(err_code);*/
		
				send_reverse_data();
				
	//SEGGER_RTT_WriteString(0, "Sending2.\n");
				break;
		default:
				break;
	}
}

/**@brief Function for handling ANT RX channel events. 
 *
 * @param[in] p_event_message_buffer The ANT event message buffer. 
 */
static void channel_event_handle_recieve(uint8_t* p_event_message_buffer) {
	uint8_t success=0;
	int err_code;
	//SEGGER_RTT_WriteString(0, "Recieved EVENT\n");
	
	switch (p_event_message_buffer[ANT_MSG_IDX_ID]) {
		case MESG_BROADCAST_DATA_ID:
			
				Segger_write_string("Event RX.\n")
					
				Segger_write_string("Recieved values:");
				for(uint16_t i=0; i<12; i++)
					Segger_write_one_hex_value(p_event_message_buffer[i]);			
				SEGGER_RTT_WriteString(0, "\n");	
			
				success = AddMessage(p_event_message_buffer);	
		
				if(success==1){
					Segger_write_string("Recieve SUCCESS\n");
					
					//recieved_value=p_event_message_buffer[5];
					Decode(recieve.length, recieve.message);
					
					recieve.length=0;
					dataready=1;
					
					err_code = sd_ant_broadcast_message_tx(CHANNEL_0, 
																							 BROADCAST_DATA_BUFFER_SIZE, 
																							 m_broadcast_data);
					APP_ERROR_CHECK(err_code);
				}
				else {
					//SEGGER_RTT_WriteString(0, "Recieved EMPTY\n");
				}						
				break;
				
		default:      
				break;
	}
	Segger_write_hex_value(recieved_value);
}

/**@brief Function for stack interrupt handling.
 *
 * Implemented to clear the pending flag when receiving 
 * an interrupt from the stack.
 */
void PROTOCOL_EVENT_IRQHandler(void) {

}

/**@brief Function for handling SoftDevice asserts. 
 *
 * @param[in] pc          Value of the program counter.
 * @param[in] line_num    Line number where the assert occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void softdevice_assert_callback(uint32_t pc, uint16_t line_num, const uint8_t * p_file_name) {
	Segger_write_string("Assert callback.\n");
	for (;;) {
	}
}

/**@brief Function for handling HardFault.
 */
void HardFault_Handler(void) {
	Segger_write_string("Hard fault occured\n");
	for (;;) {
	}
}

/**@brief Function for application main entry. Does not return.
 */
int main(void) {
    // ANT event message buffer.
	init();
	static uint8_t event_message_buffer[ANT_EVENT_MSG_BUFFER_MIN_SIZE]; 
		
	// Enable SoftDevice.
	uint32_t err_code;
	err_code = sd_softdevice_enable(NRF_CLOCK_LFCLKSRC_XTAL_50_PPM, softdevice_assert_callback);
	APP_ERROR_CHECK(err_code);

	// Set application IRQ to lowest priority.
	err_code = sd_nvic_SetPriority(PROTOCOL_EVENT_IRQn, NRF_APP_PRIORITY_LOW); 
	APP_ERROR_CHECK(err_code);

	// Enable application IRQ (triggered from protocol).
	err_code = sd_nvic_EnableIRQ(PROTOCOL_EVENT_IRQn);
	APP_ERROR_CHECK(err_code);

	// Setup Channel_0 as a RX Slave.
	ant_channel_slave_broadcast_setup();

	uint8_t event;
	uint8_t ant_channel;

	// Main loop.
	for (;;) {   
		// Put CPU in sleep if possible
		err_code = sd_app_event_wait();
		APP_ERROR_CHECK(err_code);
		
		// Extract and process all pending ANT events as long as there are any left.      
		do {
			// Fetch the event.
			err_code = sd_ant_event_get(&ant_channel, &event, event_message_buffer);
			if (err_code == NRF_SUCCESS) {
					// Handle event.
				switch (event) {
					case EVENT_TX:
							channel_event_handle_transmit(event);
//												SEGGER_RTT_WriteString(0, "Sending1.\n");
							break;
					case EVENT_RX:
							channel_event_handle_recieve(event_message_buffer);
//												SEGGER_RTT_WriteString(0, "Receiving.\n");
							break;
					default:
							break;
				}
			}          
		} 
		while (err_code == NRF_SUCCESS);
	}
}

/**
 *@}
 **/
