#include "Universal.h"

static uint8_t sent_value=0;
static uint8_t has_recieved_value=0;
static uint16_t wait_treshold=0;

void Test_Compare_Recieved_Value(uint8_t recieved_value) {
	has_recieved_value=true;
	wait_treshold=0;
	
	Segger_write_string("\t\t\tTEST COMPARE!!!!!!!!\n");
	
	if(recieved_value == sent_value) {
		sent_value++;
		dataready=1;
		
		if(sent_value>=TEST_MAX_VALUE) {
			GLobal_Test_Mode_Active=false;
			Wait_For_Button_Press(0);
		}
	}
	else {
		dataready=1;
	}
}

uint8_t Check_If_Recieved(void) {
	uint8_t return_value=false;
	
	Segger_write_string("\t\t\tCHECK RECIEVED!!!!!!!!\n");
	
	if(has_recieved_value==false) {
		wait_treshold++;
		return_value=false;
	}
	else {
		wait_treshold=0;
		return_value=true;
		dataready=1;
	}
		
	
	if(Global_Default_Security == MSG_HW_SYMM )		{
		
		Segger_write_string("\t\t\tSEC RIGHT\n");
		if( wait_treshold>(TEST_MAX_WAIT_TRESHOLD*5) ) {
			Segger_write_string_int("Return value", wait_treshold);
			return_value = true;
			dataready=1;
			wait_treshold=0;
		}
	}
	else{
		Segger_write_string("\t\t\tSEC ELSE\n");
		if( wait_treshold > TEST_MAX_WAIT_TRESHOLD ){
			return_value = true;
			dataready=1;
			wait_treshold=0;
		}
	}
	
	has_recieved_value=false;
	
	return return_value;
}

uint8_t Get_Actual_Test_Value(void) {
	Segger_write_string("\t\t\tGET VALUE!!!!!!!!\n");
//	dataready=1;
	return sent_value;
}
