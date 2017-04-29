#include <stdint.h>

#define CLA_ISO7816 	0x00
#define CLA_GP_PROP 	0x80
#define CLA_GP_SECU 	0x84

#define INTERNAL_CLK 	372
#define RFU 					0x01

static uint16_t Clock_rate_conversion_factor_F[16] = { INTERNAL_CLK, 372, 558, 744,  1116, 1488, 1860, RFU,
																									     RFU,          512, 768, 1024, 1536, 2048, RFU,  RFU };
//512
static uint8_t Bit_rate_adjustment_factor_D[16] = { RFU, 1,   2, 4, 8, 16, 32, 64,
								    																 //RFU, RFU, 2, 4, 8, 16, 32,  64 };
																									12, 20, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64 };
static uint8_t Programing_Current_Factor_I[4] = {25, 50, 100, RFU };


//http://java.inf.elte.hu/java-1.2/javacard/iso7816.txt

/*http://techmeonline.com/most-used-smart-card-commands-apdu/
INS Value
		Command Description
0E 	Erase Binary
		20 	Verify
70 	Manage Channel
82 	External Authenticate
84 	Get Challenge
88 	Internal Authenticate
		A4 	Select File
B0 	Read Binary
B2 	Read Record(s)
C0 	Get Response
C2 	Envelope
CA 	Get Data
D0 	Write Binary
D2 	Write Record
D6 	Update Binary
DA 	Put Data
DC 	Update Record
E2 	Append Record*/

#define P_CLA 0
#define P_INS 1
#define P_P1 	2
#define P_P2 	3
#define P_Lc 	4
#define P_Dat	5

#define INS_GET_DATA 	0xCA
	#define	P2_DATA_ISSUER_NUMBER 0x42
	#define P2_DATA_CARD_IMAGE		0x45
	#define P2_DATA_CARD_DATA			0x66
	#define P2_DATA_CONFIRMATION_COUNT			0xC2
	#define P2_DATA_KEY_TEMPLATE	0xE0
	#define P2_DATA_SEQ_COUNTER		0xE0

#define INS_READ_RECORD		0xB2
#define INS_GET_RESPONSE 	0xC0
#define INS_GET_STAT 	0xF2
#define INS_DELETE	 	0xEA

#define INS_INSTALL	 	0xE6
#define INS_LOAD		 	0xE8
#define INS_MAN_CHAN 	0x70
#define INS_PUT_KEY	 	0xD8
#define INS_SELECT	 	0xA4
#define INS_SET_STAT	0xF0
#define INS_STR_DATA	0xE2
/*
b8=1 indicates that the Application is a Security Domain.
b7=1 indicates that the Security Domain has DAP Verification capability.
b6=1 indicates that the Security Domain has Delegated Management privileges.
b5=1 indicates that the Application has the privilege to lock the card.
b4=1 indicates that the Application has the privilege to terminate the card.
b3=1 indicates that the Application has the Default Selected privilege.
b2=1 indicates that the Application has CVM management privileges.
b1=1 indicates that the Security Domain has mandated DAP Verification capability.
*/

#define DELAY_ETU_CYCLES 45000
#define one_CLK_cycle 	 6

#define APDU
#define TEST 						 0

uint16_t Calc_Default_Cycles_ETU(void);
uint16_t Calc_Cycles_ETU(uint8_t di, uint8_t fi);

uint8_t Is_Card_Active(void);
void Card_Activate(void);
void Card_Deactivate(void);
void Card_Cold_Reset(void);
void Warm_Reset(void);

void Try_Locating_Card_Manager(void);
void Try_Locating_Card_Manager_Brute(void);
void Try_Locating_Classes(void);
void Try_Locating_Instructions(void);

void Set_Default_Timing_Params(void);

void Try_Get_Status(void);
void Try_DATA(void);
void Try_STATUS(void);
void Try_RECORDS(void);
void Try_Card_Production_Life_Cycle_Data(void);

void Send_Negotiate_Block_Protocol_Alone(void);
void Send_Negotiate_Block_Protocol_APDU(void);
void Send_Negotiate_Block_Protocol_Block(void);
void Send_Test_Block_Frame(uint8_t Length, uint8_t* Payload);

void Card_wait(void);
void Card_wait_ETU_cycles(uint32_t Cycles);
void Copy_Mem(uint8_t Lenght, uint8_t * from, uint8_t * to);

void Test_Error_Database(void);
void Locate_Error(uint8_t SW1, uint8_t SW2);
void test_Card(void);

uint16_t Concat_Bytes(uint8_t byte1, uint8_t byte2);

void		Analyze_Message(uint8_t Lenght, uint8_t * Message);
uint8_t Is_Valid_Message(uint8_t offset, uint8_t Lenght, uint8_t * Message);

