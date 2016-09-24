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

#include "TSRProtocol.h"
#include <stdlib.h>

TSRProtocol::TSRProtocol()
   :m_packetIdCounter(0),
    m_idleCounter(0),
    m_incomingStateMachine(eidMagicByte1),
    m_incomingDeviceAddress(0),
    m_incomingPacketType(ptCommand),
    m_incomingReserved(0),
    m_incomingPacketId(0),
    m_incomingCommand(0),
    m_incomingCustomParam1(0),
    m_incomingCustomParam2(0),
    m_incomingDataSize(0),
    m_incomingHeaderSum(0),
    m_incomingDataBuffer(NULL),
    m_incomingDataBufferSize(0),
    m_incomingDataHandler(NULL),
    m_incomingDataCounter(0),
    m_incomingDataSum(0),
    m_incomingTimeout(30000),
    m_incomingRealSum(0),
    m_outgoingStateMachine(eidWaitForNewCommand),
    m_outgoingDeviceAddress(0),
    m_outgoingPacketType(ptCommand),
    m_outgoingReserved(0),
    m_outgoingPacketId(0),
    m_outgoingCommand(0),
    m_outgoingCustomParam1(0),
    m_outgoingCustomParam2(0),
    m_outgoingDataSize(0),
    m_outgoingHeaderSum(0),
    m_outgoingDataBuffer(NULL),
    m_outgoingDataBufferSize(0),
    m_outgoingDataHandler(NULL),
    m_outgoingDataCounter(0),
    m_outgoingDataSum(0),
    m_outgoingTimeout(30000),
    m_outgoingRealSum(0),
    m_outgoingDataSending(false),
    m_lastError(peSuccess),
    m_callbackSendByte(NULL),
    m_callbackHeaderReceived(NULL),
    m_callbackPacketError(NULL),
    m_callbackPacketReceived(NULL),
    m_callbackPacketSent(NULL)
{
     
}

void TSRProtocol::SetDataBuffer(unsigned char* incomingDataBuffer, unsigned long incomingDataBufferSize)
{
	m_incomingDataBuffer     = incomingDataBuffer;
	m_incomingDataBufferSize = incomingDataBufferSize;
	m_incomingDataHandler    = NULL;
	m_outgoingDataHandler    = NULL;
}

void TSRProtocol::SetDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler)
{
	m_incomingDataBuffer     = NULL;
	m_incomingDataBufferSize = NULL;
	m_incomingDataHandler    = incomingDataHandler;
	m_outgoingDataHandler    = outgoingDataHandler;
}

void TSRProtocol::SetEventHandlers(SendByteCallback sendByteHandler, PacketEventCallback packetReceivedHandler, PacketEventCallback packetErrorHandler, PacketEventCallback packetSentHandler)
{
	m_callbackSendByte       = sendByteHandler;
	m_callbackPacketReceived = packetReceivedHandler;
	m_callbackPacketError    = packetErrorHandler;
	m_callbackPacketSent     = packetSentHandler;
}

void TSRProtocol::SetLastError(eProtocolError err)
{
    m_lastError = err;
}
             
void TSRProtocol::SetAddress(unsigned short deviceAddress)
{
    m_deviceAddress = deviceAddress;
}



