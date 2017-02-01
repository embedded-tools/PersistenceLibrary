/*
 * Persistence Library / Protocols / TBinaryProtocol
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

TBinaryProtocol::TBinaryProtocol()
   :
    m_idleCounter(0),
    m_busAddress(0),
    m_incomingStateMachine(eidMagicByte1),
    m_incomingBusAddress(0),
    m_incomingCommand(0),
    m_incomingCustomParam(0),
    m_incomingCustomParam2(0),
    m_incomingDataSize(0),
    m_incomingDataMode(0),
    m_incomingDataHandler(NULL),
    m_incomingDataBuffer(NULL),
    m_incomingDataBufferSize(0),
    m_incomingDataCounter(0),
    m_incomingCommandAccepted(false),
    m_incomingTimeout(30000),
    m_incomingFletcherCRC(0),
    m_incomingFletcherCRC_sum1(0),
    m_incomingFletcherCRC_sum2(0),
    m_incomingPacketReady(false),
    m_incomingAcknowledge(false),
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
    m_callbackSleep(NULL),
    m_callbackHeaderReceived(NULL),
    m_callbackCommandReceived(NULL),
    m_callbackCommandReceivingError(NULL),    
    m_callbackCommandSent(NULL)
{
     
}

void TBinaryProtocol::SetDataMode(eSRDataMode dataMode)
{
    m_incomingDataMode = dataMode;   
}

void TBinaryProtocol::SetDataBuffer(unsigned char* incomingDataBuffer, unsigned short incomingDataBufferSize)
{
	m_incomingDataBuffer     = incomingDataBuffer;
	m_incomingDataBufferSize = incomingDataBufferSize;
	m_incomingDataHandler    = NULL;
	m_outgoingDataHandler    = NULL;
}

void TBinaryProtocol::SetOnFlyDataHandlers(WriteByteToStorage incomingDataHandler, ReadByteFromStorage outgoingDataHandler)
{
	m_incomingDataBuffer     = NULL;
	m_incomingDataBufferSize = 0;
	m_incomingDataHandler    = incomingDataHandler;
	m_outgoingDataHandler    = outgoingDataHandler;
}

void TBinaryProtocol::SetEventHandlers(SendByteCallback sendByteHandler, CommandReceivedCallback packetReceivedHandler, CommandReceivedCallback packetErrorHandler, CommandReceivedCallback packetSentHandler)
{
	m_callbackSendByte       = sendByteHandler;
	m_callbackCommandReceived = packetReceivedHandler;
	m_callbackCommandReceivingError    = packetErrorHandler;
	m_callbackCommandSent     = packetSentHandler;
}

void TBinaryProtocol::SetLastError(eProtocolError err)
{
    m_lastError = err;
}
             
void TBinaryProtocol::SetBusAddress(unsigned short deviceAddress)
{
    m_busAddress = deviceAddress;
}


void TBinaryProtocol::OnByteReceived(unsigned char c)
{
    switch (m_incomingStateMachine)
    {
        case eidMagicByte1:
            {
                m_incomingTimeout = 0;
                if (c==BINPROTOCOL_MAGIC_BYTE_1) 
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
                if (c==BINPROTOCOL_MAGIC_BYTE_2)
                {
                    m_incomingStateMachine = eidCommandLow;
                } else {
                    m_incomingStateMachine = eidMagicByte1;
                }
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
                m_incomingStateMachine = eidBusAddressLow;
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
                m_incomingCustomParam += (unsigned short)(c<<8);
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
                m_incomingCustomParam2 += (unsigned short)(c<<8);
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
                    if (m_incomingCommand & 0x8000) 
                    {   
                        //acknowledge packet is always accepted
                        m_incomingCommandAccepted = true;
                    } else {
                        //command packet is accepted if command targets to this device
                        m_incomingCommandAccepted = m_incomingBusAddress == m_busAddress;
					    if (m_callbackHeaderReceived)
					    {
						    m_callbackHeaderReceived(m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingCommandAccepted);
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
								if (m_callbackCommandReceivingError)
								{
									m_callbackCommandReceivingError(this, m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
								}
								m_incomingCommandAccepted = false;
							}
						}
                    }
                } else {
					m_incomingStateMachine = eidMagicByte1;
                    SetLastError(peInvalidHeaderCRC);                    
					if (m_callbackCommandReceivingError)
					{
						m_callbackCommandReceivingError(this, m_incomingBusAddress, m_incomingCommand & 0x3FFF, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataBufferSize, m_incomingDataBuffer);
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
						if (m_callbackCommandReceivingError)
						{
							m_callbackCommandReceivingError(this, m_incomingBusAddress, m_incomingCommand & 0x3FFF, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
						}						
					}
				}	               
			}
            return;

        case eidWaitForLastByte:
            {
                if (m_incomingCommandAccepted)
                {
                    if (m_incomingCommand & 0x8000)
                    {
                        m_incomingAcknowledge = true;
                    }
                    if (m_callbackCommandReceived)
                    {
                        m_callbackCommandReceived(this, m_incomingBusAddress, m_incomingCommand, m_incomingCustomParam, m_incomingCustomParam2, m_incomingDataSize, m_incomingDataBuffer);
                    }
                }
                m_incomingStateMachine = eidMagicByte1;
            }
            return;
    }
    m_incomingFletcherCRC_sum1 = (m_incomingFletcherCRC_sum1 + c) % 255;
    m_incomingFletcherCRC_sum2 = (m_incomingFletcherCRC_sum2 + m_incomingFletcherCRC_sum1) % 255;
}

unsigned short TBinaryProtocol::GetIncomingFletcherCRC()
{
    return m_incomingFletcherCRC_sum1 + (m_incomingFletcherCRC_sum2<<8);
}

void TBinaryProtocol::SendByte(unsigned char c)
{
    if (m_callbackSendByte) 
    {
        m_callbackSendByte(c);
    }
}

void TBinaryProtocol::OnByteSent()
{
    unsigned char c = 0;
    switch (m_outgoingStateMachine)
    {
        case eidMagicByte1:
            {
                c = BINPROTOCOL_MAGIC_BYTE_1;
                SendByte(c);
                m_outgoingFletcherCRC_sum1 = 0;
                m_outgoingFletcherCRC_sum2 = 0;
                m_outgoingTimeout = 0;                
                m_outgoingStateMachine = eidMagicByte2;
            }
            break;

        case eidMagicByte2:
            {
                c = BINPROTOCOL_MAGIC_BYTE_2;
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
                if (m_callbackCommandSent)
                {
                    m_callbackCommandSent(this, m_outgoingBusAddress, m_outgoingCommand, m_outgoingCustomParam, m_outgoingCustomParam2, m_outgoingDataBufferSize, m_outgoingDataBuffer);
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

unsigned short TBinaryProtocol::GetOutgoingFletcherCRC()
{
    return m_outgoingFletcherCRC_sum1 + (m_outgoingFletcherCRC_sum2<<8);
}


void TBinaryProtocol::OnTimer(unsigned short intervalMS)
{
    m_incomingTimeout++;
    if (m_incomingTimeout==BINPROTOCOL_TIMEOUT)
    {
        m_incomingStateMachine = eidMagicByte1;
    }
}

bool TBinaryProtocol::SendPacket (unsigned short command, unsigned short busAddress, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data )
{
    if (m_outgoingDataSending)
    {
        return false;
    }
    m_incomingAcknowledge    = false;
    m_outgoingDataSending    = true;
    m_outgoingCommand        = command;
    m_outgoingBusAddress     = busAddress;
    m_outgoingCustomParam    = customParam1;
    m_outgoingCustomParam2   = customParam2;
    m_outgoingDataBufferSize = dataSize;
    m_outgoingDataBuffer     = data;
    m_outgoingStateMachine   = eidMagicByte1;
    OnByteSent();
    return true;
}

eProtocolError TBinaryProtocol::GetLastError()
{
    return m_lastError;
}

void TBinaryProtocol::SendCommand(unsigned short busAddress, unsigned short command, unsigned char* pData, unsigned short dataSize)
{
    SendPacket(command, busAddress, 0, 0, dataSize, pData);
}

void TBinaryProtocol::SendCommandEx(unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize )
{
    SendPacket(command, busAddress, customParam1, customParam2, dataSize, pData);
}

void TBinaryProtocol::SendCustomResponse(unsigned short command, unsigned short errorCode, short customParam1, short customParam2, unsigned char* pData, unsigned short dataSize)
{
     SendPacket(command | 0x8000, errorCode, customParam1, customParam2, dataSize, pData);
}

bool TBinaryProtocol::WaitForResponse(unsigned short command, unsigned char* pOutput, unsigned short* pOutputLength, unsigned short maxOutputLength)
{
    unsigned short timeout = m_incomingTimeout;
    while(timeout>0)
    {
        if (m_incomingAcknowledge) break;
        if (m_callbackSleep)
        {
            m_callbackSleep(5);
        }
        timeout -= 5;
    }
    m_lastError = (eProtocolError)(m_incomingCustomParam & 0x7FFF);
    return false;
}

bool TBinaryProtocol::SendPing(unsigned short deviceAddress)
{
    SendPacket(ecPing, deviceAddress, 0, 0, 0, NULL);
    return WaitForResponse(ecPing);
}

bool TBinaryProtocol::LedGreenOn(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedGreenOn, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedGreenOn);
}

bool TBinaryProtocol::LedGreenOff(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedGreenOff, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedGreenOff);
}

bool TBinaryProtocol::LedGreenToggle(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedGreenToggle, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedGreenToggle);
}

bool TBinaryProtocol::LedRedOn(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedRedOn, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedRedOn);
}

bool TBinaryProtocol::LedRedOff(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedRedOff, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedRedOff);
}

bool TBinaryProtocol::LedRedToggle(unsigned short deviceAddress, unsigned short ledNumber)
{
    SendPacket(ecLedRedToggle, deviceAddress, ledNumber, 0, 0, NULL);
    return WaitForResponse(ecLedRedToggle);
}

bool TBinaryProtocol::OutputOn(unsigned short deviceAddress, unsigned short outputNumber)
{
    SendPacket(ecOutputOn, deviceAddress, outputNumber, 0, 0, NULL);
    return WaitForResponse(ecOutputOn);
}

bool TBinaryProtocol::OutputOff(unsigned short deviceAddress, unsigned short outputNumber)
{
    SendPacket(ecOutputOff, deviceAddress, outputNumber, 0, 0, NULL);
    return WaitForResponse(ecOutputOff);
}

bool TBinaryProtocol::OutputToggle(unsigned short deviceAddress, unsigned short outputNumber)
{
    SendPacket(ecOutputToggle, deviceAddress, outputNumber, 0, 0, NULL);
    return WaitForResponse(ecOutputToggle);
}

bool TBinaryProtocol::Beep(unsigned short deviceAddress, unsigned short frequency, unsigned short length)
{
    SendPacket(ecBeep, deviceAddress, frequency, length, 0, NULL);
    return WaitForResponse(ecOutputToggle);
}

bool TBinaryProtocol::ReadConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput, unsigned char maxOutputLength)
{
    bool result;
    unsigned short realLength = 0;

    SendPacket(ecReadConfiguration, deviceAddress, address, length, 0, NULL);    
    result = WaitForResponse(ecReadConfiguration, pOutput, &realLength, maxOutputLength);
    if (realLength<length)
    {
        return false;
    }
    return result;
}

bool TBinaryProtocol::StoreConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pData)
{
    SendPacket(ecStoreConfiguration, deviceAddress, address, 0, length, pData);
    return WaitForResponse(ecStoreConfiguration);
}

bool TBinaryProtocol::ReadData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput, unsigned short maxOutputLength)
{
    bool result;
    unsigned short realLength = 0;

    SendPacket(ecReadData, deviceAddress, address, length, 0, NULL);
    result = WaitForResponse(ecReadData, pOutput, &realLength, maxOutputLength);
    if (realLength<length)
    {
        return false;
    }
    return result;
}

bool TBinaryProtocol::WriteData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* pOutput)
{
    SendPacket(ecWriteData, deviceAddress, address, 0, length, pOutput);
    return WaitForResponse(ecWriteData);

}

bool TBinaryProtocol::OpenPort(unsigned short deviceAddress, unsigned short portAddress)
{
    SendPacket(ecOpenPort, deviceAddress, portAddress, 0, 0, NULL);
    return WaitForResponse(ecOpenPort);
}

bool TBinaryProtocol::ClosePort(unsigned short deviceAddress, unsigned short portAddress)
{
    SendPacket(ecClosePort, deviceAddress, portAddress, 0, 0, NULL);
    return WaitForResponse(ecClosePort);
}

bool TBinaryProtocol::WriteToPort(unsigned short deviceAddress, unsigned char* pData, unsigned short dataLength)
{
    SendPacket(ecWriteToPort, deviceAddress, 0, 0, dataLength, pData);
    return WaitForResponse(ecWriteToPort);
}

bool TBinaryProtocol::ReadFromPort(unsigned short deviceAddress, unsigned short timeout, unsigned char* pOutput, unsigned short* pOutputLength, const unsigned short maxDataLength)
{
    SendPacket(ecReadFromPort, deviceAddress,  timeout, 0, 0, NULL);
    return WaitForResponse(ecReadFromPort, pOutput, pOutputLength, maxDataLength);    
}

bool TBinaryProtocol::SendCustomCommand(unsigned short command, unsigned short deviceAddress, unsigned short customParam1, unsigned short customParam2, 
                                        unsigned char* pInputData, unsigned short inputDataLength, 
                                        unsigned char* pOutputData, unsigned short* pOutputDataLength, unsigned short maxOutputDataLength)
{
    SendPacket(command, deviceAddress, customParam1, customParam2, inputDataLength, pInputData);
    return WaitForResponse(command, pOutputData, pOutputDataLength, maxOutputDataLength);
}




