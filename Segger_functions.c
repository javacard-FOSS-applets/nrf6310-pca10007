#include "Universal.h"
#include <stdio.h>
#include <stdint.h>
#include "SEGGER_RTT.h"

void Segger_write_hex_value(uint8_t value) {
	#ifdef DEBUG
	static uint8_t previous_value;
	char str[20];
	int count;

	if( value != previous_value) {
		count = sprintf(str, "Value: 0x%02xh\n", value);
		if(count == 0 ) {
			SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, "Couldnt write to string.\n");
		}
		else {
			SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, str);
		}
		previous_value = value;
	}
	#endif
}

void Segger_write_one_hex_value(uint8_t value) {
	#ifdef DEBUG
	char str[20];
	int count;

	count = sprintf(str, "0x%02xh ", value);
	if(count == 0 ) {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, "Couldnt write to string.\n");
	}
	else {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, str);
	}
	#endif
}

void Segger_write_string_int(const char* message, uint32_t value) {
	#ifdef DEBUG
	Segger_write_string(message);
	
	char str[20];
	int count;

	count = sprintf(str, "%d ", value);
	if(count == 0 ) {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, "Couldnt write to string.\n");
	}
	else {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, str);
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, "\n");
	}
		
	#endif
}

void Segger_write_one_hex_value_32(uint32_t value) {
	#ifdef DEBUG
	char str[20];
	int count;

	count = sprintf(str, "0x%08xh ", value);
	if(count == 0 ) {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, "Couldnt write to string.\n");
	}
	else {
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, str);
	}
	#endif
}

void Segger_write_string(const char *string) {
	#ifdef DEBUG
		SEGGER_RTT_WriteString(DEFAULT_SEGGER_JLINK_RTT_VIEWER_CONSOLE, string);
	#endif
}

void Segger_write_string_value(const char *string, uint8_t value) {
	#ifdef DEBUG
		Segger_write_string(string);
		Segger_write_one_hex_value(value);
		Segger_write_string("\n");
	#endif
}
