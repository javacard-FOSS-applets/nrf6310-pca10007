#line 1 "EncodeDecode.c"
#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdbool.h"
 






 





#line 25 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdbool.h"



#line 2 "EncodeDecode.c"
#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
 
 





 









     
#line 27 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
     











#line 46 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"





 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     




typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;


     
typedef   signed     long long intmax_t;
typedef unsigned     long long uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     






     






     






     

     


     


     


     

     
#line 216 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     



     






     
    
 



#line 241 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     







     










     











#line 305 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"






 
#line 3 "EncodeDecode.c"
#line 1 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
 
 
 
 




 








 












#line 38 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"


  



    typedef unsigned int size_t;    
#line 54 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"




extern __declspec(__nothrow) void *memcpy(void * __restrict  ,
                    const void * __restrict  , size_t  ) __attribute__((__nonnull__(1,2)));
   




 
extern __declspec(__nothrow) void *memmove(void *  ,
                    const void *  , size_t  ) __attribute__((__nonnull__(1,2)));
   







 
extern __declspec(__nothrow) char *strcpy(char * __restrict  , const char * __restrict  ) __attribute__((__nonnull__(1,2)));
   




 
extern __declspec(__nothrow) char *strncpy(char * __restrict  , const char * __restrict  , size_t  ) __attribute__((__nonnull__(1,2)));
   





 

extern __declspec(__nothrow) char *strcat(char * __restrict  , const char * __restrict  ) __attribute__((__nonnull__(1,2)));
   




 
extern __declspec(__nothrow) char *strncat(char * __restrict  , const char * __restrict  , size_t  ) __attribute__((__nonnull__(1,2)));
   






 






 

