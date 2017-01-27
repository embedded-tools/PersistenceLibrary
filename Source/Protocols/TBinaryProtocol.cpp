/*
 * Persistence Library / Protocols / TSimpleBinaryProtocol
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

#include "TBinaryProtocol.h"
#include <stdlib.h>

TSimpleBinaryProtocol::TSimpleBinaryProtocol()
   :
    m_idleCounter(0),
    m_incomingStateMachine(eidMagicByte1),
    m_incomingBusAddress(0),
    m_incomingCommand(0),
    m_incomingCustomParam(0),
    m_incomingCustomParam2(0),
    m_incomingDataBuffer(NULL),
    m_incomingDataBufferSize(0),
    m_incomingDataCounter(0),
    m_incomingDataSize(0),
    m_incomingCommandAccepted(false),
    m_incomingDataHandler(NULL),
    m_incomingTimeout(30000),
    m_incomingFletcherCRC(0),
    m_incomingFletcherCRC_sum1(0),
    m_incomingFletcherCRC_sum2(0),
    m_outgoingStateMachine(eidWaitForNewCommand),
    m_outgoingBusAddress(0),
    m_outgoingCommand(0),
    m_outgoingCustomParam(0),
    m_outgoingCustomParam2(0),
    m_outgoingDataBuffer(NULL),
    m_outgoingDataBufferSize(0),    
    m_outgoingFletcherCRC_sum1(0),
    m_outgoingFletcherCRC_sum2(0),
    m_outgoingDataHandler(NULL),
    m_outgoingDataCounter(0),
    m_outgoingTimeout(30000),
    m_outgoingDataSending(false),
    m_lastError(peSuccess),
    m_callbackSendByte(NULL),
    m_callbackHeaderReceived(NULL),
    m_callbackPacketError(NULL),
    m_callbackCommandReceived(NULL),
    m_callbacCommandSent(NULL)
{
     
}

void TSimpleBinaryProtocol::SetDataMode(eSRDataMode dataMode)
{
    m_incomingDataMode = dataMode;   
}

void TSimpleBinaryProtocol::SetDataBuffer(unsigned char* incomingDataBuffer, unsigned long incomingDataBufferSize)
{
	m_incomingDataBuffer     = incomingDataBuffer;
	m_incomingDataBufferSize = incomingDataBufferSize;
	m_incomingDataHandler    = NULL;
	m_outgoingDataHandler    = NULL;
}

void TSimpleBinaryProtocol::SetOnFlyDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler)
{
	m_incomingDataBuffer     = NULL;
	m_incomingDataBufferSize = 0;
	m_incomingDataHandler    = incomingDataHandler;
	m_outgoingDataHandler    = outgoingDataHandler;
}

void TSimpleBinaryProtocol::SetEventHandlers(SendByteCallback sendByteHandler, PacketCompleteCallback packetReceivedHandler, PacketCompleteCallback packetErrorHandler, PacketCompleteCallback packetSentHandler)
{
	m_callbackSendByte       = sendByteHandler;
	m_callbackCommandReceived = packetReceivedHandler;
	m_callbackPacketError    = packetErrorHandler;
	m_callbacCommandSent     = packetSentHandler;
}

void TSimpleBinaryProtocol::SetLastError(eProtocolError err)
{
    m_lastError = err;
}
             
void TSimpleBinaryProtocol::SetBusAddress(unsigned short deviceAddress)
{
    m_busAddress = deviceAddress;
}


void TSimpleBinaryProtocol::OnByteReceived(unsigned char c)
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
                m_incomingFletcherCRC = 0;
                m_incomingFletcherCRC_sum1 = 0;
                m_incomingFletcherCRC_sum2 = 0;
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
            }
            break;

        case eidBusAddressLow: 
            {
                m_incomingBusAddress = c;        
                m_incomingStateMachine = eidBusAddressHigh; 
            }
            break;

        case eidBusAddressHigh: 
            {
                m_incomingBusAddress += (c<<8); 
                m_incomingStateMachine = eidCommandLow;
            }
            break;

        case eidCommandLow:
            {
                m_incomingCommand = c;
                m_incomingStateMachine = eidCommandHigh;
            }
            break;

        case eidCommandHigh:
            {
                m_incomingCommand += (c<<8);
                m_incomingStateMachine = eidCustomParamLow;
            }
            break;

        case eidCustomParamLow:
            {
                m_incomingCustomParam = (unsigned char)c;
                m_incomingStateMachine = eidCustomParamHigh;
            }
            break;

        case eidCustomParamHigh:
            {
                m_incomingCustomParam += (unsigned char)(c>>8);
                m_incomingStateMachine = eidCustomParam2Low;
            }
            break;

        case eidCustomParam2Low:
            {
                m_incomingCustomParam2 = (unsigned char)c;
                m_incomingStateMachine = eidCustomParam2High;
            }
            break;

        case eidCustomParam2High:
            {
                m_incomingCustomParam2 += (unsigned char)(c>>8);
                m_incomingStateMachine = eidDataLengthLow;
            }
            break;

        case eidDataLengthLow:
            {
                m_incomingDataSize = c;
                m_incomingStateMachine = eidDataLengthHigh;
            }
            break;

        case eidDataLengthHigh:
            {
                m_incomingDataSize += (c<<8);
                m_incomingDataCounter = 0;
                m_incomingStateMachine = eidHeaderSumLow;
            }
            break;

        case eidHeaderSumLow:
            {
                m_incomingFletcherCRC = c;
                m_incomingStateMachine = eidHeaderSumHigh;
            }
            return;

        case eidHeaderSumHigh:
            {
                m_incomingFletcherCRC += (c<<8);                
                if (m_incomingFletcherCRC == GetIncomingFletcherCRC())
                {
					m_incomingCommandAccepted = m_incomingBusAddress == m_busAddress;
					if (m_callbackHeaderReceived)
					{
						m_callbackHeaderReceived(m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingCommandAccepted);
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
									m_callbackPacketError(m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
								}
								m_incomingCommandAccepted = false;
							}
						}
                    }
                } else {
					m_incomingStateMachine = eidMagicByte1;
                    SetLastError(peInvalidHeaderCRC);                    
					if (m_callbackPacketError)
					{
						m_callbackPacketError(m_incomingBusAddress, m_incomingCommand & 0x3FFF, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataBufferSize, m_incomingDataBuffer);
					}
                }            
            }
            m_incomingFletcherCRC = 0;
            m_incomingFletcherCRC_sum1 = 0;
            m_incomingFletcherCRC_sum2 = 0;
            return;

        case eidData:
            {              
				if (m_incomingCommandAccepted)
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
                m_incomingFletcherCRC = c;
                m_incomingStateMachine = eidDataSumHigh;
            }
            return;

        case eidDataSumHigh:
            {			
				m_incomingStateMachine = eidMagicByte1;
                m_incomingFletcherCRC+= c<<8;
				if (m_incomingCommandAccepted)
				{					
					if (m_incomingFletcherCRC==GetIncomingFletcherCRC())
					{
                        m_incomingStateMachine = eidWaitForLastByte;
                        OnByteReceived(0);
					} else
					{						
						if (m_callbackPacketError)
						{
							m_callbackPacketError(m_incomingBusAddress, m_incomingCommand & 0x3FFF, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
						}						
					}
				}	               
			}
            return;

        case eidWaitForLastByte:
            {
                if (m_incomingCommandAccepted)
                {
                    if (m_callbackCommandReceived)
                    {
                        m_callbackCommandReceived(m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
                    }
                }
                m_incomingStateMachine = eidMagicByte1;
            }
            return;
    }
    m_incomingFletcherCRC_sum1 = (m_incomingFletcherCRC_sum1 + c) % 255;
    m_incomingFletcherCRC_sum2 = (m_incomingFletcherCRC_sum2 + m_incomingFletcherCRC_sum1) % 255;
}

unsigned short TSimpleBinaryProtocol::GetIncomingFletcherCRC()
{
    return m_incomingFletcherCRC_sum1 + (m_incomingFletcherCRC_sum2<<8);
}

void TSimpleBinaryProtocol::SendByte(unsigned char c)
{
    if (m_callbackSendByte) 
    {
        m_callbackSendByte(c);
    }
}

void TSimpleBinaryProtocol::OnByteSent()
{
    unsigned char c = 0;
    switch (m_outgoingStateMachine)
    {
        case eidMagicByte1:
            {
                c = SRPROTOCOL_MAGIC_BYTE_1;
                SendByte(c);
                m_outgoingFletcherCRC_sum1 = 0;
                m_outgoingFletcherCRC_sum2 = 0;
                m_outgoingTimeout = 0;                
                m_outgoingStateMachine = eidMagicByte2;
            }
            break;

        case eidMagicByte2:
            {
                c = SRPROTOCOL_MAGIC_BYTE_2;
                SendByte(c);
                m_outgoingStateMachine = eidBusAddressLow;
            }
            break;

        case eidBusAddressLow: 
            {
                c = (unsigned char)m_outgoingBusAddress;
                SendByte(c);    
                m_outgoingStateMachine = eidBusAddressHigh; 
            }
            break;

        case eidBusAddressHigh: 
            {
                c = (unsigned char)(m_outgoingBusAddress>>8);
                SendByte(c);
                m_outgoingStateMachine = eidCommandLow;
            }
            break;

        case eidCommandLow:
            {
                c = (unsigned char)m_outgoingCommand;
                SendByte(c);
                m_outgoingStateMachine = eidCommandHigh;
            }
            break;

        case eidCommandHigh:
            {
                c = (unsigned char)(m_outgoingCommand>>8);                    
                SendByte(c);
                m_outgoingStateMachine = eidCustomParamLow;
            }
            break;

        case eidCustomParamLow:
            {
                c = (unsigned char)m_outgoingCustomParam;
                SendByte(c);
                m_outgoingStateMachine = eidCustomParamHigh;
            }
            break;

        case eidCustomParamHigh:
            {
                c = (unsigned char)(m_outgoingCustomParam>>8);
                SendByte(c);
                m_outgoingStateMachine = eidCustomParam2Low;
            }
            break;

        case eidCustomParam2Low:
            {
                c = (unsigned char)m_outgoingCustomParam2;
                SendByte(c);
                m_outgoingStateMachine = eidCustomParam2High;
            }
            break;

        case eidCustomParam2High:
            {
                c = (unsigned char)(m_outgoingCustomParam2>>8);
                SendByte(c);
                m_outgoingStateMachine = eidDataLengthLow;                
            }
            break;

        case eidDataLengthLow:
            {
                c = (unsigned char)(m_outgoingDataBufferSize);                   
                SendByte(c);
                m_outgoingStateMachine = eidDataLengthHigh;
            }
            break;

        case eidDataLengthHigh:
            {
                c = (unsigned char)(m_outgoingDataBufferSize>>8);                   
                SendByte(c);
                m_outgoingStateMachine = eidHeaderSumLow;
            }
            break;

        case eidHeaderSumLow:
            {                
                c = (unsigned char)m_outgoingFletcherCRC_sum1;
                SendByte(c);
                m_outgoingStateMachine = eidHeaderSumHigh;
            }
            return;

        case eidHeaderSumHigh:
            {
                c = (unsigned char)m_outgoingFletcherCRC_sum2;
                SendByte(c);

                m_outgoingDataCounter = 0;
                if (m_outgoingDataBufferSize==0)
                {
                    m_outgoingStateMachine = eidWaitForLastByte;
                } else {
                    m_outgoingStateMachine = eidData;                    
                } 
            }
            m_outgoingFletcherCRC_sum1 = 0;
            m_outgoingFletcherCRC_sum2 = 0;
            return;

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
				SendByte(c);

                m_outgoingDataCounter++;
                if (m_outgoingDataCounter==m_outgoingDataBufferSize)
                {
                    m_outgoingStateMachine = eidDataSumLow;
                }
            }
            break;

        case eidDataSumLow:
            {
                c = (unsigned char)m_outgoingFletcherCRC_sum1;
                SendByte(c);
                m_outgoingStateMachine = eidDataSumHigh;
            }
            return;

        case eidDataSumHigh:
            {
                c = (unsigned char)m_outgoingFletcherCRC_sum2;
                SendByte(c);
                m_outgoingStateMachine = eidWaitForLastByte;
            }
            return;

        case eidWaitForLastByte:
            {
                m_outgoingDataSending = false;
                m_outgoingStateMachine = eidWaitForNewCommand;
                if (m_callbacCommandSent)
                {
                    m_callbacCommandSent(m_outgoingBusAddress, m_outgoingCommand, m_outgoingCustomParam, m_outgoingCustomParam2, m_outgoingDataBufferSize, m_outgoingDataBuffer);
                }
            }
            return;

        case eidWaitForNewCommand:
            {
                //do nothing
            }
            return;
    }
    m_outgoingFletcherCRC_sum1  = (m_outgoingFletcherCRC_sum1 + c) % 255;
    m_outgoingFletcherCRC_sum2  = (m_outgoingFletcherCRC_sum2 + m_outgoingFletcherCRC_sum1) % 255;
}

unsigned short TSimpleBinaryProtocol::GetOutgoingFletcherCRC()
{
    return m_outgoingFletcherCRC_sum1 + (m_outgoingFletcherCRC_sum2<<8);
}


void TSimpleBinaryProtocol::OnTimer(unsigned short intervalMS)
{
    m_incomingTimeout++;
    if (m_incomingTimeout==SRPROTOCOL_TIMEOUT)
    {
        m_incomingStateMachine = eidMagicByte1;
    }
}

bool TSimpleBinaryProtocol::SendPacket (unsigned short busAddress, short command, short customParam1, short customParam2, unsigned long dataSize, unsigned char* data )
{
    if (m_outgoingDataSending)
    {
        return false;
    }
    m_outgoingDataSending    = true;
    m_outgoingBusAddress     = busAddress;
    m_outgoingCommand        = command;
    m_outgoingCustomParam    = customParam1;
    m_outgoingCustomParam2   = customParam2;
    m_outgoingDataBufferSize = dataSize;
    m_outgoingDataBuffer     = data;
    m_outgoingStateMachine   = eidMagicByte1;
    OnByteSent();
    return true;
}

eProtocolError TSimpleBinaryProtocol::GetLastError()
{
    return m_lastError;
}

void TSimpleBinaryProtocol::SendPing(unsigned short busAddress)
{
    SendPacket(busAddress, SRPROTOCOL_PING, 0, 0, 0, NULL);
}

void TSimpleBinaryProtocol::SendCommand(unsigned short busAddress, short command, unsigned char* pData, unsigned short dataSize)
{
    SendPacket(busAddress, command, 0, 0, dataSize, pData);
}

void TSimpleBinaryProtocol::SendCommandEx(unsigned short busAddress, short command, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize )
{
    SendPacket(busAddress, command, customParam1, customParam2, dataSize, pData);
}





