#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>

#include "aes.h"

#include "SEGGER_RTT.h"

//########################	Defines				###############################

//#define PIN 						31

// Ports
#define PORT0 					0
#define PORT1 					8
#define PORT2 					16 //BUTTONS
#define PORT3 					24 //LEDS

// UART
#define PIN_TX_RX				0
#define PIN_TX					0
#define PIN_RX 					1

// CLK
#define GPIOTE_CHANNEL  0
#define PPI_CHANNEL 		1
#define CLK							7

// LEDS and buttons
#define BUTTONS 				PORT2
#define    LEDS 				PORT1

//########################	Structs for project				###############################

typedef enum {MSG_EMPTY=0x00,
							MSG_UNSECURED=0x01,
							MSG_SW_SYMM=0x02, MSG_SW_ASYMM=0x03,
							MSG_HW_SYMM=0x04, MSG_HW_ASYMM=0x05} security_type;

		#define DEFAULT_SECURITY (MSG_SW_SYMM)
		//security_type security = MSG_EMPTY;

typedef struct {
	uint16_t length;
	uint8_t count;
	uint8_t message[64];
	uint8_t ready;
} MessageBuffer;

//########################	Main init 											###############################

void 			init(void);


//########################	Primal GPIO											###############################

void 			SetLEDS(uint8_t);
void 			BlinkLEDS(uint8_t);
uint8_t 	ReadButtons(void);


//########################	Segger debugging && info				###############################

void 			write_hex_value(uint8_t);
void			write_one_hex_value(uint8_t);


//########################	Radio message segmentation functions		###############################

uint8_t 	AddMessage(uint8_t*);

void		 	SendData(uint8_t*);
void 			FillSendData(uint16_t, uint8_t*);


//########################	Message over radio encode decode				###############################

void Decode(uint16_t, uint8_t*);
void EnCode(security_type, uint8_t);


//########################	Radio message send recieve	###############################

extern 		MessageBuffer transmit;
extern 		MessageBuffer recieve;
extern 		uint8_t dataready;

extern 		uint8_t recieved_value;
extern 		uint8_t recieved_security;


//########################	Encryption decryption				###############################

void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);


//########################	CLK_GEN											###############################

void Timer_Start(void);
void Timer_Stop(void);

void init_GPIOTE(void);
void init_PPI(void);
void init_Timer2(void);


//########################	UART												###############################

void init_UART(void);
void Send_UART(uint8_t byte);
uint8_t Recieve_UART(void);
