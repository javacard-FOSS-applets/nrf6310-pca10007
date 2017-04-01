#include "Universal.h"
#include <stdio.h>
#include <stdint.h>
#include "SEGGER_RTT.h"

void write_hex_value(uint8_t value) {
	static uint8_t previous_value;
	char str[20];
	int count;

	if( value != previous_value)
	{
		count = sprintf(str, "Value: 0x%02xh\n", value);
		if(count == 0 )
		{
			SEGGER_RTT_WriteString(0, "Couldnt write to string.\n");
		}
		else
		{
			SEGGER_RTT_WriteString(0, str);
		}
		previous_value = value;
	}
}

void write_one_hex_value(uint8_t value) {
	char str[20];
	int count;

	count = sprintf(str, "0x%02xh ", value);
	if(count == 0 )
	{
		SEGGER_RTT_WriteString(0, "Couldnt write to string.\n");
	}
	else
	{
		SEGGER_RTT_WriteString(0, str);
	}
}
