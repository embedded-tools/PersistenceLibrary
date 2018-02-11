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

#define BINPROTOCOL_MAGIC_BYTE_1 0x05
#define BINPROTOCOL_MAGIC_BYTE_2 0x0B
#define BINPROTOCOL_QUEUE_LENGTH 16
#define BINPROTOCOL_PING 0
#define BINPROTOCOL_TIMEOUT 30000

#include <stdlib.h>

typedef enum eBinaryCommand
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
    bcReadDataFromExternalMemory = 0x61,    
    bcWriteDataToExternalMemory = 0x62,
    bcOpenPort = 0x71,
    bcClosePort = 0x72,
    bcWriteToPort = 0x73,
    bcReadFromPort = 0x74,
    bcCustomCommand = 0x80
} ECOMMAND;

typedef enum eExpectedInputData
{ 
	eidMagicByte1,        
	eidMagicByte2,        
	eidBusAddressLow,    
	eidBusAddressHigh,
    eidCommandLow,
    eidCommandHigh,
    eidCustomParamLow,
    eidCustomParamHigh,
    eidCustomParam2Low,
    eidCustomParam2High,
    eidDataLengthLow,
    eidDataLengthHigh,
    eidHeaderSumLow,
    eidHeaderSumHigh,
	eidData,
	eidDataSumLow,
	eidDataSumHigh,         
	eidWaitForLastByte,
	eidWaitForNewCommand
} EEXPECTEDINPUTDATA;

typedef enum eSRDataMode
{
    dmReadWriteToTemporaryBufferFirst,
    dmReadWriteOnFly
} ESRDATAMODE;

typedef enum eProtocolError
{
	peSuccess,
	peInvalidHeaderCRC,
	peInvalidDataCRC,
	peCustomHeaderValidationFailed,
	peNoIncomingDataHandler,
	peNoOutgoingDataHandler,
	peDataBufferTooSmall
} EPROTOCOLERROR;

class TBinaryProtocol;

typedef unsigned char (*ReadByteFromStorage)(unsigned short address);
typedef void (*WriteByteToStorage)  (unsigned short address, unsigned char value);
typedef void (*SleepCallback)  (unsigned short timeout);
typedef void (*SendByteCallback)    (unsigned char c);
typedef void (*CommandHeaderReceivedCallback) (short busAddress,   unsigned short command, short customParam1, short customParam2, unsigned short dataSize, bool &acceptCommand);
typedef void (*CommandReceivedCallback) (TBinaryProtocol* sender, unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data );

class TBinaryProtocol
{
private:
	short m_idleCounter;       
    unsigned short      m_busAddress;
	
	eExpectedInputData  m_incomingStateMachine;
	short               m_incomingBusAddress;
	unsigned short      m_incomingCommand;
    short               m_incomingCustomParam;
    short               m_incomingCustomParam2;
	unsigned short      m_incomingDataSize;    
	//18 bytes + 2 magic bytes = 20 bytes

    unsigned char       m_incomingDataMode;
	WriteByteToStorage  m_incomingDataHandler;
    unsigned char*      m_incomingDataBuffer;
    unsigned short      m_incomingDataBufferSize;
	unsigned short      m_incomingDataCounter;
	bool                m_incomingCommandAccepted;
	unsigned short      m_incomingTimeout;
	unsigned short      m_incomingFletcherCRC;
    unsigned short      m_incomingFletcherCRC_sum1;
    unsigned short      m_incomingFletcherCRC_sum2;
    bool                m_incomingPacketReady;
    bool                m_incomingAcknowledge;
	// 
	eExpectedInputData  m_outgoingStateMachine;
    unsigned short      m_outgoingBusAddress;
	unsigned short      m_outgoingCommand;
    unsigned short      m_outgoingCustomParam;
    unsigned short      m_outgoingCustomParam2;
    unsigned char*      m_outgoingDataBuffer;
	unsigned short      m_outgoingDataBufferSize;
    unsigned short      m_outgoingFletcherCRC_sum1;
    unsigned short      m_outgoingFletcherCRC_sum2;	
	//18 bytes + 2 magic bytes = 20 bytes

