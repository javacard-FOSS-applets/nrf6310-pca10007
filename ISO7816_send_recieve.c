#include <stdint.h>
#include "Universal.h"
#include "ISO7816.h"

void SC_Send_Message(uint8_t Lenght) {
	Segger_write_string("Sending Message to SC!\n");
	
	for(uint8_t i=0; i<Lenght; i++) {
		Send_UART(SC_APDU[i]);
	}
	Segger_write_string("\n");
}



uint8_t Recieve_Response(void) {
	//Card_wait_ETU_cycles(10);
	//UART_prepare_for_recieve();
	uint8_t Recieve_Count=0;
	Segger_write_string("\tRecieving response!\n\t");
			
	while(true) {
		uint8_t success=0;
		SC_Response[Recieve_Count]=Recieve_UART_timeout(DELAY_ETU_CYCLES * one_CLK_cycle, &success);
		
		if(!success) {
			break;
		}
		Segger_write_one_hex_value(SC_Response[Recieve_Count]);
		Recieve_Count++;
	}
	Segger_write_string("\n");
	
	SC_Response_Count=Recieve_Count;
	return Recieve_Count;
}

uint8_t Recieve_And_Check_Response() {
	uint8_t Recieve_Count = Recieve_Response();
	
	if(Recieve_Count<3) {
		SC_Response_Count=0;
		return 0;
	}
	
	if(Is_Valid_Message(1, Recieve_Count, SC_Response)) {
		Analyze_Message(Recieve_Count, SC_Response);
		SC_Response_Count=Recieve_Count;
		return Recieve_Count;
	}
	
	SC_Response_Count=0;
	return 0;
}

uint8_t Send_Message_Recieve_Response(uint8_t * Payload, uint8_t send_count, uint8_t * Message_Recieved) {
	Segger_write_string("Send-recieve!\n");

	uint8_t APDU_Length = Prepare_Standard_APDU(send_count, Payload);
	
	SC_Send_Message(APDU_Length);
	
	UART_prepare_for_recieve();
	uint8_t recieved_count = Recieve_And_Check_Response();
	
	return recieved_count;
}



void Send_Negotiate_Block_Protocol_Alone() { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	//SC_APDU[2] = 0x00;	//PTS1 
	//SC_APDU[3] = 0x00;  //PTS2
	//SC_APDU[4] = 0x00;	//PTS3
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	

	SC_Send_Message(3);	
}

void Send_Negotiate_Block_Protocol_APDU()  { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	
	uint8_t value = Prepare_Standard_APDU(3, SC_APDU);
	SC_Send_Message(value);
}

void Send_Negotiate_Block_Protocol_Block() { // Should negotiate protocl T=0
	Segger_write_string("Negotiating T=0!\n");
	// Negotiating new protocol via PTS 
	
	SC_APDU[0] = 0xFF;  //PTS request
	SC_APDU[1] = 0x00;	//PTS0 as TA1  0(RFU) 000(PTS 1 2 3 ) Protocol type 4 1
	SC_APDU[2] = 0xFF;  //Calc_XOR_Checksum(0, 2, SC_APDU);
	
	uint8_t value = Prepare_Standard_Block(3, SC_APDU);
	SC_Send_Message(value);
}