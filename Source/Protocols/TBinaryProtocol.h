/*
 * Persistence Library / Protocols / TSRProtocol
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TSRPROTOCOL___H
#define TSRPROTOCOL___H
#define SRPROTOCOL_MAGIC_BYTE_1 0x05
#define SRPROTOCOL_MAGIC_BYTE_2 0x0B
#define SRPROTOCOL_QUEUE_LENGTH 16
#define SRPROTOCOL_PING 0
#define SRPROTOCOL_TIMEOUT 30000


#include <stdlib.h>

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
    dmReadWriteToDataBuffer,
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

typedef unsigned char (*ReadByteFromStorage)(unsigned long address);
typedef void (*WriteByteToStorage)  (unsigned long address, unsigned char value);
typedef void (*SendByteCallback)    (unsigned char c);
typedef void (*PacketHeaderCallback) (unsigned short busAddress,   short command, short customParam1, short customParam2, unsigned short dataSize, bool &acceptCommand);
typedef void (*PacketCompleteCallback) (unsigned short busAddress, short command, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data );

class TSimpleBinaryProtocol
{
private:
	short m_idleCounter;       
    unsigned short      m_busAddress;
	
	eExpectedInputData  m_incomingStateMachine;
	short               m_incomingBusAddress;
	unsigned short      m_incomingCommand;
    short               m_incomingCustomParam;
    short               m_incomingCustomParam2;
	unsigned long       m_incomingDataSize;    
	//18 bytes + 2 magic bytes = 20 bytes

    unsigned char       m_incomingDataMode;
	WriteByteToStorage  m_incomingDataHandler;
    unsigned char*      m_incomingDataBuffer;
    unsigned short      m_incomingDataBufferSize;
	unsigned long       m_incomingDataCounter;
	bool                m_incomingCommandAccepted;
	unsigned short      m_incomingTimeout;
	unsigned short      m_incomingFletcherCRC;
    unsigned short      m_incomingFletcherCRC_sum1;
    unsigned short      m_incomingFletcherCRC_sum2;
	// 
	eExpectedInputData  m_outgoingStateMachine;
    unsigned short      m_outgoingBusAddress;
	unsigned short      m_outgoingCommand;
    unsigned short      m_outgoingCustomParam;
    unsigned short      m_outgoingCustomParam2;
    unsigned char*      m_outgoingDataBuffer;
	unsigned long       m_outgoingDataBufferSize;
    //unsigned short      m_outgoingFletcherCRC;
    unsigned short      m_outgoingFletcherCRC_sum1;
    unsigned short      m_outgoingFletcherCRC_sum2;	
	//18 bytes + 2 magic bytes = 20 bytes

	ReadByteFromStorage m_outgoingDataHandler;
	unsigned long       m_outgoingDataCounter;
	unsigned short      m_outgoingTimeout;
	bool                m_outgoingDataSending;
	//
	eProtocolError      m_lastError;

	SendByteCallback       m_callbackSendByte;    
	PacketHeaderCallback   m_callbackHeaderReceived;
	PacketCompleteCallback m_callbackCommandReceived;
	PacketCompleteCallback m_callbackPacketError;  
	PacketCompleteCallback m_callbacCommandSent;    

    void SendByte(unsigned char c);
	bool SendPacket (unsigned short busAddress, short command, short customParam1, short customParam2, unsigned long dataSize, unsigned char* data );
	void SetLastError(eProtocolError err);

public:

	TSimpleBinaryProtocol();
	void SetBusAddress (unsigned short busAddress);

    //read/write mode functions
    void SetDataMode(eSRDataMode dataMode);
    void SetDataBuffer(unsigned char* incomingDataBuffer, unsigned long incomingDataBufferSize);
	void SetOnFlyDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler);

    //events
    void SetEventHandlers(SendByteCallback sendByteHandler, PacketCompleteCallback packetReceivedHandler, PacketCompleteCallback packetErrorHandler, PacketCompleteCallback packetSentHandler);

	void OnByteReceived(unsigned char c); 
    unsigned short GetIncomingFletcherCRC();
	void OnByteSent();    
    unsigned short GetOutgoingFletcherCRC();
	void OnTimer(unsigned short intervalMS=20);

	void SendPing(unsigned short busAddress);
	void SendCommand(unsigned short busAddress, short command, unsigned char* pData, unsigned short dataSize);
	void SendCommandEx(unsigned short busAddress, short command, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize );
	void SendAck(unsigned short busAdress, unsigned short packetId);
	void SendAckOfAck(unsigned short busAddress, unsigned short packetId);

	eProtocolError GetLastError();

};


#endif
