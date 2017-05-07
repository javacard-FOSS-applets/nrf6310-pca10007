#include "stdint.h"

static struct spec_err {
	uint16_t sw;
	char *message;
} SPECIFC_ERRORS[] = 
{
#ifdef COMPILE_HW_ERRORS	
	#ifndef DEBUG_SAVE_SPACE
		{0x0600, "Class not supported."}, 
		{0x6200, "Application has been logically deleted"}, 
		{0x6200, "No information given (NV-Ram not changed)"}, 
		{0x6201, "NV-Ram not changed 1."}, 
		{0x6281, "Part of returned data may be corrupted"}, 
		{0x6282, "End of file/record reached before reading Le bytes"}, 
		{0x6283, "Selected file invalidated"}, 
		{0x6284, "Selected file is not valid. FCI not formated according to ISO"}, 
		{0x6285, "No input data available from a sensor on the card. No Purse Engine enslaved for R3bc"}, 
		{0x62A2, "Wrong R-MAC"}, 
		{0x62A4, "Card locked (during reset( ))"}, 
		{0x62F1, "Wrong C-MAC"}, 
		{0x62F3, "Internal reset"}, 
		{0x62F5, "Default agent locked"}, 
		{0x62F7, "Cardholder locked"}, 
		{0x62F8, "Basement is current agent"}, 
		{0x62F9, "CALC Key Set not unblocked"}, 
		{0x6310, "More data available"}, 
		{0x6300, "No information given (NV-Ram changed)"}, 
		{0x6381, "File filled up by the last write. Loading/updating is not allowed."}, 
		{0x6382, "Card key not supported."}, 
		{0x6383, "Reader key not supported."}, 
		{0x6384, "Plaintext transmission not supported."}, 
		{0x6385, "Secured transmission not supported."}, 
		{0x6386, "Volatile memory is not available."}, 
		{0x6387, "Non-volatile memory is not available."}, 
		{0x6388, "Key number not valid."}, 
		{0x6389, "Key length is not correct."}, 
		{0x63C0, "Verify fail, no try left."}, 
		{0x63C1, "Verify fail, 1 try left."}, 
		{0x63C2, "Verify fail, 2 tries left."}, 
		{0x63C3, "Verify fail, 3 tries left."}, 
		{0x63F1, "More data expected."}, 
		{0x63F2, "More data expected and proactive command pending."}, 
		{0x6400, "No specific diagnosis"}, 
		{0x6400, "No information given (NV-Ram not changed)"}, 
		{0x6401, "Command timeout. Immediate response required by the card."}, 
		{0x6581, "Memory Failure"}, 
		{0x6581, "Memory failure"}, 
		{0x6581, "Memory failure"}, 
		{0x6500, "No information given"}, 
		{0x6501, "Write error. Memory failure. There have been problems in writing or reading the EEPROM. Other hardware problems may also bring this error."}, 
		{0x6581, "Memory failure"}, 
		{0x6600, "Error while receiving (timeout)"}, 
		{0x6601, "Error while receiving (character parity error)"}, 
		{0x6602, "Wrong checksum"}, 
		{0x6603, "The current DF file without FCI"}, 
		{0x6604, "No SF or KF under the current DF"}, 
		{0x6669, "Incorrect Encryption/Decryption Padding"}, 
		{0x6700, "Wrong length in Lc"}, 
		{0x6700, "Wrong length"}, 
		{0x6800, "No information given (The request function is not supported by the card)"}, 
		{0x6881, "Logical channel not supported"}, 
		{0x6882, "Secure messaging not supported"}, 
		{0x6883, "Last command of the chain expected"}, 
		{0x6884, "Command chaining not supported"}, 
		{0x6982, "Security status not satisfied"}, 
		{0x6985, "Conditions of use not satisfied"}, 
		{0x6985, "Referenced data cannot be deleted"}, 
		{0x6985, "Conditions of use not satisfied "}, 
		{0x6900, "No information given (Command not allowed)"}, 
		{0x6901, "Command not accepted (inactive state)"}, 
		{0x6981, "Command incompatible with file structure"}, 
		{0x6982, "Security condition not satisfied."}, 
		{0x6983, "Authentication method blocked"}, 
		{0x6984, "Referenced data reversibly blocked (invalidated)"}, 
		{0x6985, "Conditions of use not satisfied."}, 
		{0x6986, "Command not allowed (no current EF)"}, 
		{0x6987, "Expected secure messaging (SM) object missing"}, 
		{0x6988, "Incorrect secure messaging (SM) data object"}, 
		{0x698D, "Reserved"}, 
		{0x6996, "Data must be updated again"}, 
		{0x69E1, "POL1 of the currently Enabled Profile prevents this action."}, 
		{0x69F0, "Permission Denied"}, 
		{0x69F1, "Permission Denied - Missing Privilege"}, 
		
		//{0x9200, "Exponent e is too long"}, 
			
		{0x9004, "PIN not succesfully verified, 3 or more PIN tries left"}, 
		{0x9008, "Key/file not found"}, 
		{0x9080, "Unblock Try Counter has reached zero"}, 
		{0x9100, "OK"}, 
		{0x9101, "States.activity, States.lock Status or States.lockable has wrong value"}, 
		{0x9102, "Transaction number reached its limit"}, 
		{0x910C, "No changes"}, 
		{0x910E, "Insufficient NV-Memory to complete command"}, 
		{0x911C, "Command code not supported"}, 
		{0x911E, "CRC or MAC does not match data"}, 
		{0x9140, "Invalid key number specified"}, 
		{0x917E, "Length of command string invalid"}, 
		{0x919D, "Not allow the requested command"}, 
		{0x919E, "Value of the parameter invalid"}, 
		{0x91A0, "Requested AID not present on PICC"}, 
		{0x91A1, "Unrecoverable error within application"}, 
		{0x91AE, "Authentication status does not allow the requested command"}, 
		{0x91AF, "Additional data frame is expected to be sent"}, 
		{0x91BE, "Out of boundary"}, 
		{0x91C1, "Unrecoverable error within PICC"}, 
		{0x91CA, "Previous Command was not fully completed"}, 
		{0x91CD, "PICC was disabled by an unrecoverable error"}, 
		{0x91CE, "Number of Applications limited to 28"}, 
		{0x91DE, "File or application already exists"}, 
		{0x91EE, "Could not complete NV-write operation due to loss of power"}, 
		{0x91F0, "Specified file number does not exist"}, 
		{0x91F1, "Unrecoverable error within file"}, 
		{0x9210, "Insufficient memory. No more storage available."}, 
		{0x9240, "Writing to EEPROM not successful."}, 
		{0x9301, "Integrity error"}, 
		{0x9302, "Candidate S2 invalid"}, 
		{0x9303, "Application is permanently locked"}, 
		{0x9400, "No EF selected."}, 
		{0x9401, "Candidate currency code does not match purse currency"}, 
		{0x9402, "Candidate amount too high"}, 
		{0x9402, "Address range exceeded."}, 
		{0x9403, "Candidate amount too low"}, 
		{0x9404, "FID not found, record not found or comparison pattern not found."}, 
		{0x9405, "Problems in the data field"}, 
		{0x9406, "Required MAC unavailable"}, 
		{0x9407, "Bad currency : purse engine has no slot with R3bc currency"}, 
		{0x9408, "R3bc currency not supported in purse engine"}, 
		{0x9408, "Selected file type does not match command."}, 
		{0x9580, "Bad sequence"}, 
		{0x9681, "Slave not found"}, 
		{0x9700, "PIN blocked and Unblock Try Counter is 1 or 2"}, 
		{0x9702, "Main keys are blocked"}, 
		{0x9704, "PIN not succesfully verified, 3 or more PIN tries left"}, 
		{0x9784, "Base key"}, 
		{0x9785, "Limit exceeded - C-MAC key"}, 
		{0x9786, "SM error - Limit exceeded - R-MAC key"}, 
		{0x9787, "Limit exceeded - sequence counter"}, 
		{0x9788, "Limit exceeded - R-MAC length"}, 
		{0x9789, "Service not available"}, 
		{0x9802, "No PIN defined."}, 
		{0x9804, "Access conditions not satisfied, authentication failed."}, 
		{0x9835, "ASK RANDOM or GIVE RANDOM not executed."}, 
		{0x9840, "PIN verification not successful."}, 
		{0x9850, "INCREASE or DECREASE could not be executed because a limit has been reached."}, 
		{0x9862, "Authentication Error, application specific (incorrect MAC)"}, 
		{0x9900, "1 PIN try left"}, 
		{0x9904, "PIN not succesfully verified, 1 PIN try left"}, 
		{0x9985, "Wrong status - Cardholder lock"}, 
		{0x9986, "Missing privilege"}, 
		{0x9987, "PIN is not installed"}, 
		{0x9988, "Wrong status - R-MAC state"}, 
		{0x6a86, "Incorrect P1 P2"}, 
		{0x6a80, "Incorrect parameters in data field"}, 
		{0x6a84, "Not enough memory space"}, 
		{0x6a88, "Referenced data not found"}, 
		{0x6a88, "Referenced data not found"}, 
		{0x6a82, "Application not found"}, 
		{0x6a80, "Incorrect values in command data"}, 
		{0x6a88, "Referenced data not found"}, 
		{0x6a88, "Referenced data not found"}, 
		{0x6a80, "Incorrect values in command data"}, 
		{0x6a84, "Not enough memory space"}, 
		{0x6a86, "Incorrect P1/P2"}, 
		{0x6A00, "No information given (Bytes P1 and/or P2 are incorrect)"}, 
		{0x6A80, "The parameters in the data field are incorrect."}, 
		{0x6A81, "Function not supported"}, 
		{0x6A82, "File not found"}, 
		{0x6A83, "Record not found"}, 
		{0x6A84, "There is insufficient memory space in record or file"}, 
		{0x6A85, "Lc inconsistent with TLV structure"}, 
		{0x6A86, "Incorrect P1 or P2 parameter."}, 
		{0x6A87, "Lc inconsistent with P1-P2"}, 
		{0x6A88, "Referenced data not found"}, 
		{0x6A89, "File already exists"}, 
		{0x6A8A, "DF name already exists."}, 
		{0x6AF0, "Wrong parameter value"}, 
		{0x6B00, "Wrong parameter(s) P1-P2"}, 
		{0x6C00, "Incorrect P3 length."}, 
		{0x6d00, "Invalid instruction"}, 
		{0x6D00, "Instruction code not supported or invalid"}, 
		{0x6e00, "Invalid class"}, 
		{0x6E00, "Class not supported"}, 
		{0x6F00, "Command aborted - more exact diagnosis not possible (e.g., operating system error)."}, 
		{0x6FFF, "Card dead (overuse, �)"}, 
		{0x9A00, "2 PIN try left"}, 
		{0x9A04, "PIN not succesfully verified, 2 PIN try left"}, 
		{0x9A71, "Wrong parameter value - Double agent AID"}, 
		{0x9A72, "Wrong parameter value - Double agent Type"}, 
		{0x9D05, "Incorrect certificate type"}, 
		{0x9D07, "Incorrect session data size"}, 
		{0x9D08, "Incorrect DIR file record size"}, 
		{0x9D09, "Incorrect FCI record size"}, 
		{0x9D0A, "Incorrect code size"}, 
		{0x9D10, "Insufficient memory to load application"}, 
		{0x9D11, "Invalid AID"}, 
		{0x9D12, "Duplicate AID"}, 
		{0x9D13, "Application previously loaded"}, 
		{0x9D14, "Application history list full"}, 
		{0x9D15, "Application not open"}, 
		{0x9D17, "Invalid offset"}, 
		{0x9D18, "Application already loaded"}, 
		{0x9D19, "Invalid certificate"}, 
		{0x9D1A, "Invalid signature"}, 
		{0x9D1B, "Invalid KTU"}, 
		{0x9D1D, "MSM controls not set"}, 
		{0x9D1E, "Application signature does not exist"}, 
		{0x9D1F, "KTU does not exist"}, 
		{0x9D20, "Application not loaded"}, 
		{0x9D21, "Invalid Open command data length"}, 
		{0x9D30, "Check data parameter is incorrect (invalid start address)"}, 
		{0x9D31, "Check data parameter is incorrect (invalid length)"}, 
		{0x9D32, "Check data parameter is incorrect (illegal memory check area)"}, 
		{0x9D40, "Invalid MSM Controls ciphertext"}, 
		{0x9D41, "MSM controls already set"}, 
		{0x9D42, "Set MSM Controls data length less than 2 bytes"}, 
		{0x9D43, "Invalid MSM Controls data length"}, 
		{0x9D44, "Excess MSM Controls ciphertext"}, 
		{0x9D45, "Verification of MSM Controls data failed"}, 
		{0x9D50, "Invalid MCD Issuer production ID"}, 
		{0x9D51, "Invalid MCD Issuer ID"}, 
		{0x9D52, "Invalid set MSM controls data date"}, 
		{0x9D53, "Invalid MCD number"}, 
		{0x9D54, "Reserved field error"}, 
		{0x9D55, "Reserved field error"}, 
		{0x9D56, "Reserved field error"}, 
		{0x9D57, "Reserved field error"}, 
		{0x9D60, "MAC verification failed"}, 
		{0x9D61, "Maximum number of unblocks reached"}, 
		{0x9D62, "Card was not blocked"}, 
		{0x9D63, "Crypto functions not available"}, 
		{0x9D64, "No application loaded"}, 
		{0x9E00, "PIN not installed"}, 
		{0x9E04, "PIN not succesfully verified, PIN not installed"}, 
		{0x9F00, "PIN blocked and Unblock Try Counter is 3"}, 
		{0x9F04, "PIN not succesfully verified, PIN blocked and Unblock Try Counter is 3"},
		
		{0x9000, "process completed"}, 
		{0x6401, "Exec error: Command timeout"}, 
		{0x6100, "Process completed, more data available"}, 
		{0x6200, "Warning: NV-Ram not changed"}, 
		{0x6201, "Warning: NV-Ram not changed 1"}, 
		{0x6281, "Warning: Part of data corrupted"}, 
		{0x6282, "Warning: End of file reached"}, 
		{0x6283, "Warning: Invalidated file"}, 
		{0x6284, "Warning: Invalid file control"}, 
		{0x6300, "Warning: NV-Ram changed"}, 
		{0x63C0, "Warning: Warning with counter"}, 
		{0x63C0, "Warning: Verify fail, no try left"}, 
		{0x63C1, "Warning: Verify fail, 1 try left"}, 
		{0x63C2, "Warning: Verify fail, 2 tries left"}, 
		{0x63C3, "Warning: Verify fail, 3 tries left"}, 
		{0x6400, "Exec error: NV-Ram not changed"}, 
		{0x6501, "Exec error: Memory failure"}, 
		{0x6581, "Exec error: Memory failure"}, 
		{0x6700, "Checking error: Wrong length"}, 
		{0x6800, "Checking error: Function in CLA byte not supported"}, 
		{0x6881, "Checking error: Logical channel not supported"}, 
		{0x6882, "Checking error: Secure Messaging not supported"}, 
		{0x6883, "Checking error: Last command of the chain expected"}, 
		{0x6884, "Checking error: Command chaining not supported"}, 
		{0x6900, "Checking error: Command not allowed"}, 
		{0x6981, "Checking error: Command incompatible with file structure"}, 
		{0x6982, "Checking error: Security condition not satisfied"}, 
		{0x6983, "Checking error: Authentication method locked"}, 
		{0x6984, "Checking error: Reference data not usable"}, 
		{0x6985, "Checking error: Condition of use not satisfied"}, 
		{0x6986, "Checking error: Command not allowed (no current EF)"}, 
		{0x6987, "Checking error: Expected secure messaging object missing"}, 
		{0x6988, "Checking error: Incorrect secure messaging data object"}, 
		{0x6A00, "Checking error: Wrong parameter P1-P2"}, 
		{0x6A80, "Checking error: Incorrect parameter in the command data field"}, 
		{0x6A81, "Checking error: Function not supported"}, 
		{0x6A82, "Checking error: File not found"}, 
		{0x6A82, "Checking error: File not found"}, 
		{0x6A83, "Checking error: Record not found"}, 
		{0x6A84, "Checking error: Not enough memory space in the file"}, 
		{0x6A85, "Checking error: Nc inconsistent with TLV structure"}, 
		{0x6A85, "Checking error: Access cond. n/f"}, 
		{0x6A86, "Checking error: Incorrect P1-P2"}, 
		{0x6A87, "Checking error: Lc inconsistent with P1-P2"}, 
		{0x6A88, "Checking error: Reference data not found"}, 
		{0x6A89, "Checking error: File already exists"}, 
		{0x6A8A, "Checking error: DF name already exists"}, 
		{0x6B00, "Checking error: Wrong parameter P1-P2"}, 
		{0x6C00, "Checking error: Invalid Le"}, 
		{0x6D00, "Checking error: Wrong instruction"}, 
		{0x6E00, "Checking error: Class not supported"}, 
		{0x9804, "Access conditions not satisfied"}, 
		{0x9210, "No more storage available"}, 
		{0x6F00, "Checking error: No precise diagnosis"},
	#endif
	{0x8000, "OK"}, 
	{0x9000, "Command successfully executed (OK)."}, 
	{0x8100, "NXP-NPC Possible LCR/CRC error"}, 
	{0x8200, "NXP-NPC Possible LCR/CRC error"}, 
	#else	
	{0x0000, ""}, 
	#endif
};