extern __declspec(__nothrow) int memcmp(const void *  , const void *  , size_t  ) __attribute__((__nonnull__(1,2)));
   





 
extern __declspec(__nothrow) int strcmp(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   




 
extern __declspec(__nothrow) int strncmp(const char *  , const char *  , size_t  ) __attribute__((__nonnull__(1,2)));
   






 
extern __declspec(__nothrow) int strcasecmp(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   





 
extern __declspec(__nothrow) int strncasecmp(const char *  , const char *  , size_t  ) __attribute__((__nonnull__(1,2)));
   






 
extern __declspec(__nothrow) int strcoll(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   







 

extern __declspec(__nothrow) size_t strxfrm(char * __restrict  , const char * __restrict  , size_t  ) __attribute__((__nonnull__(2)));
   













 


#line 193 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
extern __declspec(__nothrow) void *memchr(const void *  , int  , size_t  ) __attribute__((__nonnull__(1)));

   





 

#line 209 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
extern __declspec(__nothrow) char *strchr(const char *  , int  ) __attribute__((__nonnull__(1)));

   




 

extern __declspec(__nothrow) size_t strcspn(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   




 

#line 232 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
extern __declspec(__nothrow) char *strpbrk(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));

   




 

#line 247 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
extern __declspec(__nothrow) char *strrchr(const char *  , int  ) __attribute__((__nonnull__(1)));

   





 

extern __declspec(__nothrow) size_t strspn(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));
   



 

#line 270 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"
extern __declspec(__nothrow) char *strstr(const char *  , const char *  ) __attribute__((__nonnull__(1,2)));

   





 

extern __declspec(__nothrow) char *strtok(char * __restrict  , const char * __restrict  ) __attribute__((__nonnull__(2)));
extern __declspec(__nothrow) char *_strtok_r(char *  , const char *  , char **  ) __attribute__((__nonnull__(2,3)));

extern __declspec(__nothrow) char *strtok_r(char *  , const char *  , char **  ) __attribute__((__nonnull__(2,3)));

   

































 

extern __declspec(__nothrow) void *memset(void *  , int  , size_t  ) __attribute__((__nonnull__(1)));
   



 
extern __declspec(__nothrow) char *strerror(int  );
   





 
extern __declspec(__nothrow) size_t strlen(const char *  ) __attribute__((__nonnull__(1)));
   



 

extern __declspec(__nothrow) size_t strlcpy(char *  , const char *  , size_t  ) __attribute__((__nonnull__(1,2)));
   
















 

extern __declspec(__nothrow) size_t strlcat(char *  , const char *  , size_t  ) __attribute__((__nonnull__(1,2)));
   






















 

extern __declspec(__nothrow) void _membitcpybl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitcpybb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitcpyhl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitcpyhb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitcpywl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitcpywb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovebl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovebb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovehl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovehb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovewl(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
extern __declspec(__nothrow) void _membitmovewb(void *  , const void *  , int  , int  , size_t  ) __attribute__((__nonnull__(1,2)));
    














































 







#line 502 "C:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\string.h"



 

#line 4 "EncodeDecode.c"

#line 1 "Universal.h"
#line 1 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"












































 




#line 1 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT_Conf.h"
















 




#line 1 ".\\Include\\compiler_abstraction.h"










 




 






#line 46 ".\\Include\\compiler_abstraction.h"

 

#line 23 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT_Conf.h"










 




















 
#line 97 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT_Conf.h"




 
#line 123 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT_Conf.h"




 









 
#line 51 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"






 






 





typedef struct {
  const     char*    sName;         
            char*    pBuffer;       
            unsigned SizeOfBuffer;  
  volatile  unsigned WrOff;         
  volatile  unsigned RdOff;         
            unsigned Flags;         
} SEGGER_RTT_RING_BUFFER;






typedef struct {
  char                    acID[16];                                 
  int                     MaxNumUpBuffers;                          
  int                     MaxNumDownBuffers;                        
  SEGGER_RTT_RING_BUFFER  aUp[(2)];       
  SEGGER_RTT_RING_BUFFER  aDown[(2)];   
} SEGGER_RTT_CB;






 
extern SEGGER_RTT_CB _SEGGER_RTT;






 
int          SEGGER_RTT_ConfigUpBuffer   (unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int          SEGGER_RTT_ConfigDownBuffer (unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int          SEGGER_RTT_GetKey           (void);
unsigned     SEGGER_RTT_HasData          (unsigned BufferIndex);
int          SEGGER_RTT_HasKey           (void);
void         SEGGER_RTT_Init             (void);
unsigned     SEGGER_RTT_Read             (unsigned BufferIndex,       void* pBuffer, unsigned BufferSize);
unsigned     SEGGER_RTT_ReadNoLock       (unsigned BufferIndex,       void* pData,   unsigned BufferSize);
int          SEGGER_RTT_SetNameDownBuffer(unsigned BufferIndex, const char* sName);
int          SEGGER_RTT_SetNameUpBuffer  (unsigned BufferIndex, const char* sName);
int          SEGGER_RTT_WaitKey          (void);
unsigned     SEGGER_RTT_Write            (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned     SEGGER_RTT_WriteNoLock      (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned     SEGGER_RTT_WriteSkipNoLock  (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned     SEGGER_RTT_WriteString      (unsigned BufferIndex, const char* s);










 
int     SEGGER_RTT_SetTerminal        (char TerminalId);
int     SEGGER_RTT_TerminalOut        (char TerminalId, const char* s);






 
int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);






 
















#line 173 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"

#line 182 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"

#line 191 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"

#line 200 "C:\\Nordic Semiconductor\\external\\segger_rtt\\SEGGER_RTT.h"




 
#line 2 "Universal.h"

typedef enum {MSG_EMPTY=0x00,
							MSG_UNSECURED=0x01,
							MSG_SW_SYMM=0x02, MSG_SW_ASYMM=0x03,
							MSG_HW_SYMM=0x04, MSG_HW_ASYMM=0x05} security_type;

							

typedef struct {
	uint16_t length;
	uint8_t count;
	uint8_t message[64];
	uint8_t ready;
} MessageBuffer;











void 			init(void);

void 			SetLEDS(uint8_t);
void 			BlinkLEDS(uint8_t);
uint8_t 	ReadButtons(void);

void 			write_hex_value(uint8_t value);
void			write_one_hex_value(uint8_t value);

uint8_t 	AddMessage(uint8_t*);

void		 	SendData(uint8_t*);
void 			FillSendData(uint16_t, uint8_t*);


void Decode(uint16_t, uint8_t*);
void EnCode(security_type, uint8_t);


extern 		MessageBuffer transmit;
extern 		MessageBuffer recieve;
extern 		uint8_t dataready;

extern 		uint8_t recieved_value;



#line 6 "EncodeDecode.c"

void Decode(uint16_t count, uint8_t* message) {
	SEGGER_RTT_WriteString(0, "Decode: ");
	write_one_hex_value(count);
	if(count==0){
		
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
				
				
			}
			break;
		
		case MSG_SW_SYMM:
			
			
			
			break;
		case MSG_SW_ASYMM:
			
			
			
			break;
		
		case MSG_HW_SYMM:
			
			
			
			break;
		case MSG_HW_ASYMM:
			
			
			
			break;
		default:
			
			break;
	}
}

void EnCode(security_type security, uint8_t value) {
	uint8_t send_message_uns[1+1];


	
	switch(security){
		case MSG_UNSECURED:
				send_message_uns[0]=(uint8_t) security;
				send_message_uns[1]= value;
		
				
		
				if(transmit.ready==1) {
					FillSendData(2, send_message_uns);
				}
			break;
		
		case MSG_SW_SYMM:
			
			
			break;
		case MSG_SW_ASYMM:
			
			
			break;
		
		case MSG_HW_SYMM:
			
			
			break;
		case MSG_HW_ASYMM:
			
			
			break;
		
		default:
			
			break;
	}
}
