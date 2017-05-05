#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Universal.h"

#include "PSK.h"

	uint8_t bufferout[55];
	uint8_t store_message[55];

void Decode(uint16_t count, uint8_t* message) {
	Segger_write_string_value("Count: ", count);
	
	if(count==0){
		//Empty message recieved	
		return;
	}
	
	Segger_write_string("Message recieved: ");
	for(int i=0; i<count; i++)
		Segger_write_one_hex_value(message[i]);
	Segger_write_string("\n");
	
	recieved_security=message[0];
	
	Segger_write_string_value("  Security type: ", recieved_security);
	
	switch((security_type)message[0]){
		case MSG_UNSECURED:
			if(count==2) {
				recieved_value=message[1];
				//recieve.ready=0;
			}
			break;
		
		case MSG_SW_SYMM:
			if(count==17) {
				for(int i=0; i<16; i++)
					store_message[i]=message[i+1];
				
				AES128_CBC_decrypt_buffer(bufferout, store_message, 16, PSK, IVECTOR);
				//SW_AES_decode();
				recieved_value=bufferout[0];
			}
			break;
		/*case MSG_SW_ASYMM:
			//if(count==XXX)
			//	SW_RSA_decode();
			break;*/
		
		case MSG_HW_SYMM:
			if(count==17) {
				recieved_value=HW_AES_Decode(message);
			}
			break;
		/*case MSG_HW_ASYMM:
			//if(count==XXX)
			//	HW_RSA_decode();
			break;*/
		default:
			//unexpected type
			break;
	}
	
	Segger_write_string_value("\nReceived value: ", recieved_value);
}

//!!!! does not know, if the data will be sent, because, transmiter can be busy


	static uint8_t send_message_uns[1+1];
	
	static uint8_t in[55];
	static uint8_t message[55];

void EnCode(security_type security, uint8_t value) {
//	uint8_t send_message_aes[16+1];
//	uint8_t send_message_sym[64+1];
	
	switch(security){
		case MSG_UNSECURED:
				if(transmit.ready==1) {		
					send_message_uns[0]=(uint8_t) security;
					send_message_uns[1]= value;
			
					//write_one_hex_value(value);

					FillSendData(2, send_message_uns);
				}
			break;
		
		case MSG_SW_SYMM:
				message[0]=(uint8_t) security;
				in[0]=(uint8_t) value;
				
				//SEGGER_RTT_WriteString(0, "Starting encoding \n");
				AES128_CBC_encrypt_buffer(message+1, in,  16, PSK, IVECTOR);
				//SEGGER_RTT_WriteString(0, "Ended encoding \n");
		
				/*for(int i=1; i<17; i++)
					message[i]=(uint8_t) i;*/
				FillSendData(17, message);
				break;
		/*case MSG_SW_ASYMM:
			//if(count==XXX)
			//	SW_RSA_Encode();
			break;*/
		
		case MSG_HW_SYMM:
			
				/*message[0]=(uint8_t) security;
				in[0]=(uint8_t) value;
				AES128_CBC_encrypt_buffer(message+1, in,  16, PSK, IVECTOR);
				FillSendData(17, message);*/
				
				HW_AES_Encode(message, value);
				message[0]=security;
				FillSendData(17, message);
			break;
		
		/*case MSG_HW_ASYMM:
			//if(count==XXX)
			//	HW_RSA_Encode();
			break;*/
		
		default:
			//unexpected type
			break;
	}
	Global_Data_Ready_For_Transfer=true;
	Segger_write_string("\t\tData prepared for sending!");
}
