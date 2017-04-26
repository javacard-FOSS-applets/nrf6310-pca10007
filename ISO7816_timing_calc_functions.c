#include "Universal.h"

#include "iso7816.h"

uint16_t F=0;
uint16_t D=0;

uint16_t ETU =16;
uint8_t  IFCS=32;
uint8_t  CWI =13;
uint8_t  BWI =4 ;
uint8_t  WI  =10;

float work_ETU;
float WT;
float CWT;
float BWT;

//Default values
//F = 372 ;   D = 1 ;  I = 50 ;  P = 5 ;  N = 0
#define INT_CLK 0

uint16_t Calc_Cycles_ETU(uint8_t di, uint8_t fi) {
	uint16_t ETU;
	
	if(fi==INT_CLK) {
		//work etu = (1/D)*(1/9600) [s]
		return 0;
	}
	else {
		//work etu = (1/D)*(F/fs) s		
		//value= ( 1/Bit_rate_adjustment_factor_D[di] ) * (Clock_rate_conversion_factor_F[fi]/ ISO7816_CLK );
	
		ETU= ( Clock_rate_conversion_factor_F[fi] ) / (Bit_rate_adjustment_factor_D[di] );
		Segger_write_string_int("\t Clock_rate_conversion_factor_F:\t", Clock_rate_conversion_factor_F[fi]);
		Segger_write_string_int("\t Bit_rate_adjustment_factor_D:\t", Bit_rate_adjustment_factor_D[di]);
		
		Segger_write_string_int("\t Calculated ETU divisor for clock:\t", ETU);
		
		Segger_write_string_int("\t Calculated Baudrate:\t\t\t", ISO7816_CLK/ETU);
		return ETU;
	}
	
	//baudrate=clock_freq(should fs)/etu
}

uint16_t Calc_Default_Cycles_ETU(void) {
	return Calc_Cycles_ETU(1, 1);
}

float Calc_WT_Wait_Time() {
	return (float)((float)WI*(float)960*(float)(F/(float)ISO7816_CLK) * work_ETU);
}
float Calc_CWT_Character_Wait_Time() {
	return (float)((float)((11 + (float)(pow((float)2, (int) CWI))) * (float) work_ETU));
}
float Calc_BWT_Block_Wait_Time() {
	return (float)( 11 * work_ETU + (float)pow((float)2, (int) BWI) * 960 * F/ISO7816_CLK);
}
float Calc_Work_ETU() {
	return (float)((float)F*(float)(D / (float) ISO7816_CLK));
	return SYSCLK/ETU;
}

void Set_Default() {
	F=   372;
	D=   1;
	ETU =16;
	IFCS=32;
	CWI =13;
	BWI =4 ;
	WI  =10;
}

void Calc_ALL_Times() {
	work_ETU = Calc_Work_ETU();
	WT = Calc_WT_Wait_Time();
	CWT = Calc_CWT_Character_Wait_Time();
	BWT = Calc_BWT_Block_Wait_Time();
}
