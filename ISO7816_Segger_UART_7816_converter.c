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

#define CONFG 'G'

#define HELP1 'H'
#define HELP2 '?'

#define RESET 'R'

#define ACTIV 'T'
#define DEACT 't'
#define EXIT  'X'

#define TESTK 's'

// example inputs to segger rtt viewer
/*
	R
	05
		00 00 01 fF fe
	08	
		00 00 01 00 a4 00 00 a5
	0a	
		00 00 06 80 10 01 02 01 FF FF
	05
		00 00 00 00 00
		
		05
		00 a4 04 00 00
			0x60h 0x61h 0x07h
		05
		00 c0 00 00 07
			0xc0h 0xa0h 0x00h 0x00h 0x00h 0x03h 0x00h 0x00h 0x90h 0x00h

		0d
		80F0800F08 A0000000 03000000

		09
		000005 00a4040000ff 1010
													 0101

		05
		00 20 00 00 00
		05
		00 f2 60 00 00
		08
		00 80 F2 20 00 02 4F 00 
		0d
		00 20 00 00 08 00 00 00 00 00 00 00 00
*/


void Print_Help() {
	Segger_write_string(
	"\
	##############################################\n\
	########             HELP             ########\n\
	##############################################\n\n\
	Usage through Segger RTT viewer console:\n \
\t ? or H -help\n\n\
\t R -cold reset Smart Card\n\
\t T -Activate Smart Card\n\
\t t -Deactivate Cards VCC, CLK, RESET, IO\n\
\t G -CONFG  G0/1 Set protocol  \n\
\t\t GM BruteForceLocating Card Manager\n\
\t\t Gm Locating Card Manager\n\
\t\t GC Locate Classes\n\
\t\t GI Locate Instructiions\n\
\t\t GN Send Negotiation Block Protocol\n\
\t\t GS Try to Get Status Info\n\
\t\t GR Try read records\n\
\t\t Gc Try CPLCD\n\
\t\t DD Try to get Data info\n\
\t\t DB0xXXXXXXXX UART Baud rate set\n\
Message sending to card:\n \
\t First send how many data, then the actual message\n\
\t\t03\n\
\t\tXX XX XX\n\
\t\tor\n\
\t\tXXXXXX \n\
where X is [0-9, a-f, A-F]\n\
Actual message can be on multiple lines\n\n\n\
Set Segger options as:\n\
\tINPUT->Sending...->Send on Enter\n\
\tINPUT->Sending...->Block if FIFO full\n\
	##############################################\n\
	##############################################\n\n");
	
	// TODO! if you add next line, segger wont be able to send that long message
	//##############################################\n
	Card_wait();
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

uint32_t Hex_UINT(uint8_t offset, uint8_t* Segger_recieve_buffer, uint8_t recieved) {
	uint32_t value=0;
	
	uint8_t count=offset;
	
	while(count<=recieved && (count-offset)<9) {
		value=value << 4;
		if( !(Segger_recieve_buffer[count]==' ' || Segger_recieve_buffer[count]=='\t' || Segger_recieve_buffer[count]=='\n')) {
			value |= String_To_Dec(Segger_recieve_buffer[count], 1);
		}
		count++;
	}
	return value;
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
	
	uint8_t FLAG_run=1;
	
	while(FLAG_run) {
		uint8_t ready_to_recieve = SEGGER_RTT_HasData(0);
		
		if(ready_to_recieve) {
			//Segger_write_string("Data ready to be recieved\n");
			recieved = SEGGER_RTT_Read(0, Segger_recieve_buffer, 255);
			
			Segger_write_string_value("Data ready:", recieved);
			
			switch(Segger_recieve_buffer[0]) {
					case RESET:
											lenght=0;
											ready_to_send=0;
											Card_Deactivate();
											Set_Baudrate(UART_BAUDRATE_BAUDRATE_Baud7168);
											Card_Cold_Reset();
											SC_Recieve_ATR();
											SC_Analyze_ATR();
											break;
					case HELP1: 
					case HELP2: 
											Print_Help();
											break;
					case ACTIV: 
											Card_Activate();
											SC_Recieve_ATR();
											SC_Analyze_ATR();
											break;
			
					case DEACT:
											Card_Deactivate();				
											break;
					case EXIT:
											Card_Deactivate();
											FLAG_run=0;
											break;
					case CONFG:
											if(recieved>1) {
												switch(Segger_recieve_buffer[1]) {
													case '0': SC_ATR_Set_Protocol_Type(0x00); Segger_write_string("Recieved messege will be sent as APDU\n"); break;
													case '1': SC_ATR_Set_Protocol_Type(0x01); Segger_write_string("Recieved messege will be encapsulated in block\n");break;
													case 'M': Try_Locating_Card_Manager_Brute(); break;
													case 'm': Try_Locating_Card_Manager(); Get_Response(7); break;
													case 'C': Try_Locating_Classes(); break;
													case 'I': Try_Locating_Instructions(); break;
													case 'N': Send_Negotiate_Block_Protocol_Alone(); Recieve_And_Check_Response(); SC_ATR_Set_Protocol_Type(0x00); break;
													case 'S': Try_STATUS(); break;
													case 'D': Try_DATA(); break;
													case 'R': Try_RECORDS(); break;
													case 'c': Try_Card_Production_Life_Cycle_Data(); break;
													case 'B': if(recieved==0x0d){
																				//Segger_write_one_hex_value_32( Hex_UINT(3, Segger_recieve_buffer, recieved));
																				Set_Baudrate(Hex_UINT(3, Segger_recieve_buffer, recieved));
																						//strtol( (char*)(Segger_recieve_buffer+2), NULL, 32) );
																								
																		}
																		break;
													default: break;
												}
											}
											break;
					//case TESTK:
					//						test_Card();
					//						break;
					default:
											if(lenght==0) {
												Convert_To_Hex_String(2, Segger_recieve_buffer);
												lenght = SC_APDU[0];
												Segger_write_string_value("Waiting for additional bytes:", lenght);
											}
											else {
												ready_to_send += Convert_To_Hex_String(recieved, Segger_recieve_buffer);
												
												if(ready_to_send>=lenght) {
													
													if(SC_ATR_Get_Protocol_Type()==0) {
														SC_Send_Message(lenght);
													}
													else {
														uint8_t count = Prepare_Standard_Block(lenght, SC_APDU);
														SC_Send_Message(count);
													}
													
													Recieve_And_Check_Response();
													
													lenght=0;
													ready_to_send=0;
												}
											}
											break;
				}
		}
	}
	return 0;
}