uint8_t Prepare_Standard_APDU(uint8_t Lenght, uint8_t * Payload);
uint8_t Prepare_Standard_Block(uint8_t Lenght, uint8_t * Payload);
uint8_t	Prepare_Standard_APDU_Block(uint8_t Lenght, uint8_t * Payload);


void 		SC_Send_Message(uint8_t Lenght);
uint8_t Recieve_Response(void);
uint8_t Recieve_And_Check_Response(void);
uint8_t Send_Message_Recieve_Response(uint8_t * Payload, uint8_t send_count, uint8_t * Message_Recieved);
uint8_t Send_And_Recieve(uint8_t count);

uint8_t SC_Analyze_ATR_Content(uint8_t T_identifier, T0 *t0);
uint8_t SC_Get_Next_ATR_Content(T0 *t0);
uint8_t SC_ATR_Get_Protocol_Type(void);
void 		SC_ATR_Set_Protocol_Type(uint8_t T_Type_Protocol);

uint8_t Calc_XOR_Checksum(uint8_t init_value, uint8_t offset, uint8_t lenght, uint8_t * message);
uint8_t Send_Message_Recieve_Response(uint8_t * Message_Send, uint8_t send_count, uint8_t * Message_Recieved);

/*void SC_Check_Card(void);
void SC_Analyze_ATR(void);
void SC_Recieve_ATR(void);*/
void SC_Recieve_ATR_And_Config(void);

void Get_Response(uint8_t count);
void Correct_First_False_Byte(uint8_t* count, uint8_t *Message);

void Print_Touple_Send_Recieve(void);


extern uint8_t ATR_Message[];
extern uint8_t ATR_count;

extern uint8_t SC_Response[];
extern uint8_t SC_Response_Count;

extern uint8_t SC_APDU[];
extern uint8_t SC_Send_Count;

extern uint8_t SC_Temp_Buffer[];
extern uint8_t SC_Header[];

extern uint16_t F;
extern uint16_t D;

extern uint16_t ATR_ETU;
extern uint16_t ETU;
extern uint8_t IFCS;
extern uint8_t CWI;
extern uint8_t BWI;
extern uint8_t WI;
extern uint8_t PSS;
extern float   work_ETU;

/*
	tlac
	t=0
	t=1

	zopar metod
	test sim karty
*/

// Books
// 	https://www.globalplatform.org/specificationscard.asp
//	http://www.win.tue.nl/pinpasjc/docs/GPCardSpec_v2.2.pdf
//	http://www.ttfn.net/techno/smartcards/iso7816_4.html

//todo Longer Messages than bytes

// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0095a/ch03s02s07.html
//http://www.ruimtools.com/atr.php
//http://ludovic.rousseau.free.fr/softwares/pcsc-tools/smartcard_list.txt

//https://smartcard-atr.appspot.com/parse?ATR=3bea00008131fe450031c173c840000090007a
	//p5cn145
		//0x3bh 0xeah 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x00h 0x31h 0xc1h 0x73h 0xc8h 0x40h 0x00h 0x00h 0x90h 0x00h 0x7ah
		//3B EA 00 00 81 31 FE 45 00 31 C1 73 C8 40 00 00 90 00 7A

//https://smartcard-atr.appspot.com/parse?ATR=3bf81300008131fe454a434f5076323431b7
	//p5cn080
		//0x3bh 0xf8h 0x13h 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x4ah 0x43h 0x4fh 0x50h 0x76h 0x32h 0x34h 0x31h 0xb7h
		//3B F8 13 00 00 81 31 FR 45 4A 43 4F 50 76 32 34 31 B7

//https://smartcard-atr.appspot.com/parse?ATR=3b9e96801fc78031e073fe211b66d0017b100f0065
	//t-mobile sim card
		//0x3bh 0x9eh 0x96h 0x80h 0x1fh 0xc7h 0x80h 0x31h 0xe0h 0x73h 0xfeh 0x21h 0x1bh 0x66h 0xd0h 0x01h 0x7bh 0x10h 0x0fh 0x00h 0x65h
		//3B 9E 96 80 1F C7 80 31 E0 73 FE 21 1B 66 D0 01 7B 10 0F 00 65

//0x3bh 0xf8h 0x13h 0x00h 0x00h 0x81h 0x31h 0xfeh 0x45h 0x4ch 0x47h 0x4dh 0x43h 0x61h 0x72h 0x64h 0x31h 0xa3h
	//3b f8 13 00 00 81 31 fe 45 4c 47 4d 43 61 72 64h 31 a3
	
//apdu error list
	//https://www.eftlab.co.uk/index.php/site-map/knowledge-base/118-apdu-response-list
	//www.smartjac.biz/index.php/support/main-menu?view=kb&kbartid=3&tmpl=component&print=1