	ReadByteFromStorage m_outgoingDataHandler;
	unsigned short      m_outgoingDataCounter;
	unsigned short      m_outgoingTimeout;
	bool                m_outgoingDataSending;
	//
	eProtocolError      m_lastError;

	SendByteCallback       m_callbackSendByte;    
    SleepCallback          m_callbackSleep;
	CommandHeaderReceivedCallback  m_callbackHeaderReceived;
	CommandReceivedCallback m_callbackCommandReceived;
	CommandReceivedCallback m_callbackCommandReceivingError;  
	CommandReceivedCallback m_callbackCommandSent;        

    void SendByte(unsigned char c);
    bool SendPacket (unsigned short command, unsigned short errorCode, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data );
	void SetLastError(eProtocolError err);

    void SendCommand(unsigned short busAddress, unsigned short command, unsigned char* pData, unsigned short dataSize);
    void SendCommandEx(unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize );    

    unsigned short GetIncomingFletcherCRC();
        
    unsigned short GetOutgoingFletcherCRC();

    virtual bool WaitForResponse(unsigned short command, unsigned char* pOutput=NULL, unsigned short* pOutputLength=NULL, unsigned short maxOutputLength=0);
public:

	TBinaryProtocol();
	void SetBusAddress (unsigned short busAddress);

    //read/write mode functions
    void SetDataMode(eSRDataMode dataMode);
    void SetDataBuffer(unsigned char* incomingDataBuffer, unsigned short incomingDataBufferSize);
	void SetOnFlyDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler);
    void SetSleepCallback(SleepCallback sleepFunction);

    //events
    void SetEventHandlers(SendByteCallback sendByteHandler, CommandReceivedCallback packetReceivedHandler, CommandReceivedCallback packetErrorHandler, CommandReceivedCallback packetSentHandler);

	void OnByteReceived(unsigned char c); 
	void OnTimer(unsigned short intervalMS=20);
    void OnByteSent();

    eProtocolError GetLastError();
  
    bool SendPing(unsigned short deviceAddress);
    bool LedGreenOn(unsigned short deviceAddress, unsigned short ledNumber);
    bool LedGreenOff(unsigned short deviceAddress, unsigned short ledNumber);
    bool LedGreenToggle(unsigned short deviceAddress, unsigned short ledNumber);
    bool LedRedOn(unsigned short deviceAddress, unsigned short ledNumber);
    bool LedRedOff(unsigned short deviceAddress, unsigned short ledNumber);
    bool LedRedToggle(unsigned short deviceAddress, unsigned short ledNumber);
    bool OutputOn(unsigned short deviceAddress, unsigned short outputNumber);
    bool OutputOff(unsigned short deviceAddress, unsigned short outputNumber);
    bool OutputToggle(unsigned short deviceAddress, unsigned short outputNumber);
    bool Beep(unsigned short deviceAddress, unsigned short frequency, unsigned short length);
    bool ReadConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput, unsigned char maxOutputLength);
    bool StoreConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pData);
    bool ReadData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput, unsigned short maxOutputLength);
    bool WriteData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput);
    bool ReadExternalData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput, unsigned short maxOutputLength);
    bool WriteExternalData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput);

    bool OpenPort(unsigned short deviceAddress, unsigned short portAddress);
    bool ClosePort(unsigned short deviceAddress, unsigned short portAddress);
    bool WriteToPort(unsigned short deviceAddress, unsigned char* pData, unsigned short dataLength);
    bool ReadFromPort(unsigned short deviceAddress, unsigned short timeout, unsigned char* pData, unsigned short* pDataLength, const unsigned short maxDataLength);
    bool SendCustomCommand(unsigned short deviceAddress, unsigned short command, unsigned short customParam1, unsigned short customParam2, 
                           unsigned char* pInputData=NULL, unsigned short inputDataLength=0,
                           unsigned char* pOutputData=NULL, unsigned short* pOutputDataLength=NULL, unsigned short maxOutputDataLength=0);
    void SendCustomResponse(unsigned short command, unsigned short errorCode, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize);

};


#endif
