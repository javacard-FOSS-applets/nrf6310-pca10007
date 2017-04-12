#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>

#include "aes.h"

#include "SEGGER_RTT.h"

//########################	Defines				###############################

//#define PIN 						31

// Ports
#define	 	PORT0 					0
#define	 	PORT1 					8
#define 	PORT2 					16 //BUTTONS
#define 	PORT3 					24 //LEDS

// CLK ISO 7816 related
	#define CHANNEL_GPIOTE  0
	#define CHANNEL_PPI 		1

	#define PIN_CLK					5
	#define PIN_RESET				4
	#define PIN_VCC					6

	// UART
	#define PIN_TX					3  //Cannot do how to same pin
	#define PIN_RX 					2
	//#define PIN_TX_RX				2


// LEDS and buttons
#define 	BUTTONS 				PORT2
#define   LEDS 						PORT1

#define 	SYSCLK					16000000

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
uint8_t ButtonsChanged(void);

//########################	Segger debugging && info				###############################

void 			Segger_write_hex_value(uint8_t);
void			Segger_write_one_hex_value(uint8_t);
void 			Segger_write_string(const char *string);
void 			Segger_write_string_value(const char *string, uint8_t);
void 			Segger_write_one_hex_value_32(uint32_t) ;

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

void 			init_RF_segmenter(void);

//########################	Encryption decryption				###############################

void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);


	//########################	CLK_GEN											###############################

	void init_CLK(void);
	void Start_CLK(void);
	void Stop_CLK(void);

	void init_RESET(void);
	void Set_RESET(void);
	void Clear_RESET(void);

	//########################	VCC 												###############################

	void init_VCC(void);
	void Set_VCC(void);
	void Clear_VCC(void);

	//########################	UART												###############################

	void init_UART(void);
	void Send_UART(uint8_t);
	void Clear_UART(void);
	void UART_input(void);
	void UART_output(void);
	uint8_t Recieve_UART(void);
	uint8_t Recieve_UART_timeout(uint32_t, uint8_t*);
	void NRF_Clear_UART_Errors(void);
	
	//########################	ISO7816  										###############################
	void nrf_delay_us(uint32_t);
		
	void init_ISO7816_pins(void);
	void init_Card(void);