// CLA INS
	//http://techmeonline.com/most-used-smart-card-commands-apdu/
	//http://www.cardwerk.com/smartcards/smartcard_standard_iso7816-4_5_basic_organizations.aspx

//http://stackoverflow.com/questions/27600597/cant-select-aid-card-manager-when-testing-to-send-apdu
	//Locating Card Manager
	//https://www.eftlab.com.au/index.php/site-map/knowledge-base/212-emv-rid
	
//Loading APP
	//https://adywicaksono.wordpress.com/2008/01/05/installing-javacard-applet-into-real-smartcard/
	

//http://stackoverflow.com/questions/27600597/cant-select-aid-card-manager-when-testing-to-send-apdu

/*uint8_t InverseByte(uint8_t byte) {
	uint8_t value = 0;

	value  = (byte & 0x01) << 7;
	value |= (byte & 0x02) << 5;
	value |= (byte & 0x04) << 3;
	value |= (byte & 0x08) << 1;

	value |= (byte & 0x10) >> 1;
	value |= (byte & 0x20) >> 3;
	value |= (byte & 0x40) >> 5;
	value |= (byte & 0x80) >> 7;

	return value;
}*/

//5
//0 1 2  3  4
//0 0 90 00 90

/* //verify CRC
//calculate CRC
// add header
// 00
// 80
// 01
// 02
// length
// message
// CRC*/

// RFU reserved for future use
// ICC integrated circuit card
// IFD

/*CLA INS P1 P2    L Data LRC
class
	instruction
		instruction parameter 1, 2

http://www.oracle.com/technetwork/java/javacard/javacard1-139251.html*/

/*Table 1. ISO 7816 CLA Values
CLA Value 	Instruction Class
0x0n, 0x1n 	ISO 7816-4 card instructions, such as for file access and security operations
20 to 0x7F 	Reserved
0x8n or 0x9n 	ISO/IEC 7816-4 format you can use for your application-specific instructions, interpreting 'X' according to the standard
0xAn 	Application- or vendor-specific instructions
B0 to CF 	ISO/IEC 7816-4 format you can use for application-specific instructions
D0 to FE 	Application- or vendor-specific instructions
FF 	Reserved for protocol type selection*/


/*
P5Cn080
0> -->		0x00h 0x00h 0x05h 0x00h 0xa4h 0x04h 0x00h 0x00h 0xa5h 
0> 	<--		0x00h 0x00h 0x00h 0x69h 0x6fh 0x65h 0x84h 0x08h 0xa0h 0x00h 0x00h 0x00h 0x03h 0x00h 0x00h 0x00h 0xa5h 0x59h 0x9fh 0x65h 0x01h 0xffh 0x9fh 0x6eh 0x06h 0x47h 0x91h 0x00h 0x78h 0x33h 0x00h 0x73h 0x4ah 0x06h 0x07h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x01h 0x60h 0x0ch 0x06h 0x0ah 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x02h 0x02h 0x01h 0x01h 0x63h 0x09h 0x06h 0x07h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x03h 0x64h 0x0bh 0x06h 0x09h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x04h 0x02h 0x15h 0x65h 0x0bh 0x06h 0x09h 0x2bh 0x85h 0x10h 0x86h 0x48h 0x64h 0x02h 0x01h 0x03h 0x66h 0x0ch 0x06h 0x0ah 0x2bh 0x06h 0x01h 0x04h 0x01h 0x2ah 0x02h 0x6eh 0x01h 0x02h 0x90h 0x00h 0x73h 

P5Cn145
0> -->		0x00h 0x00h 0x05h 0x00h 0xa4h 0x04h 0x00h 0x00h 0xa5h 
0> 	<--		0x00h 0x00h 0x00h 0x69h 0x6fh 0x65h 0x84h 0x08h 0xa0h 0x00h 0x00h 0x00h 0x03h 0x00h 0x00h 0x00h 0xa5h 0x59h 0x9fh 0x65h 0x01h 0xffh 0x9fh 0x6eh 0x06h 0x47h 0x91h 0x20h 0x17h 0x3fh 0x00h 0x73h 0x4ah 0x06h 0x07h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x01h 0x60h 0x0ch 0x06h 0x0ah 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x02h 0x02h 0x01h 0x01h 0x63h 0x09h 0x06h 0x07h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x03h 0x64h 0x0bh 0x06h 0x09h 0x2ah 0x86h 0x48h 0x86h 0xfch 0x6bh 0x04h 0x02h 0x15h 0x65h 0x0bh 0x06h 0x09h 0x2bh 0x85h 0x10h 0x86h 0x48h 0x64h 0x02h 0x01h 0x03h 0x66h 0x0ch 0x06h 0x0ah 0x2bh 0x06h 0x01h 0x04h 0x01h 0x2ah 0x02h 0x6eh 0x01h 0x02h 0x90h 0x00h 0x30h 

*/
