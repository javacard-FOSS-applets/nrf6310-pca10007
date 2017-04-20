#include "stdint.h"
#include "SEGGER_RTT.h"

#include "Universal.h"
#include "ISO7816.h"

/*
	SEGGER ADAPTER ADAPTOR CONVERTER
	SEGGET TO UART TO ISO7816
*/

uint8_t recieved_value=0;
uint8_t recieved_security=0;


uint8_t ready_to_send=0;


uint8_t Segger_recieve_buffer[255];
uint8_t HEX_String[255];

#define HELP1 'H'
#define HELP2 '?'

#define RESET 'R'

#define ACTIV 'T'
#define DEACT 't'
#define EXIT  'X'

// example inputs to segger rtt viewer
/*
	R
	05
		00 00 01 fF fe
	08	
		00 00 01 00 a4 00 00 a5
	09	
		00 00 80 10 01 02 01 FF FF
	05
		00 00 00 00 00
*/


void Print_Help() {
	Segger_write_string(
	"\
	##############################################\n\
	########             HELP             ########\n\
	##############################################\n\n\
	Usage through Segger RTT viewer console:\n \
\t ? or H -help\n\n\n\
\t R -cold reset Smart Card\n\
\t T -Activate Smart Card\n\
\t t -Deactivate Cards VCC, CLK, RESET, IO\n\
Message sending to card:\n \
\t First send how many data, then the actual message\n\
\t\t03\n\
\t\tXX XX XX\n\
\t\tor\n\
\t\tXXXXXX \n\
where X is [0-9, a-f, A-F]\n\
Actual message can be on multiple lines\n\n\n\
Set Segger options as:\
\tINPUT->Sending...->Send on Enter\n\
\tINPUT->Sending...->Block if FIFO full\n\
	##############################################\n\
	##############################################\n\
	##############################################\n\n\n\n");
}


uint8_t String_To_Dec(uint8_t value, uint8_t position) {
	uint8_t return_value=0;

	//Segger_write_string_value(" Value:", value);
	
	if(value>='0' && value<='9') {
		return_value=value-'0';
	}
	if(value>='a' && value<='f') {
		return_value=10+value-'a';
	}
	if(value>='A' && value<='F') {
		return_value=10+value-'A';
	}

	//Segger_write_string_value(" Converted:", return_value);
	
	if(position==0) {
		return return_value<<4;
	}
	
	return return_value;
}

uint8_t Convert_To_Hex_String(uint8_t recieved, uint8_t * Segger_recieve_buffer) {
	uint8_t count=0;
	uint8_t lenght=0;
	
	SC_APDU[lenght+ready_to_send]=0;
	
	for(uint8_t i=0; i<recieved; i++) {
		if(Segger_recieve_buffer[i]==' ' || Segger_recieve_buffer[i]=='\t' || Segger_recieve_buffer[i]=='\n') {
			;
		}
		else {
			SC_APDU[lenght+ready_to_send]|=String_To_Dec(Segger_recieve_buffer[i], count++);
	
			if(count>=2) {
				count=0;
				lenght++;
				SC_APDU[lenght+ready_to_send]=0;
			}
		}
	}
	return lenght;
}

int main(void) {
	Print_Help();
	
	init_ISO7816_pins();
	uint8_t recieved=0;
	
	ready_to_send=0;
	
	uint8_t lenght=0;
	while(true) {
		uint8_t ready_to_recieve = SEGGER_RTT_HasData(0);
		
		if(ready_to_recieve) {
			//Segger_write_string("Data ready to be recieved\n");
			recieved = SEGGER_RTT_Read(0, Segger_recieve_buffer, 255);
			
			Segger_write_string_value("Data ready:", recieved);
			
			if(Segger_recieve_buffer[0]==RESET) {
				lenght=0;
				ready_to_send=0;
				Card_Cold_Reset();
				SC_Recieve_ATR();
				SC_Analyze_ATR();
			}
			else if(Segger_recieve_buffer[0]==HELP1 || Segger_recieve_buffer[0]==HELP2) {
				Print_Help();
			}
			else if(Segger_recieve_buffer[0]==ACTIV) {
				Card_Activate();
				SC_Recieve_ATR();
				SC_Analyze_ATR();
			}
			else if(Segger_recieve_buffer[0]==DEACT) {
				Card_Deactivate();				
			}
			else if(Segger_recieve_buffer[0]==EXIT) {
				Card_Deactivate();
				break;
			}
			else {
				if(lenght==0) {
					Convert_To_Hex_String(2, Segger_recieve_buffer);
					lenght = SC_APDU[0];
					Segger_write_string_value("Waiting for additional bytes:", lenght);
				}
				else {
					ready_to_send += Convert_To_Hex_String(recieved, Segger_recieve_buffer);
					
					if(ready_to_send>=lenght) {
						SC_Send_Message(lenght);
						Recieve_And_Check_Response();
						
						lenght=0;
						ready_to_send=0;
					}
				}
			}
		}
	}
	return 0;
}
