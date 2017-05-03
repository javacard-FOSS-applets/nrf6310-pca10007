#include "Universal.h"
#include "iso7816.h"

#define HW_AES_ENCRYPT 0x03
#define HW_AES_DECRYPT 0x04
#define HW_RSA_ENCRYPT 0x05
#define HW_RSA_DECRYPT 0x06

// todo test
uint8_t Retrieve_Message(uint8_t * Message, uint8_t count) {
	if(SC_Response_Count<count) {
		return 0;
	}
	
	uint8_t offset=0;
	if(SC_ATR_Get_Protocol_Type()==0x01) {
		offset=1;
	}
	else {
		offset=0;
	}
	
	for(uint8_t i=0; i<count; i++) {
		Message[i] = SC_Response[SC_Response_Count-count-2-offset+i];
	}
	
	return 1;
}

uint8_t ED_SC_Message_Wrap(uint8_t * message) {
	//reciever response //retrieve message
	if(SC_ATR_Get_Protocol_Type()==0) {
			uint8_t count = Prepare_Standard_APDU(17, message);
			Send_And_Recieve(count);
	}
	else {
			uint8_t count = Prepare_Standard_APDU_Block(17, message);
			Send_And_Recieve(count);
	}
	
	//todo retransmit if no succesfull// for now drop the message
	if(Check_Succesfull_Execution_of_Instruction()) {
		Segger_write_string("Encrypt/decrypt operations succesful.");
		return 1;
	}
	Segger_write_string("Encrypt/decrypt operations NOT succesful!");
	return 0;
}



void HW_AES_Encode(uint8_t * message, uint8_t value) {
	//prepare array 16+1 padding
	message[0]=HW_AES_ENCRYPT;
	
	for(uint8_t i=0; i<16; i++){
		message[i+1]=0x00;
	}
	message[1]=value;
	
	if(ED_SC_Message_Wrap(message)) {
		//retrieve message
		Retrieve_Message(message, 16); //12
	}
}

uint8_t HW_AES_Decode(uint8_t * message) {
	//prepare selector
	message[0]=HW_AES_DECRYPT;
	
	if(ED_SC_Message_Wrap(message)) {
		Retrieve_Message(message, 16); //12
		//return  valuse
		return message[0];
	}
	return 0xFF;
}