static struct gen_err {
	uint8_t sw;
	char *message;
} GENERAL_ERRORS[] = {
	#ifdef COMPILE_HW_ERRORS	
		{0x61, "\tResponse bytes still available SW2"},
		{0x62, "\tState of non-volatile memory unchanged"},
		{0x63, "\tState of non-volatile memory changed"},
		{0x64, "\tState of non-volatile memory unchanged"},
		{0x65, "\tState of non-volatile memory changed"},
		{0x66, "\tRecieving errors"},
		{0x67, "\tLength incorrect (procedure)(ISO 7816-3)"},
		{0x68, "\tFunctions in CLA not supported"},
		{0x69, "\tCommand not allowed"},
		{0x6A, "\tWrong parameter(s) P1-P2"},
		{0x6B, "\tReference incorrect (procedure byte), (ISO 7816-3)"},
		{0x6C, "\tBad length value in Le; 'xx' is the correct exact Le"},
		{0x6D, "\tInstruction code not programmed or invalid (procedure byte), (ISO 7816-3)"},
		{0x6E, "\tInstruction class not supported (procedure byte), (ISO 7816-3)"},
		{0x6F, "\tNo precise diagnosis (procedure byte), (ISO 7816-3)"},

		//{0x9XXX, "Application related status, (ISO 7816-3)"}, 
		{0x92, "\tWrite to EEPRom sucesfull after SW2 atempts"}, //
		{0x9F, "\tCommand successfully executed; 'xx' bytes of data are available and can be requested using GET RESPONSE."} //
	#else	
		{0x0000, ""}, 
	#endif
};

#define SPECIFC_ERRORS_COUNT (sizeof(SPECIFC_ERRORS) / sizeof(SPECIFC_ERRORS[0]))
#define GENERAL_ERRORS_COUNT (sizeof(GENERAL_ERRORS) / sizeof(GENERAL_ERRORS[0]))

