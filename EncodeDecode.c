#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Universal.h"

void Decode(uint16_t count, uint8_t* message) {
	SEGGER_RTT_WriteString(0, "Decode: ");
	write_one_hex_value(count);
	if(count==0){
		//Empty message recieved	
		return;
	}
	SEGGER_RTT_WriteString(0, "  Security type: ");
	write_one_hex_value(message[0]);
	
	switch((security_type)message[0]){
		case MSG_UNSECURED:
			if(count==2) {
				recieved_value=message[1];
				
				SEGGER_RTT_WriteString(0, "\nReceived value: ");
				write_one_hex_value(recieved_value);
				//recieve.ready=0;
				//dataready=1; //ok for SLAVE, not for a master
			}
			break;
		
		case MSG_SW_SYMM:
			//if(count==17)
			//	SW_AES_decode();
			//	dataready=1;
			break;
		case MSG_SW_ASYMM:
			//if(count==XXX)
			//	SW_RSA_decode();
			//	dataready=1;
			break;
		
		case MSG_HW_SYMM:
			//if(count==17)
			//	HW_AES_decode();
			//	dataready=1;
			break;
		case MSG_HW_ASYMM:
			//if(count==XXX)
			//	HW_RSA_decode();
			//	dataready=1;
			break;
		default:
			//unexpected type
			break;
	}
}

void EnCode(security_type security, uint8_t value) {
	uint8_t send_message_uns[1+1];
//	uint8_t send_message_aes[16+1];
//	uint8_t send_message_sym[64+1];
	
	switch(security){
		case MSG_UNSECURED:
				send_message_uns[0]=(uint8_t) security;
				send_message_uns[1]= value;
		
				//write_one_hex_value(value);
		
				if(transmit.ready==1) {
					FillSendData(2, send_message_uns);
				}
			break;
		
		case MSG_SW_SYMM:
			//if(count==17)
			//	SW_AES_Encode();
			break;
		case MSG_SW_ASYMM:
			//if(count==XXX)
			//	SW_RSA_Encode();
			break;
		
		case MSG_HW_SYMM:
			//if(count==17)
			//	HW_AES_Encode();
			break;
		case MSG_HW_ASYMM:
			//if(count==XXX)
			//	HW_RSA_Encode();
			break;
		
		default:
			//unexpected type
			break;
	}
}
