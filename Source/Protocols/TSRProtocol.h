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
#define SRPROTOCOL_SUM_CONST 75

#include <stdlib.h>

typedef enum eExpectedInputData
{ 
	eidMagicByte1, 
	eidMagicByte2,     
	eidBusAddressLow,
	eidBusAddressHigh,
	eidPacketType,  
	eidReserved,
	eidPacketIdLow, 
	eidPacketIdHigh,
	eidPacketCommandLow,
	eidPacketCommandHigh, 
	eidCustomParam1Low,
	eidCustomParam1High,
	eidCustomParam2Low,
	eidCustomParam2High,
	eidDataSize0, 
	eidDataSize1, 
	eidDataSize2, 
	eidDataSize3, 
	eidHeaderSumLow, 
	eidHeaderSumHigh,
	eidData,
	eidDataSumLow,
	eidDataSumHigh,
	eidWaitForLastByte,
	eidWaitForNewCommand
};

typedef enum ePacketType 
{
	ptInvalid,
	ptCommand,
	ptAck,
	ptAckOfAck
};

typedef enum eProtocolError
{
	peSuccess,
	peInvalidHeaderCRC,
	peInvalidDataCRC,
	peCustomHeaderValidationFailed,
	peNoIncomingDataHandler,
	peNoOutgoingDataHandler,
	peDataBufferTooSmall
};

typedef unsigned char (*ReadByteFromStorage)(unsigned long address);
typedef void (*WriteByteToStorage)(unsigned long address, unsigned char value);
typedef void (*SendByteCallback)    (unsigned char c);
typedef bool (*HeaderEventCallback) (unsigned short busAddress, ePacketType packetType, short packetId, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize);
typedef void (*PacketEventCallback) (unsigned short busAddress, ePacketType packetType, short packetId, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize, unsigned char* data );

class TSRProtocol
{
private:
	short m_packetIdCounter;  
	short m_idleCounter;       
	short m_packet_AwaitingAck[SRPROTOCOL_QUEUE_LENGTH];

	unsigned short      m_deviceAddress;
	eExpectedInputData  m_incomingStateMachine;

	unsigned char       m_incomingReserved;
	short               m_incomingDeviceAddress;
	ePacketType         m_incomingPacketType;    
	short               m_incomingPacketId;    
	unsigned short      m_incomingCommand;
	unsigned short      m_incomingCustomParam1;
	unsigned short      m_incomingCustomParam2;
	unsigned long       m_incomingDataSize;    
	unsigned short      m_incomingHeaderSum;
	//18 bytes + 2 magic bytes = 20 bytes

	unsigned char*      m_incomingDataBuffer;  
	unsigned long       m_incomingDataBufferSize;
	WriteByteToStorage  m_incomingDataHandler;
	unsigned long       m_incomingDataCounter;
	unsigned short      m_incomingDataSum;
	bool                m_incomingDataIgnore;
	unsigned short      m_incomingTimeout;
	unsigned short      m_incomingRealSum;
	// 
	eExpectedInputData  m_outgoingStateMachine;
	unsigned char       m_outgoingReserved;
	short               m_outgoingDeviceAddress;
	ePacketType         m_outgoingPacketType;    
	short               m_outgoingPacketId;    
	unsigned short      m_outgoingCommand;
	unsigned short      m_outgoingCustomParam1;
	unsigned short      m_outgoingCustomParam2;
	unsigned long       m_outgoingDataSize;    
	unsigned short      m_outgoingHeaderSum;
	//18 bytes + 2 magic bytes = 20 bytes

	unsigned char*      m_outgoingDataBuffer;
	unsigned long       m_outgoingDataBufferSize;
	ReadByteFromStorage m_outgoingDataHandler;
	unsigned long       m_outgoingDataCounter;
	unsigned short      m_outgoingDataSum;
	unsigned short      m_outgoingTimeout;
	unsigned short      m_outgoingRealSum;
	bool                m_outgoingDataSending;
	//
	eProtocolError      m_lastError;

	SendByteCallback    m_callbackSendByte;    
	HeaderEventCallback m_callbackHeaderReceived;
	PacketEventCallback m_callbackPacketReceived;
	PacketEventCallback m_callbackPacketError;  
	PacketEventCallback m_callbackPacketSent;    

	bool SendPacket (unsigned short busAddress, ePacketType packetType, short packetId, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize, unsigned char* data );
	void SetLastError(eProtocolError err);

public:

	TSRProtocol();
	void SetAddress (unsigned short busAddress);
	void SetDataBuffer(unsigned char* incomingDataBuffer, unsigned long incomingDataBufferSize);
	void SetDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler);
	void SetEventHandlers(SendByteCallback sendByteHandler, PacketEventCallback packetReceivedHandler, PacketEventCallback packetErrorHandler=NULL, PacketEventCallback packetSentHandler=NULL);

	void OnByteReceived(unsigned char c); 
	void OnByteSent();    
	void OnTimer(unsigned short intervalMS=20);

	void SendPing(unsigned short busAddress);
	void SendCommand(unsigned short busAddress, short command, unsigned long dataSize, unsigned char* data );
	void SendCommandEx(unsigned short busAddress, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize, unsigned char* data );
	void SendAck(unsigned short busAdress, unsigned short packetId);
	void SendAckOfAck(unsigned short busAddress, unsigned short packetId);


	eProtocolError GetLastError();

};


#endif
