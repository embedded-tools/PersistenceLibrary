#ifndef ECOMMAND___H
#define ECOMMAND___H

typedef enum eCommand
{
	bcPing = 0,
	bcLedGreenOn = 0x11,
	bcLedGreenOff = 0x12,
	bcLedGreenToggle = 0x13,
	bcLedRedOn = 0x21,
	bcLedRedOff = 0x22,
	bcLedRedToggle = 0x23,
	bcOutputOn = 0x31,
	bcOutputOff = 0x32,
	bcOutputToggle = 0x33,
	bcBeep = 0x41,
	bcReadConfiguration = 0x51,
	bcStoreConfiguration = 0x52,
	bcReadData = 0x61,    
	bcWriteData = 0x62,
	bcReadDataFromExternalMemory = 0x63,    
	bcWriteDataToExternalMemory = 0x64,
	bcOpenPort = 0x71,
	bcClosePort = 0x72,
	bcWriteToPort = 0x73,
	bcReadFromPort = 0x74,
	bcCustomCommand = 0x80,
	bcMax
} ECOMMAND;

#endif