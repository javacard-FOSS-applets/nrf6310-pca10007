#include <stdint.h>

#include "Universal.h"
#include "ISO7816.h"


uint8_t SC_ATR_Preffered_Protocol_Type=0x00;

uint8_t SC_ATR_Get_Protocol_Type() {
	 return SC_ATR_Preffered_Protocol_Type;
}

void SC_ATR_Set_Protocol_Type(uint8_t T_Type_Protocol) {
	SC_ATR_Preffered_Protocol_Type = T_Type_Protocol;
	Segger_write_string_value("D:   Protocol type set to:", SC_ATR_Preffered_Protocol_Type);
}


uint8_t SC_Get_Next_ATR_Content(T0 *t0) { 
	uint8_t skip=0;	
	
	if(t0->Bit_MAP_TA) {
		skip++;
	}
	if(t0->Bit_MAP_TB) {
		skip++;
	}
	if(t0->Bit_MAP_TC) {
		skip++;
	}
	if(t0->Bit_MAP_TD) {
		skip++;
	}
	
	return skip+1;
}

uint8_t SC_Analyze_ATR_Content(uint8_t T_identifier, T0 *t0) { 
	uint8_t skip=0;	

	// info about this T
	if(T_identifier==1) {
		Segger_write_string_value("\t Y=", ((TA*) t0)->F1);
	}
	if(T_identifier==2) {
		Segger_write_string_value("\t Y=", ((TA*) t0)->F1);
		
		SC_ATR_Set_Protocol_Type(t0->Historical_Bytes & 0x0f);
		Segger_write_string_value("\t Preferred protocol: ", SC_ATR_Preffered_Protocol_Type);
	}
	if(T_identifier==3) {
		Segger_write_string_value("\t Y=", ((TA*) t0)->F1);
		
		Segger_write_string_value("\t Preferred protocol: ", t0->Historical_Bytes & 0x0f);
	}
	
	
	if(t0->Bit_MAP_TA) {
		Segger_write_string("\tTA present\n");
		if(T_identifier==1) {
			
			uint8_t di = ((TA*) t0+1)->D1;
			uint8_t fi = ((TA*) t0+1)->F1;
			
			F=Clock_rate_conversion_factor_F[fi];
			D=Bit_rate_adjustment_factor_D[di];

			Segger_write_string_value("\t Di: ", di);
			Segger_write_string_value("\t Fi: ", fi);
			uint16_t value = Calc_Cycles_ETU(((TA*) t0+1)->D1, ((TA*) t0+1)->F1);
			ATR_ETU=value;
		}

		if(T_identifier==2) {
			Segger_write_string_value("\t TA2 PPS: ", ((TC*) t0+1)->Protocol_Type);
			PSS=0;
		}
		if(T_identifier>=3) {
			Segger_write_string_value("\t IFCS: ", ((TC*) t0+1)->Protocol_Type);
		}
		skip++;
	}
	if(t0->Bit_MAP_TB) {
		Segger_write_string("\tTB present\n");
		if(T_identifier==1) {
			Segger_write_string_value("\t VPP: ", ((TC*) t0+skip+1)->Protocol_Type);
		}
		if(T_identifier==3) {
			CWI=((TA*) t0+skip+1)->D1;
			BWI=((TA*) t0+skip+1)->F1;
			
			Segger_write_string_value("\t CWI: ", CWI);
			Segger_write_string_value("\t BWI: ", BWI);
			//uint16_t value = Calc_Cycles_ETU(((TA*) t0+2)->I1, ((TA*) t0+2)->T1);
		}
		skip++;
	}
	if(t0->Bit_MAP_TC) {
		Segger_write_string("\tTC present\n");
		if(T_identifier==1) {
			Segger_write_string_value("\t Extra Guard time: ", ((TC*) t0+skip+1)->Protocol_Type);
		}
		if(T_identifier==3) {
			Segger_write_string_value("\t LCR/CRC: ", ((TC*) t0+skip+1)->Protocol_Type);
			if(((TC*) t0+skip+1)->Protocol_Type & 0x01) {
				Segger_write_string("\t\t CRC used");
			}
			else {
				Segger_write_string("\t\t LRC used");
			}
		}
		skip++;
	}
	
	if(t0->Bit_MAP_TD) {
		Segger_write_string("\tTD present\n");
		skip++;
	}
	else {
		return 0;
	}
	
	return skip;
}

void SC_Recieve_ATR(void) {
	uint8_t success=0;
	Set_Default_Timing_Params();
	
	ATR_count=0;
	
	//UART_prepare_for_recieve();
	
	for(uint8_t i=0; i<ISO_7816_MAX_ATR_BYTES; i++) {
		//value=InverseByte(value);
		success=0;
		
		ATR_Message[i]=Recieve_UART_timeout(ISO_WAIT_TIME, &success);
		if(!success) {
			break;
		}
		
		Segger_write_one_hex_value(ATR_Message[i]);
		ATR_count++;
	}
	
	Segger_write_string("\n");
}


void SC_Analyze_ATR(void) {
	if(ATR_count==0) {
		return;
	}
	uint8_t TCount=1;
	Segger_write_string_value("\tHistorical Bytes:", ((T0*) (ATR_Message + 1))->Historical_Bytes);
	
	Segger_write_string_value("T0: ", ATR_Message[1]);
	
	uint8_t next_td = SC_Analyze_ATR_Content(TCount, (T0*)  (ATR_Message + 1 ));
	uint8_t now_td = 0;
	
	while(next_td != now_td) {
		now_td = next_td;
		TCount++;
		
		Segger_write_string_value("TD: ", ATR_Message[1+next_td]);
		next_td = next_td + SC_Analyze_ATR_Content(TCount, (T0*)  (ATR_Message + 1 + next_td ));
	}
	
	Segger_write_string("Historical bytes: \n\t");
	
	uint8_t  next_block = SC_Get_Next_ATR_Content((T0*)  (ATR_Message + 1 + next_td));
	
	for(uint8_t i=0; i<((T0*) (ATR_Message + 1))->Historical_Bytes; i++) {
		Segger_write_one_hex_value(ATR_Message[1+next_td+next_block+ i]);
	}
	
	Segger_write_string("\n\n");
	//Segger_write_string_value("TD1: ", ATR_Message[1]);
}

extern void SC_Recieve_ATR(void);
extern void SC_Check_Card(void);
extern void SC_Analyze_ATR(void);

void SC_Recieve_ATR_And_Config() {
	//uint8_t success=0;
	
	SC_Recieve_ATR();
	Is_Valid_Message(1, ATR_count, ATR_Message);
	SC_Check_Card();
	SC_Analyze_ATR();
	
	if(PSS && false) {
		Send_Negotiate_Block_Protocol_Alone();
		Recieve_And_Check_Response();
		//Recieve_UART_timeout(ISO_WAIT_TIME, &success);
		Correct_First_False_Byte(&SC_Response_Count, SC_Response);		
		Print_Touple_Send_Recieve();
		
		if(Validate_Valid_PPS_Response()) {
			Segger_write_string("Ack of PPS is Valid\n");
		}
		else {
			Segger_write_string("Ack of PPS is NOT valid\n");
		}
	}
	
	//Calc_Communication_Baudrate();
	//Calc_Timing
	//reconfigure_UART();
}