void TSRProtocol::OnByteReceived(unsigned char c)
{
    switch (m_incomingStateMachine)
    {
        case eidMagicByte1:
            {
                m_incomingTimeout = 0;
                if (c==SRPROTOCOL_MAGIC_BYTE_1) 
                {
                    m_incomingStateMachine = eidMagicByte2;
                }
                m_incomingRealSum = c;
            }
            break;

        case eidMagicByte2:
            {
                if (c==SRPROTOCOL_MAGIC_BYTE_2)
                {
                    m_incomingStateMachine = eidBusAddressLow;
                } else {
                    m_incomingStateMachine = eidMagicByte1;
                }
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
            }
            break;

        case eidBusAddressLow: 
            {
                m_incomingDeviceAddress = c;        
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c; 
                m_incomingStateMachine = eidBusAddressHigh; 
            }
            break;

        case eidBusAddressHigh: 
            {
                m_incomingDeviceAddress += (c<<8); 
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidPacketType; 
            }
            break;

        case eidPacketType: 
            {
                m_incomingPacketType = (ePacketType) c;    
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidReserved;
            }
            break;

        case eidReserved:
            {
                m_incomingReserved = c;    
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidPacketIdLow; 
            }
            break;


        case eidPacketIdLow: 
            {
                m_incomingPacketId = c;   
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidPacketIdHigh; 
            }
            break;

        case eidPacketIdHigh: 
            {
                m_incomingPacketId += (c<<8);    
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidPacketCommandLow; 
            }
            break;

        case eidPacketCommandLow: 
            {
                m_incomingCommand = c;    
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidPacketCommandHigh;
            }
            break;

        case eidPacketCommandHigh: 
            {
                m_incomingCommand += (c<<8);     
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidCustomParam1Low; 
            }
            break;

        case eidCustomParam1Low:
            {
                m_incomingCustomParam1 = c;
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidCustomParam1High;
            }
            break;

        case eidCustomParam1High:
            {
                m_incomingCustomParam1 += (c<<8);
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidCustomParam2Low;
            }
            break;

        case eidCustomParam2Low:
            {
                m_incomingCustomParam2 = c;
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidCustomParam2High;
            }
            break;

        case eidCustomParam2High:
            {
                m_incomingCustomParam2 += (c<<8);
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidDataSize0;
            }
            break;

        case eidDataSize0:
            {
                m_incomingDataSize = c;
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidDataSize1;
            }
            break;

        case eidDataSize1:
            {
                m_incomingDataSize += (c<<8);
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidDataSize2;
            }
            break;

        case eidDataSize2:
            {
                m_incomingDataSize += (c<<16);
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidDataSize3;
            }
            break;

        case eidDataSize3:
            {
                m_incomingDataSize += (c<<24);
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                m_incomingStateMachine = eidHeaderSumLow;
            }
            break;

        case eidHeaderSumLow:
            {
                m_incomingHeaderSum = c;
                m_incomingStateMachine = eidHeaderSumHigh;
            }
            break;

        case eidHeaderSumHigh:
            {
                m_incomingHeaderSum += (c<<8);
                if (m_incomingHeaderSum==m_incomingRealSum)
                {
					m_incomingDataIgnore = false;
					if (m_callbackHeaderReceived)
					{
						bool res = m_callbackHeaderReceived(m_incomingDeviceAddress, m_incomingPacketType, m_incomingPacketId, m_incomingCommand, m_incomingCustomParam1, m_incomingCustomParam2, m_incomingDataSize);
						if (!res)
						{
							m_incomingDataIgnore = true;
						}
					}
                    m_incomingStateMachine = eidData;     
                    if (m_incomingDataSize==0)
                    {
                        m_incomingStateMachine = eidWaitForLastByte;
						OnByteReceived(0);
                    } else 
                    {
						if (m_incomingDataBuffer!=NULL)
						{
							if (m_incomingDataBufferSize<m_incomingDataSize)
							{
								SetLastError(peDataBufferTooSmall);
								if (m_callbackPacketError)
								{
									m_callbackPacketError(0, ptInvalid, 0, 0, 0, 0, 0, NULL);
								}
								m_incomingDataIgnore = true;
							}
						}
                    }
                } else {
					m_incomingStateMachine = eidMagicByte1;
                    SetLastError(peInvalidHeaderCRC);                    
					if (m_callbackPacketError)
					{
						m_callbackPacketError(0, ptInvalid, 0, 0, 0, 0, 0, NULL);
					}
                }            
            }
            break;

        case eidData:
            {
                m_incomingRealSum *= SRPROTOCOL_SUM_CONST;
                m_incomingRealSum += c;
                
				if (!m_incomingDataIgnore)
				{
					if (m_incomingDataBuffer!=NULL)
					{
						m_incomingDataBuffer[m_incomingDataCounter] = c;
					}
					if (m_incomingDataHandler!=NULL)
					{
						m_incomingDataHandler(m_incomingDataCounter, c);
					}
				}
                m_incomingDataCounter++;
                if (m_incomingDataCounter==m_incomingDataSize)
                {
                    m_incomingStateMachine = eidDataSumLow;
                }
            }
            break;

        case eidDataSumLow:
            {
                m_incomingDataSum = c;
                m_incomingStateMachine = eidDataSumHigh;
            }
            break;

        case eidDataSumHigh:
            {			
				m_incomingStateMachine = eidMagicByte1;
				if (!m_incomingDataIgnore)
				{
					m_incomingDataSum+= c<<8;
					if (m_incomingDataSum == m_incomingRealSum)
					{
						m_incomingStateMachine = eidWaitForLastByte;
						OnByteReceived(c);
					} else
					{						
						if (m_callbackPacketError)
						{
							m_callbackPacketError(m_incomingDeviceAddress, m_incomingPacketType, m_incomingPacketId, m_incomingCommand, m_incomingCustomParam1, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
						}						
					}
				}				
			}
            break;

        case eidWaitForLastByte:
            {
                if (m_callbackPacketReceived)
                {
                    m_callbackPacketReceived(m_incomingDeviceAddress, m_incomingPacketType, m_incomingPacketId, m_incomingCommand, m_incomingCustomParam1, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
                }
				m_incomingStateMachine = eidMagicByte1;
            }
            break;

    }
}

void TSRProtocol::OnByteSent()
{
    unsigned char c = 0;
    switch (m_outgoingStateMachine)
    {
        case eidMagicByte1:
            {
                c = SRPROTOCOL_MAGIC_BYTE_1;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingTimeout = 0;                
                m_outgoingRealSum = c;
                m_outgoingStateMachine = eidMagicByte2;
            }
            break;

        case eidMagicByte2:
            {
                c = SRPROTOCOL_MAGIC_BYTE_2;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidBusAddressLow;
            }
            break;

        case eidBusAddressLow: 
            {
                c = (unsigned char)m_outgoingDeviceAddress;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c; 
                m_outgoingStateMachine = eidBusAddressHigh; 
            }
            break;

        case eidBusAddressHigh: 
            {
                c = (unsigned char)(m_outgoingDeviceAddress>>8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidPacketType; 
            }
            break;

        case eidPacketType: 
            {
                c = (unsigned char)m_outgoingPacketType;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidReserved; 
            }
            break;

        case eidReserved:
            {
                c = (unsigned char)m_outgoingReserved;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidPacketIdLow; 
            }
            break;


        case eidPacketIdLow: 
            {
                c = (unsigned char)m_outgoingPacketId;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidPacketIdHigh; 
            }
            break;

        case eidPacketIdHigh: 
            {
                c = (unsigned char)(m_outgoingPacketId>>8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidPacketCommandLow; 
            }
            break;

        case eidPacketCommandLow: 
            {
                c = (unsigned char)m_outgoingCommand;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidPacketCommandHigh;
            }
            break;

        case eidPacketCommandHigh: 
            {
                c = (unsigned char)(m_outgoingCommand >> 8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidCustomParam1Low; 
            }
            break;

        case eidCustomParam1Low:
            {
                c = (unsigned char)m_outgoingCustomParam1;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidCustomParam1High;
            }
            break;

        case eidCustomParam1High:
            {
                c = (unsigned char)(m_outgoingCustomParam1>>8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidCustomParam2Low;
            }
            break;

        case eidCustomParam2Low:
            {
                c = (unsigned char)m_outgoingCustomParam2;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidCustomParam2High;
            }
            break;

        case eidCustomParam2High:
            {
                c = (unsigned char)(m_outgoingCustomParam2>>8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidDataSize0;
            }
            break;

        case eidDataSize0:
            {
                c = (unsigned char)m_outgoingDataSize;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidDataSize1;
            }
            break;

        case eidDataSize1:
            {
                c = (unsigned char)(m_outgoingDataSize >> 8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidDataSize2;
            }
            break;

        case eidDataSize2:
            {
                c = (unsigned char)(m_outgoingDataSize >> 16);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidDataSize3;
            }
            break;

        case eidDataSize3:
            {
                c = (unsigned char)(m_outgoingDataSize >> 24);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;
                m_outgoingRealSum += c;
                m_outgoingStateMachine = eidHeaderSumLow;            
            }
            break;

        case eidHeaderSumLow:
            {
                c = (unsigned char)m_outgoingRealSum;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingStateMachine = eidHeaderSumHigh;
            }
            break;

        case eidHeaderSumHigh:
            {
                c = (unsigned char)(m_outgoingRealSum>>8);                
                if (m_callbackSendByte) m_callbackSendByte(c);

                m_outgoingDataCounter = 0;
                if (m_outgoingDataSize==0)
                {
                    m_outgoingStateMachine = eidWaitForLastByte;
                } else {
                    m_outgoingStateMachine = eidData;                    
                } 
            }
            break;

        case eidData:
            {                
                if (m_outgoingDataBuffer!=NULL)
                {
                    c = m_outgoingDataBuffer[m_outgoingDataCounter];
                }
                if (m_outgoingDataHandler!=NULL)
                {
                    c = m_outgoingDataHandler(m_outgoingDataCounter);   
                }
				if (m_callbackSendByte) m_callbackSendByte(c);

				m_outgoingRealSum *= SRPROTOCOL_SUM_CONST;                                
				m_outgoingRealSum += c;

                m_outgoingDataCounter++;
                if (m_outgoingDataCounter==m_outgoingDataSize)
                {
                    m_outgoingStateMachine = eidDataSumLow;
                }
            }
            break;

        case eidDataSumLow:
            {
                c = (unsigned char)m_outgoingRealSum;
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingStateMachine = eidDataSumHigh;
            }
            break;

        case eidDataSumHigh:
            {
                c = (unsigned char)(m_outgoingRealSum>>8);
                if (m_callbackSendByte) m_callbackSendByte(c);
                m_outgoingStateMachine = eidWaitForLastByte;
            }
            break;

        case eidWaitForLastByte:
            {
                m_outgoingDataSending = false;
                m_outgoingStateMachine = eidWaitForNewCommand;
                if (m_callbackPacketSent)
                {
                    m_callbackPacketSent(m_outgoingDeviceAddress, m_outgoingPacketType, m_outgoingPacketId, m_outgoingCommand, m_outgoingCustomParam1, m_outgoingCustomParam2, m_outgoingDataSize, m_outgoingDataBuffer);
                }
            }
            break;

        case eidWaitForNewCommand:
            {
                //do nothing
            }
            break;
    }
}

void TSRProtocol::OnTimer(unsigned short intervalMS)
{
    m_incomingTimeout++;
    if (m_incomingTimeout==SRPROTOCOL_TIMEOUT)
    {
        m_incomingStateMachine = eidMagicByte1;
    }
}

bool TSRProtocol::SendPacket (unsigned short busAddress, ePacketType packetType, short packetId, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize, unsigned char* data )
{
    if (m_outgoingDataSending)
    {
        return false;
    }
    m_outgoingDataSending = true;
    m_outgoingDeviceAddress = busAddress;
    m_outgoingPacketType = packetType;
    m_outgoingReserved = 0;
    m_outgoingPacketId = packetId;
    m_outgoingCommand = command;
    m_outgoingCustomParam1 = customParam1;
    m_outgoingCustomParam2 = customParam2;
    m_outgoingDataSize = dataSize;
    m_outgoingDataBuffer = data;
    m_outgoingStateMachine = eidMagicByte1;
    OnByteSent();
    return true;
}

eProtocolError TSRProtocol::GetLastError()
{
    return m_lastError;
}

void TSRProtocol::SendPing(unsigned short busAddress)
{
    m_packetIdCounter++;
    SendPacket(busAddress, ptCommand, m_packetIdCounter, SRPROTOCOL_PING, 0, 0, 0, NULL);
}

void TSRProtocol::SendCommand(unsigned short busAddress, short command, unsigned long dataSize, unsigned char* data )
{
    m_packetIdCounter++;
    SendPacket(busAddress, ptCommand, m_packetIdCounter, command, 0, 0, dataSize, data);
}

void TSRProtocol::SendCommandEx(unsigned short busAddress, short command, unsigned short customParam1, unsigned short customParam2, unsigned long dataSize, unsigned char* data )
{
    m_packetIdCounter++;
    SendPacket(busAddress, ptCommand, m_packetIdCounter, command, customParam1, customParam2, dataSize, data);
}

void TSRProtocol::SendAck(unsigned short busAddress, unsigned short packetId)
{
    SendPacket(busAddress, ptCommand, packetId, ptAck, 0, 0, 0, NULL);
}

void TSRProtocol::SendAckOfAck(unsigned short busAddress, unsigned short packetId)
{
    SendPacket(busAddress, ptCommand, packetId, ptAckOfAck, 0, 0, 0, NULL);
}





