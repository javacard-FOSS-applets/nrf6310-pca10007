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
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"

#include "Universal.h"

// Channel configuration. 
#define CHANNEL_0_TX_CHANNEL_PERIOD     65535u               /**< Channel period 4 Hz. 8192    x/32768 [s] */
#define CHANNEL_0_ANT_EXT_ASSIGN        0x00                 /**< ANT Ext Assign. */

// Channel ID configuration. 
#define CHANNEL_0_CHAN_ID_DEV_TYPE      0x02u                /**< Device type. */
#define CHANNEL_0_CHAN_ID_DEV_NUM       0x02u                /**< Device number. */
#define CHANNEL_0_CHAN_ID_TRANS_TYPE    0x01u                /**< Transmission type. */

// Miscellaneous defines. 
#define ANT_CHANNEL_DEFAULT_NETWORK     0x00                 /**< ANT Channel Network. */
#define ANT_MSG_IDX_ID                  1u   		             /**< ANT message ID index. */
#define ANT_EVENT_MSG_BUFFER_MIN_SIZE   32u                  /**< Minimum size of ANT event message buffer. */
#define BROADCAST_DATA_BUFFER_SIZE      8u                   /**< Size of the broadcast data buffer. */

// Static variables and buffers. 
static uint8_t m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE]; /**< Primary data transmit buffer. */
//static uint8_t m_counter = 1u;                               /**< Counter to increment the ANT broadcast data payload. */


#define DEVICEID 0xffff


/**@brief Function for handling an error. 
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the error occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
  Segger_write_string("App error! \n");  
	for (;;) {
	}
}


/**@brief Function for setting up the ANT module to be ready for TX broadcast.
 *
 * The following commands are issued in this order:
 * - assign channel 
 * - set channel ID 
 * - open channel 
 */
