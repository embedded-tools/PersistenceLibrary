/*
 * Persistence Library / Protocols / TSRProtocol
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef TBINPROTOCOL___H
#define TBINPROTOCOL___H

#include <stdlib.h>
#include "TUserManager.h"

enum ECommand : unsigned short
{
	bcPing = 0x01,
    bcLogin = 0x02,
    bcLogout = 0x03,
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
	bcWriteConfiguration = 0x52,
	bcReadData = 0x61,    
	bcWriteData = 0x62,
	bcReadDataExternal = 0x63,    
	bcWriteDataExternal = 0x64,
	bcOpenPort = 0x71,
	bcClosePort = 0x72,
	bcReadFromPort = 0x73,
	bcWriteToPort = 0x74,
	bcCustomCommand = 0x80,
	bcMax,
	bcForceInt16 = 0x7FFF
};

enum ECommandResult : unsigned short
{
	crUnknown = 0x00,
    crSuccess,
    crTimeout,
	crClassNotInitialized,
    crLoginRequired, 
	crInvalidCRC,
	crInvalidCommand,
	crInvalidRequest,
	crInvalidResponse,
	crInvalidArgument,
    crInvalidCredentials,
	crBufferTooSmall,
    crAddressMismatch,
    	    
	crForceInt16 = 0x7FFF
};

struct TRequest
{
	unsigned short packetId;
	unsigned short senderAddress;
	unsigned short targetAddress;
	ECommand       command;
	unsigned short customParam1;
	unsigned short customParam2;
	unsigned short customParam3;										 
	unsigned short dataLength;
	unsigned char* data;
	ECommandResult error;
};

struct TResponse
{
	unsigned short packetId;
	unsigned short senderAddress;
	unsigned short targetAddress;
	ECommandResult response;
	unsigned short dataLength;
	unsigned char* data;
	ECommandResult error;	
};
  

class TBinaryProtocol
{
public:
	typedef void (*SleepCallback)(unsigned short time_ms);
	typedef void (*SendPacketCallback)(unsigned char* data, unsigned short dataLength);

private:
	
	unsigned char          m_magicByte1;
	unsigned char          m_magicByte2;
	unsigned short         m_packetCounter;
	unsigned short         m_busAddress;
	
    SleepCallback          m_sleepCallback;
	SendPacketCallback     m_sendPacketCallback;
	unsigned char*         m_inputBuffer;
	unsigned short         m_inputBufferSize;
	unsigned short         m_inputBufferPos;
	unsigned char*         m_outputBuffer;
	unsigned short         m_outputBufferSize;
	unsigned short         m_outputBufferPos;
    bool                   m_loginComplete;
    unsigned short         m_loginDeviceAddress;
    TUserManager*          m_userManager;
    unsigned char          m_lastNonce[64];

    bool VerifyPassword (unsigned char* hash, unsigned short hashLength);
    bool CalculateHash(const char* userName, const char* password, 
                         unsigned char* nonce, unsigned short nonceLength, 
                         unsigned char* out_hash);

public:

    TBinaryProtocol(bool loginRequiredFirst = false);
	
	//class settings
	void SetBusAddress  (unsigned short busAddress);
	void SetSleepCallback(SleepCallback sleepFunction);
	void SetSendPacketCallback(SendPacketCallback sendPacketFunction);
	void SetOutputBuffer(unsigned char* outputPacketBuffer, unsigned short outputPacketSize);
	void SetInputBuffer(unsigned char* inputPacketBuffer, unsigned short inputPacketSize);
    void SetUserManager(TUserManager* userManager);

	//buffer 
	void ClearOutputBuffer();
	void ClearInputBuffer();
	void DeleteFromInputBuffer(unsigned short dataLength);
	void DataReceived(unsigned char* data, unsigned short dataLength);
	
	//generic commands									
	TResponse WaitForSpecificResponse(const TRequest request,
		                              unsigned short timeOut);

	TResponse WaitForResponse(unsigned short timeOut,
		                      unsigned short expectedPacketId = 0);

	TRequest WaitForRequest (unsigned short timeOut);
									
	TRequest SendCommand(unsigned short deviceAddress,
					     ECommand       command,
					     unsigned short param1 = 0,
		 			     unsigned short param2 = 0,
		 			     unsigned short param3 = 0,
		 			     unsigned short dataLength = 0,
		 			     unsigned char* data = NULL);

	TResponse SendResponse(TRequest request,
                           ECommandResult response, 
		                   unsigned short dataLength = 0,
		                   unsigned char* data = NULL);

    bool Login(unsigned short deviceAddress, const char* userName, const char* password, int timeOut);

	TRequest SendPing(unsigned short deviceAddress);
	TRequest LedGreenOn(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest LedGreenOff(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest LedGreenToggle(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest LedRedOn(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest LedRedOff(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest LedRedToggle(unsigned short deviceAddress, unsigned short ledNumber);
	TRequest OutputOn(unsigned short deviceAddress, unsigned short outputNumber);
	TRequest OutputOff(unsigned short deviceAddress, unsigned short outputNumber);
	TRequest OutputToggle(unsigned short deviceAddress, unsigned short outputNumber);
    
	TRequest Beep(unsigned short deviceAddress, unsigned short frequency, unsigned short length);
	TRequest ReadConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length);
	TRequest WriteConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data);

    TRequest ReadIO(unsigned short deviceAddress, unsigned short ioNumber);
    TRequest WriteIO(unsigned short deviceAddress, unsigned short ioNumber, unsigned short ioValue);
	TRequest ReadData(unsigned short deviceAddress, unsigned short address, unsigned short length);
	TRequest WriteData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data);
	TRequest ReadDataExternal(unsigned short deviceAddress, unsigned short address, unsigned short length);
	TRequest WriteDataExternal(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data);
	TRequest OpenPort(unsigned short deviceAddress, unsigned short portAddress);
	TRequest ClosePort(unsigned short deviceAddress, unsigned short portAddress);
	TRequest WriteToPort(unsigned short deviceAddress, unsigned short portAddress, unsigned char* data, unsigned short dataLength);
	TRequest ReadFromPort(unsigned short deviceAddress, unsigned short portAddress, unsigned short maxDataLength, unsigned short timeOut);
	TRequest SendCustomCommand(unsigned short command, 
							  unsigned short deviceAddress, 
							  unsigned short customParam1, 
						      unsigned short customParam2, 
						      unsigned short customParam3, 
						      unsigned char* data, 
						      unsigned short dataLength);
    bool Logout(unsigned short deviceAddress, int timeOut);

};


#endif
