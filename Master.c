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

// Channel configuration. 
#define CHANNEL_0                       0x00                 /**< ANT Channel 0. */
#define CHANNEL_0_TX_CHANNEL_PERIOD     8192u                /**< Channel period 4 Hz. */
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

uint8_t recieved_value=0;

#define DEVICEID 0xffff

void init(void);
void SetLEDS(uint8_t);
void BlinkLEDS(uint8_t);
uint8_t ReadButtons(void);
int AddMessage(uint8_t*);
void write_hex_value(uint8_t value);

/**@brief Function for handling an error. 
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the error occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    for (;;)
    {
    }
}


/**@brief Function for setting up the ANT module to be ready for TX broadcast.
 *
 * The following commands are issued in this order:
 * - assign channel 
 * - set channel ID 
 * - open channel 
 */
static void ant_channel_master_broadcast_setup(void)
{
    uint32_t err_code;
    
    // Set Channel Number. 
    err_code = sd_ant_channel_assign(CHANNEL_0, 
                                     CHANNEL_TYPE_MASTER, 
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


/**@brief Function for handling ANT TX channel events. 
 *
 * @param[in] event The received ANT event to handle.
 */
static void channel_event_handle_transmit(uint32_t event)
{
    uint32_t err_code;
    
    switch (event)
    {
        // ANT broadcast success.
        // Send a new broadcast and increment the counter.
        case EVENT_TX:
            // Assign a new value to the broadcast data. 
            m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = ReadButtons();
            
            // Broadcast the data. 
            err_code = sd_ant_broadcast_message_tx(CHANNEL_0, 
                                                   BROADCAST_DATA_BUFFER_SIZE, 
                                                   m_broadcast_data);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}

/**@brief Function for handling ANT RX channel events. 
 *
 * @param[in] p_event_message_buffer The ANT event message buffer. 
 */
static void channel_event_handle_recieve(uint8_t* p_event_message_buffer)        
{
			switch (p_event_message_buffer[ANT_MSG_IDX_ID])
			{
				case MESG_BROADCAST_DATA_ID:    
						recieved_value=p_event_message_buffer[10];
						AddMessage(p_event_message_buffer);
						break;
						
				default:      
						break;
			}
			write_hex_value(recieved_value);
}

/**@brief Function for stack interrupt handling.
 *
 * Implemented to clear the pending flag when receiving 
 * an interrupt from the stack.
 */
void PROTOCOL_EVENT_IRQHandler(void)
{
}

/**@brief Function for handling SoftDevice asserts. 
 *
 * @param[in] pc          Value of the program counter.
 * @param[in] line_num    Line number where the assert occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void softdevice_assert_callback(uint32_t pc, uint16_t line_num, const uint8_t * p_file_name)
{
    for (;;)
    {
				SEGGER_RTT_WriteString(0, "Assert callback.\n");
    }
}


void HardFault_Handler(void)
{
    for (;;)
    {
				SEGGER_RTT_WriteString(0, "Hard fault occured\n");
    }
}

int main(void)
{    
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

    // Setup Channel_0 as a TX Master Only. 
    ant_channel_master_broadcast_setup();
    
    // Write counter value to last byte of the broadcast data.
    // The last byte is chosen to get the data more visible in the end of an printout
    // on the recieving end. 
    m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = ReadButtons();
  
    // Initiate the broadcast loop by sending a packet on air, 
    // then start waiting for an event on this broadcast message.
    err_code = sd_ant_broadcast_message_tx(CHANNEL_0, BROADCAST_DATA_BUFFER_SIZE, m_broadcast_data);
		
		//static uint8_t burst_setup[]={ADV_BURST_MODE_ENABLE, ADV_BURST_MODES_MAX_SIZE, 0,0,0,0,0,0};
		//err_code = sd_ant_adv_burst_config_set(burst_setup,sizeof(burst_setup));
    
		APP_ERROR_CHECK(err_code);
  
  	BlinkLEDS(2);
		
    uint8_t event;
    uint8_t ant_channel;
  	  
    for (;;)
    {   
        // Put CPU in sleep if possible. 
        err_code = sd_app_event_wait();
        APP_ERROR_CHECK(err_code);
    
        // Extract and process all pending ANT events as long as there are any left. 
        do
        {
            // Fetch the event. 
            err_code = sd_ant_event_get(&ant_channel, &event, event_message_buffer);
            if (err_code == NRF_SUCCESS)
            {
                // Handle event. 
                switch (event)
                {
                    case EVENT_TX:
												channel_event_handle_transmit(event);
                        break;

                    case EVENT_RX:				
                        channel_event_handle_recieve(event_message_buffer);
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