static void ant_channel_master_broadcast_setup(void) {
    uint32_t err_code;
    
		 err_code = sd_ant_channel_period_set(NORDIC_CHANNEL, CHANNEL_0_TX_CHANNEL_PERIOD);
	  APP_ERROR_CHECK(err_code);
	
	
    // Set Channel Number. 
    err_code = sd_ant_channel_assign(NORDIC_CHANNEL, 
                                     CHANNEL_TYPE_MASTER, 
                                     ANT_CHANNEL_DEFAULT_NETWORK, 
                                     CHANNEL_0_ANT_EXT_ASSIGN);
    
    APP_ERROR_CHECK(err_code);

    // Set Channel ID. 
    err_code = sd_ant_channel_id_set(NORDIC_CHANNEL, 
                                     CHANNEL_0_CHAN_ID_DEV_NUM, 
                                     CHANNEL_0_CHAN_ID_DEV_TYPE, 
                                     CHANNEL_0_CHAN_ID_TRANS_TYPE);
    APP_ERROR_CHECK(err_code);

	
		
    // Open channel. 
    err_code = sd_ant_channel_open(NORDIC_CHANNEL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling ANT TX channel events. 
 *
 * @param[in] event The received ANT event to handle.
 */
static void channel_event_handle_transmit(uint32_t event) {
	uint32_t err_code;
	uint8_t Data_was_ready;
	
	uint8_t Pending_Transmit=0;
	sd_ant_pending_transmit(NORDIC_CHANNEL, &Pending_Transmit);
	
	if(Pending_Transmit) {
		Segger_write_string("TRANSMIT PENDING!!!");
		//return;
	}
	
	switch (event) {
			case EVENT_TX:
					
					// Assign a new value to the broadcast data. 
					Segger_write_string("Event TX.\n");
			
					if(GLobal_Test_Mode_Active) {
						Check_If_Recieved();
					}
					else {
						ReadButtons();
					}
			
					Data_was_ready=Global_Data_Ready_For_Transfer;
			
					if(dataready==1 && transmit.ready==1 && Global_Data_Ready_For_Transfer==false) { // imp
						dataready=0; //!!!!
						//uint8_t message[1];
						//message[0] = ReadButtons();
						//FillSendData(1, message);
						//#ifdef DEBUG_SEGMENTER_MESSAGES
							Segger_write_string("Data ready.\n");
						//#endif
						if(GLobal_Test_Mode_Active){
							EnCode(Global_Default_Security, Get_Actual_Test_Value());
						}
						else {
							EnCode(Global_Default_Security, ReadButtons());
						}
					}

					SendData(m_broadcast_data, Data_was_ready);
				
					// Broadcast the data. 
					err_code = sd_ant_broadcast_message_tx(NORDIC_CHANNEL, 
																								 BROADCAST_DATA_BUFFER_SIZE, 
																								 m_broadcast_data);
					APP_ERROR_CHECK(err_code);						
					
					#ifdef DEBUG_SEGMENTER_MESSAGES
						Segger_write_string("Sending values:");
						for(uint16_t i=0; i<8; i++)
							Segger_write_one_hex_value(m_broadcast_data[i]);
						Segger_write_string("\n");
					#endif
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

	switch (p_event_message_buffer[ANT_MSG_IDX_ID]) {
		case MESG_BROADCAST_DATA_ID:    
				//recieved_value=p_event_message_buffer[5];
				Segger_write_string("Event RX.\n");
				success = AddMessage(p_event_message_buffer);	
				
				if(success==1){
					#ifdef DEBUG_SEGMENTER_MESSAGES
						Segger_write_string("Recieve SUCCESS\n");
					#endif
					//recieved_value=p_event_message_buffer[5];
					
					//copy string
					/*uint8_t message_received[64];
					for(uint16_t i=0; i<recieve.count; i++){
						message_received[i]=recieve.message[i];
					}*/
					//decode
					Decode(recieve.length, recieve.message);
					recieve.ready=0;
					recieve.length=0;
					
					//SetLEDS
					//SetLEDS(recieved_value);
				}
				else {
					#ifdef DEBUG_SEGMENTER_MESSAGES
						Segger_write_string("Recieved EMPTY\n");
					#endif
				}
				
				#ifdef DEBUG_SEGMENTER_MESSAGES
					Segger_write_string("Recieved values:");
					for(uint16_t i=0; i<12; i++)
						Segger_write_one_hex_value(p_event_message_buffer[i]);
					Segger_write_string("\n");
				#endif
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
	//SEGGER_RTT_WriteString(0, "IRQHandler! \n");
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


void HardFault_Handler(void) {
	Segger_write_string("Hard fault occured\n");
	for (;;) {
	}
}


int main(void) {    
	init();

	Wait_For_Button_Press(1);

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

	// Setup Channel_0 as a TX Master Only. 
	ant_channel_master_broadcast_setup();
	
	// Write counter value to last byte of the broadcast data.
	// The last byte is chosen to get the data more visible in the end of an printout
	// on the recieving end. 
	dataready=1;
	//EnCode(MSG_UNSECURED, ReadButtons());
EnCode(Global_Default_Security, ReadButtons());
	//m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = ReadButtons();

	// Initiate the broadcast loop by sending a packet on air, 
	// then start waiting for an event on this broadcast message.
	err_code = sd_ant_broadcast_message_tx(NORDIC_CHANNEL, BROADCAST_DATA_BUFFER_SIZE, m_broadcast_data);
	
	//static uint8_t burst_setup[]={ADV_BURST_MODE_ENABLE, ADV_BURST_MODES_MAX_SIZE, 0,0,0,0,0,0};
	//err_code = sd_ant_adv_burst_config_set(burst_setup,sizeof(burst_setup));
	
	APP_ERROR_CHECK(err_code);

	BlinkLEDS(2);
	
	uint8_t event;
	uint8_t ant_channel;
		
	for (;;) {   
		// Put CPU in sleep if possible. 
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
										Wait_until_not_transmitting();
										channel_event_handle_transmit(event);
										break;

								case EVENT_RX:
										channel_event_handle_recieve(event_message_buffer);
										if(GLobal_Test_Mode_Active) {
											Test_Compare_Recieved_Value(recieved_value);
										}
										SetLEDS(recieved_value);
										//SEGGER_RTT_WriteString(0, "Receiving.\n");
										break;
								//case BURST_SEGMENT_START:
											//SVC_ANT_ADV_BURST_CONFIG_SET();
								//
								//ADV_BURST_MODES_SIZE_24_BYTES
//												break;
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
