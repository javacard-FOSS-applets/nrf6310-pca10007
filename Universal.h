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
	
	#define ISO_7816_MAX_ATR_BYTES 33
	#define ISO_7816_DIRECT_CONV	 0x3B
	#define ISO_7816_INVERSE_CONV	 0x3F
	
	#define SC_MAX_MEM_BUFFER			 255
	
	#define UART_BAUDRATE_BAUDRATE_Baud7467 0x001e9000 //2002944 decimal
	#define UART_BAUDRATE_BAUDRATE_Baud7168 0x001d6000 //7461 real baud


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
	uint8_t message[5];
	uint8_t ready;
} MessageBuffer;

typedef struct {
	// prologue
	uint8_t NAD;  // 0x00
	uint8_t PCB;	// information, recieve ready, supervisory
	uint8_t LEN;  // 0x01
	
	// information field
	uint8_t* message[6]; // 0xFF
	
	// epilogue field
	// longitudinal redundancy check
	uint8_t LRC;
	
	//CRC cyclic redundancy check
	uint16_t CRC;
} SC_BlockFrame;

#define LRC_OFFSET_APDU  1
#define LRC_OFFSET_BLOCK 0


//########################	Main init 											###############################

void 			init(void);


//########################	Primal GPIO											###############################

void 			SetLEDS(uint8_t);
void 			BlinkLEDS(uint8_t);
uint8_t 	ReadButtons(void);
uint8_t	 	ButtonsChanged(void);
void 			Wait_For_Button_Press(void);

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
	void UART_prepare_for_recieve(void);
	void UART_Enable(void);
	void UART_Disable(void);
	void Set_Baudrate(uint32_t new_baud);
	
	//########################	ISO7816  										###############################
	void nrf_delay_us(uint32_t);
		
	void init_ISO7816_pins(void);
	void init_Card(void);
	
	//#pragma pack
	typedef struct {
		uint8_t Historical_Bytes:4;  //
		//uint8_t Bit_MAP					:4;  // 
		uint8_t Bit_MAP_TA				:1;  // 
		uint8_t Bit_MAP_TB				:1;  // 
		uint8_t Bit_MAP_TC				:1;  // 
		uint8_t Bit_MAP_TD				:1;  // 
	} __attribute__((packed)) T0;

	typedef struct {
		uint8_t D1 :4;  //
		uint8_t F1 :4;  // 
	} __attribute__((packed)) TA;

	typedef struct {
		uint8_t PI :5;  //
		uint8_t I1 :2;  // 
	} __attribute__((packed)) TB;

	typedef struct {
		uint8_t Protocol_Type :8;  //
	} __attribute__((packed)) TC;

	typedef struct {
		uint8_t Protocol_Type :4;  //
		//uint8_t Bit_MAP 			:4;  // 
		uint8_t Bit_MAP_TA				:1;  // 
		uint8_t Bit_MAP_TB				:1;  // 
		uint8_t Bit_MAP_TC				:1;  // 
		uint8_t Bit_MAP_TD				:1;  // 
	}__attribute__((packed)) TD;

	
