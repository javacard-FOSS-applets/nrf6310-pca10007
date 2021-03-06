#include <stdio.h>
#include <string.h>
#include <stdint.h>

//typedef unsigned char uint8_t;
//typedef unsigned char uint16_t;

//static uint8_t PSK[16] = 		{ 0x02b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };  
//static uint8_t IVECTOR[16]	=	{ 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };

#include "Universal.h"
#include "PSK.h"
#include "aes.h"

// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1

void ENC_DEC_Test(void);

void test(uint8_t value) {
	uint8_t in[16];
	uint8_t message[17];
	uint8_t bufferout[16];				
	uint8_t recieved_value;

	message[0]=(uint8_t) 0x02;
	in[0]=(uint8_t) value;

	AES128_CBC_encrypt_buffer(message+1, in,  16, PSK, IVECTOR);
	
	AES128_CBC_decrypt_buffer(bufferout, message+1,  16, PSK, IVECTOR);
				//SW_AES_decode();
	recieved_value=bufferout[1];
}

/*int main(void)
{
    //ENC_DEC_Test();
		test(0x01);
    return 0;
}*/


void ENC_DEC_Test()
{
 int errorsfound;
 int i;
  //uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
  //uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  uint8_t in_t[16]  = { 'H', 'e',  'l',  'l',  'o',  'S',  't',  'r',  'i',  'n',  'g',  'T',  'e',  's', 't',  0x00 };
  uint8_t in[16]  	= { 'H', 'e',  'l',  'l',  'o',  'S',  't',  'r',  'i',  'n',  'g',  'T',  'e',  's', 't',  0x00 };
  //uint8_t out[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
  uint8_t buffer[16];
  uint8_t bufferout[16];
  
  //printf("Encrypting %s\n", in);
	//SEGGER_RTT_WriteString(0, "Encrypt: ");
	//SEGGER_RTT_WriteString(0, (const char *) in);
	//SEGGER_RTT_WriteString(0, "\n");
  AES128_CBC_encrypt_buffer(buffer, in,  16, PSK, IVECTOR);
  //SEGGER_RTT_WriteString(0, "Decrypt: ");
  AES128_CBC_decrypt_buffer(bufferout, buffer,  16, PSK, IVECTOR);
	
	bufferout[15]=0;
	//SEGGER_RTT_WriteString(0, (const char *) bufferout);
	//SEGGER_RTT_WriteString(0, "\n");
	
	
	errorsfound = 0;
	for(i=0; i<16; i++) {
		if(bufferout[i]!=in_t[i]) {
			errorsfound++;
		}
	}
		
	if(errorsfound==0){
		//SEGGER_RTT_WriteString(0, "Success!\n");
	}
	else {
		//SEGGER_RTT_WriteString(0, "Error!\n");
	}		
		
  /*if(0 == memcmp((char*) out, (char*) buffer, 16))
  {
    printf("SUCCESS!\n");
  }
  else
  {
    printf("FAILURE!\n");
  }*/
}
