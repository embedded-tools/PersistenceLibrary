/*
 * Persistence Library / Protocols / TBinaryProtocol
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

#include "TBinaryProtocol.h"
#include <string.h>

extern "C"
{
    #include "Sha1.h"
}


TBinaryProtocol::TBinaryProtocol(bool loginRequiredFirst)
   :
    m_magicByte1('P'),
	m_magicByte2('l'),
	m_packetCounter(0),
    m_busAddress(0),
	m_sleepCallback(NULL),
	m_sendPacketCallback(NULL),
	m_inputBuffer(NULL),
	m_inputBufferSize(0),
	m_inputBufferPos(0),
	m_outputBuffer(NULL),
	m_outputBufferSize(0),
	m_outputBufferPos(0),
    m_loginComplete(!loginRequiredFirst),
    m_userManager(NULL)
{
     
}

void TBinaryProtocol::SetOutputBuffer(unsigned char* outputPacketBuffer, unsigned short outputPacketSize)
{
	m_outputBuffer     = outputPacketBuffer;
	m_outputBufferSize = outputPacketSize;
	m_outputBufferPos  = 0;	
}

void TBinaryProtocol::SetInputBuffer(unsigned char* inputPacketBuffer, unsigned short inputPacketSize)
{
	m_inputBuffer     = inputPacketBuffer;
	m_inputBufferSize = inputPacketSize;
	m_inputBufferPos  = 0;
}

void TBinaryProtocol::SetSleepCallback(SleepCallback sleepFunction)
{
	m_sleepCallback = sleepFunction;
}

void TBinaryProtocol::SetSendPacketCallback(SendPacketCallback sendPacketFunction)
{
	m_sendPacketCallback = sendPacketFunction;
}

void TBinaryProtocol::ClearOutputBuffer()
{
	m_outputBufferPos = 0;
}

void TBinaryProtocol::ClearInputBuffer()
{
	m_inputBufferPos = 0;
}

void TBinaryProtocol::DeleteFromInputBuffer(unsigned short dataLength)
{
	if (dataLength > m_inputBufferPos)
	{
		dataLength = m_inputBufferPos;
	}
	if (dataLength > 0)
	{
		memcpy(m_inputBuffer, m_inputBuffer + dataLength, m_inputBufferPos - dataLength);
	}
	m_inputBufferPos -= dataLength;
	if (m_inputBufferPos >= 2)
	{
		if ((m_inputBuffer[0] != m_magicByte1) || (m_inputBuffer[1] != m_magicByte2))
		{
			m_inputBufferPos = 0;
		}
	}
}

void TBinaryProtocol::SetBusAddress(unsigned short busAddress)
{
	m_busAddress = busAddress;
}

void TBinaryProtocol::DataReceived(unsigned char* data, unsigned short dataLength)
{
	if (m_inputBufferPos == 1)
	{
		if (data[0] != m_magicByte2)
		{
			m_inputBufferPos = 0;
		}
	}	
	if (m_inputBufferPos == 0)
	{			
		//look for magic byte 1 and magic byte 2
		for(unsigned char i = 0; i < dataLength; i++)
		{
			if (data[i] == m_magicByte1)
			{
				unsigned short byteCount = dataLength - i;
				if (byteCount >= m_inputBufferSize)
				{
					byteCount = m_inputBufferSize;
				}
				if (byteCount == 1)
				{
					memcpy(m_inputBuffer + m_inputBufferPos, data + i, byteCount);
					m_inputBufferPos += byteCount;
					break;
				} 
				else 
				{
					if (data[i+1] == m_magicByte2)
					{
						//both magicbyte1 and magicbyte2 were found
						memcpy(m_inputBuffer + m_inputBufferPos, data + i, byteCount);
						m_inputBufferPos += byteCount;
						break;
					}
				}
			}
		}
	} 
	else 
	{
		if ((m_inputBufferPos + dataLength) > m_inputBufferSize)
		{
			dataLength = m_inputBufferSize - m_inputBufferPos;
		}
		if (dataLength > 0)
		{		
			memcpy(m_inputBuffer + m_inputBufferPos, data, dataLength);
			m_inputBufferPos += dataLength;
		}		
	}	
}

TResponse TBinaryProtocol::WaitForSpecificResponse(const TRequest request,											  
		  									       unsigned short timeOut)
{
	return WaitForResponse(timeOut,
						   request.packetId);
}

TResponse TBinaryProtocol::WaitForResponse(unsigned short timeOut,
									       unsigned short expectedPacketId)
{
	TResponse response;
	memset(&response, 0, sizeof(response));

	if ((m_inputBuffer == NULL) || (m_busAddress == 0))
	{
		response.error = crClassNotInitialized;
		return response;
	}
	if (m_inputBufferSize < 20)
	{
		response.error = crBufferTooSmall;
		return response;
	}
	
	unsigned short expectedLength = 14;
	bool waitForBinaryData = false;
	bool packetComplete = false;	
	unsigned short expectedCRC    = 0;

	if (m_sleepCallback)
	{
		response.error = crTimeout;
		while(timeOut)
		{
			if (m_inputBufferPos >= expectedLength)
			{
				if (!waitForBinaryData)
				{				
					response.packetId      = m_inputBuffer[2] + (m_inputBuffer[3] << 8);
					response.senderAddress = m_inputBuffer[4] + (m_inputBuffer[5] << 8);
					response.targetAddress = m_inputBuffer[6] + (m_inputBuffer[7] << 8);
					unsigned short w       = m_inputBuffer[8] + (m_inputBuffer[9] << 8);					
					if ((w & 0x8000) == 0x8000)
					{
						response.response = (ECommandResult)(w & 0x7FFF);
						response.dataLength    = m_inputBuffer[10] + (m_inputBuffer[11] << 8);
						if (response.dataLength > 0)
						{
							waitForBinaryData = true;
							expectedLength = response.dataLength + 14;
						} 
						else 
						{
							packetComplete = true;
							expectedCRC = m_inputBuffer[12] + (m_inputBuffer[13] << 8);
						}
					} 
					else 
					{						
						if (expectedLength < 20)
						{
							expectedLength = response.dataLength + 20;
							continue;
						}
						//echoed command will be ignored and deleted from input buffer
						unsigned short dataLength = m_inputBuffer[16] + (m_inputBuffer[17] << 8);
						DeleteFromInputBuffer(dataLength + 20);
						continue;
					}
				} 
				else 
				{
					packetComplete = true;
					expectedCRC = m_inputBuffer[response.dataLength + 12] + (m_inputBuffer[response.dataLength + 13] << 8);
				}
			}
			if (packetComplete)
			{
				unsigned short totalBytes = response.dataLength + 12;
				unsigned short sum1 = 0;
				unsigned short sum2 = 0;
				for (unsigned short i = 0; i <  totalBytes; i++)
				{
					sum1 = (sum1 + m_inputBuffer[i]) % 255;
					sum2 = (sum2 + sum1) % 255;
				}		
				unsigned short crc = sum2 * 256 + sum1;
				if (crc == expectedCRC)
				{
					if (!expectedPacketId)
					{
						response.packetId = expectedPacketId;
					}
					if (response.packetId == expectedPacketId)
					{
						if (response.dataLength > 0)
						{
							response.data = m_inputBuffer + 12;
						}
						if (response.targetAddress == m_busAddress)
						{
							response.error = crSuccess;
							break;
						}
						response.error = crAddressMismatch;
						break;
					} 
					else 
					{
						response.error = crInvalidCRC;
						return response;
					}					
				}				
			}			
			if (m_sleepCallback)
			{
				m_sleepCallback(1);
			}
			timeOut--;
		}
	}
	return response;
}

TRequest TBinaryProtocol::WaitForRequest (unsigned short timeOut)
{
	TRequest request;
	memset(&request, 0, sizeof(request));
	request.error = crClassNotInitialized;

	if ((m_inputBuffer == NULL) || (m_busAddress == 0))
	{
		return request;		
	}
	if (m_inputBufferSize < 20)
	{
		request.error = crBufferTooSmall;
		return request;
	}

	unsigned short expectedLength = 20;
	bool waitForBinaryData = false;
	bool packetComplete = false;	
	unsigned short expectedCRC    = 0;

	if (m_sleepCallback)
	{
		request.error = crTimeout;
		while(timeOut)
		{
			if (m_inputBufferPos >= expectedLength)
			{
				if (!waitForBinaryData)
				{	
					request.packetId      = m_inputBuffer[2] + (m_inputBuffer[3] << 8);
					request.senderAddress = m_inputBuffer[4] + (m_inputBuffer[5] << 8);					
					request.targetAddress = m_inputBuffer[6] + (m_inputBuffer[7] << 8);
					unsigned short w      = m_inputBuffer[8] + (m_inputBuffer[9] << 8);
					if ((w & 0x8000) == 0)
					{
						request.command = (ECommand) w;
						request.customParam1  = m_inputBuffer[10] + (m_inputBuffer[11] << 8);
						request.customParam2  = m_inputBuffer[12] + (m_inputBuffer[13] << 8);
						request.customParam3  = m_inputBuffer[14] + (m_inputBuffer[15] << 8);									
						request.dataLength    = m_inputBuffer[16] + (m_inputBuffer[17] << 8);
					} 
					else 
					{
						request.error = crInvalidRequest;
						return request;
					}
					if (request.dataLength > 0)
					{
						waitForBinaryData = true;
						expectedLength = request.dataLength + 20;
						if (expectedLength > m_inputBufferSize)
						{
							request.error = crBufferTooSmall;
							return request;
						}
					} 
					else 
					{                                                                                                                                                                                                                                                                                                                                                          						packetComplete = true;
						expectedCRC = m_inputBuffer[18] + (m_inputBuffer[19] << 8);
						packetComplete = true;
					}
				} 
				else 
				{					
					expectedCRC = m_inputBuffer[request.dataLength + 18] + (m_inputBuffer[request.dataLength + 19] << 8);
					packetComplete = true;
					break;
				}
			}
			if (packetComplete)
			{
				unsigned short totalBytes = request.dataLength + 18;
				unsigned short sum1 = 0;
				unsigned short sum2 = 0;
				for (unsigned short i = 0; i <  totalBytes; i++)
				{
					sum1 = (sum1 + m_inputBuffer[i]) % 255;
					sum2 = (sum2 + sum1) % 255;
				}		
				unsigned short crc = sum2 * 256 + sum1;
				if (crc == expectedCRC)
				{
					if (request.targetAddress == m_busAddress)
					{
						if ((request.command & 0x8000) == 0)
						{
							request.error = crSuccess;
						}
					} 
					else 
					{
						request.error = crAddressMismatch;
					}
					break;
				}
				request.error = crInvalidCRC;
				break;
			}
			if (m_sleepCallback)
			{
				m_sleepCallback(1);
			}
			timeOut--;
		}
	}
	return request;	
}


TRequest TBinaryProtocol::SendCommand(unsigned short deviceAddress,
                                      ECommand command, 
                                      unsigned short param1,
								      unsigned short param2,
								      unsigned short param3,
								      unsigned short dataLength,
								      unsigned char* data)
{
	ClearInputBuffer();

	TRequest request;
	memset(&request, 0, sizeof(request));

    if ((!m_loginComplete) && (command != bcLogin))
    {
        request.error = crLoginRequired;
        return request;
    }

	if ((m_outputBuffer == NULL) || (m_busAddress == 0) || (m_sendPacketCallback == NULL))
	{
		request.error = crClassNotInitialized;
		return request;
	}
	if (m_outputBufferSize < (dataLength + 20))
	{
		request.error = crBufferTooSmall;
		return request;
	}

	m_outputBufferPos = 0;

	m_packetCounter++;
	if (m_packetCounter == 0)
	{
		m_packetCounter = 0;
	}

	//packet header
	m_outputBuffer[m_outputBufferPos++] = m_magicByte1;
	m_outputBuffer[m_outputBufferPos++] = m_magicByte2;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) m_packetCounter;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (m_packetCounter >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) m_busAddress;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (m_busAddress >> 8);	
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) deviceAddress;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (deviceAddress >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) command;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (command >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) param1;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (param1 >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) param2;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (param2 >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) param3;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (param3 >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) dataLength;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (dataLength >> 8);
	
	//data
	memcpy(m_outputBuffer + m_outputBufferPos, data, dataLength);	
	m_outputBufferPos += dataLength;
	
    //fletcher 16 check sum
	unsigned short sum1 = 0;
	unsigned short sum2 = 0;
	for (unsigned short i = 0; i <  dataLength + 18; i++)
	{
		sum1 = (sum1 + m_outputBuffer[i]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}		
	unsigned short crc = sum2 * 256 + sum1;
	
	//crc
	m_outputBuffer[m_outputBufferPos++] = (unsigned char)crc;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char)(crc >> 8);	

	//send data
	if (m_sendPacketCallback)
	{
		m_sendPacketCallback(m_outputBuffer, m_outputBufferPos);

		request.packetId = m_packetCounter;
		request.senderAddress = m_busAddress;
		request.targetAddress = deviceAddress;
		request.command = command;
		request.customParam1 = param1;
		request.customParam2 = param2;
		request.customParam3 = param3;
		request.dataLength = dataLength;
		request.data = data;
		request.error = crSuccess;
	}
	return request;
}

TResponse TBinaryProtocol::SendResponse(TRequest request,
                                        ECommandResult response, 
 								        unsigned short dataLength,
								        unsigned char* data)
{
	TResponse result;
	memset(&result, 0, sizeof(result));

	if ((m_outputBuffer == NULL) || (m_busAddress == 0) || (m_sendPacketCallback == NULL))
	{
		result.error = crClassNotInitialized;
		return result;
	}
	if ((dataLength + 14) > m_outputBufferSize)
	{
		result.error = crBufferTooSmall;
		return result;
	}

	ClearOutputBuffer();
	
	//packet header
	m_outputBuffer[m_outputBufferPos++] = m_magicByte1;
	m_outputBuffer[m_outputBufferPos++] = m_magicByte2;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) request.packetId;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (request.packetId >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) m_busAddress;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (m_busAddress >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) request.senderAddress;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (request.senderAddress >> 8);
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) response;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (response >> 8) | 0x80;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) dataLength;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char) (dataLength >> 8);
	
	//data
	if (dataLength > 0)
	{
		memcpy(m_outputBuffer + m_outputBufferPos, data, dataLength);	
		m_outputBufferPos += dataLength;
	}
	
	unsigned short sum1 = 0;
	unsigned short sum2 = 0;
	for (unsigned short i = 0; i <  dataLength + 12; i++)
	{
		sum1 = (sum1 + m_outputBuffer[i]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}		
	unsigned short crc = sum2 * 256 + sum1;
	
	//crc
	m_outputBuffer[m_outputBufferPos++] = (unsigned char)crc;
	m_outputBuffer[m_outputBufferPos++] = (unsigned char)(crc >> 8);
	
	//send data
	if (m_sendPacketCallback)
	{
		m_sendPacketCallback(m_outputBuffer, m_outputBufferPos);

		result.packetId = m_packetCounter;
		result.senderAddress = m_busAddress;
		result.targetAddress = request.senderAddress;
		result.response = response;
		result.dataLength = dataLength;
		result.data = data;
		result.error = crSuccess;
	}
	return result;
}

bool TBinaryProtocol::CalculateHash(const char* userName, const char* password, unsigned char* nonce, unsigned short nonceLength, unsigned char* out_hash)
{
    static const int placeHolder = 8;            
    static const int maximumPossibleNonceLength = 160;
    static char tmpBuffer[TUserManager::MaximumUserLength + 
                          TUserManager::MaximumPasswordLength + 
                          maximumPossibleNonceLength +
                          placeHolder];
    int tmpBufferPosition = 0;
    int userNameLength = strlen(userName);
    int passwordLength = strlen(password);

    if (nonceLength > maximumPossibleNonceLength)
    {
        return false;
    }

    memcpy(tmpBuffer + tmpBufferPosition, userName, userNameLength);
    tmpBufferPosition += userNameLength;
    tmpBuffer[tmpBufferPosition++] = ':';

    memcpy(tmpBuffer + tmpBufferPosition, password, passwordLength);
    tmpBufferPosition += passwordLength;
    tmpBuffer[tmpBufferPosition++] = ':';

    memcpy(tmpBuffer + tmpBufferPosition, nonce, nonceLength);
    tmpBufferPosition += nonceLength;

    Sha1(tmpBuffer, tmpBufferPosition, out_hash);
    return true;
}

bool TBinaryProtocol::Login(unsigned short deviceAddress, const char* userName, const char* password, int timeOut)
{
    //asks for nonce
    TRequest request = SendCommand(deviceAddress, bcLogin);
    if (request.error != crSuccess)
    {
        return false;
    }
    

    //wait for nonce
    TResponse response = WaitForSpecificResponse(request, timeOut);
    if (response.error != crSuccess)
    {
        return false;
    }

    //calculate SHA1 hash of (UserName + ":" + Password + ":" + Nonce)
    unsigned char hash[20];
    CalculateHash(userName, password, response.data, response.dataLength, hash);

    //send calculated hash
    request = SendCommand(deviceAddress, bcLogin, 0, 0, 0, sizeof(hash), (unsigned char*) &hash[0]);
    if (request.error != crSuccess)
    {
        return false;
    }
    //receives response
    response = WaitForSpecificResponse(request, timeOut);

    //crSuccess means that username and password is valid
    m_loginComplete = response.error == crSuccess;
    if (m_loginComplete)
    {
        m_loginDeviceAddress = deviceAddress;
    } 
    else 
    {
        m_loginDeviceAddress = 0;
    }
    return m_loginComplete;
}

bool TBinaryProtocol::VerifyPassword (unsigned char* hash, unsigned short hashLength)
{
    unsigned char userhash[20];

    for(int i = 0; i < m_userManager->GetUserCount(); i++)
    {
        TUserManager::TUser user = m_userManager->GetUser(i);
        CalculateHash(user.userName, user.password, m_lastNonce, sizeof(m_lastNonce), userhash);
        if (memcmp(userhash, hash, hashLength) == 0)
        {
            return true;
        }
    }
    return false;
}

TRequest TBinaryProtocol::SendPing(unsigned short deviceAddress)
{
    return SendCommand(deviceAddress, bcPing);
}

TRequest TBinaryProtocol::LedGreenOn(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedGreenOn, ledNumber);
}

TRequest TBinaryProtocol::LedGreenOff(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedGreenOff, ledNumber);
}

TRequest TBinaryProtocol::LedGreenToggle(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedGreenToggle, ledNumber);
}

TRequest TBinaryProtocol::LedRedOn(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedRedOn, ledNumber);
}

TRequest TBinaryProtocol::LedRedOff(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedRedOff, ledNumber);
}

TRequest TBinaryProtocol::LedRedToggle(unsigned short deviceAddress, unsigned short ledNumber)
{
	return SendCommand(deviceAddress, bcLedRedToggle, ledNumber);
}

TRequest TBinaryProtocol::OutputOn(unsigned short deviceAddress, unsigned short outputNumber)
{
	return SendCommand(deviceAddress, bcOutputOn, outputNumber);
}

TRequest TBinaryProtocol::OutputOff(unsigned short deviceAddress, unsigned short outputNumber)
{
	return SendCommand(deviceAddress, bcOutputOff, outputNumber);
}

TRequest TBinaryProtocol::OutputToggle(unsigned short deviceAddress, unsigned short outputNumber)
{
	return SendCommand(deviceAddress, bcOutputToggle, outputNumber);
}

TRequest TBinaryProtocol::Beep(unsigned short deviceAddress, unsigned short frequency, unsigned short length)
{
	return SendCommand(deviceAddress, bcBeep, frequency, length);
}

TRequest TBinaryProtocol::ReadConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length)
{
	return SendCommand(deviceAddress, bcReadConfiguration, address, length);
}

TRequest TBinaryProtocol::WriteConfiguration(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data)
{
	return SendCommand(deviceAddress, bcWriteConfiguration, address, length, 0, length, data);
}

TRequest TBinaryProtocol::ReadData(unsigned short deviceAddress, unsigned short address, unsigned short length)
{
	return SendCommand(deviceAddress, bcReadData, address, length, 0);
}

TRequest TBinaryProtocol::WriteData(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data)
{
	return SendCommand(deviceAddress, bcWriteData, address, length, 0, length, data);
}

TRequest TBinaryProtocol::ReadDataExternal(unsigned short deviceAddress, unsigned short address, unsigned short length)
{
	return SendCommand(deviceAddress, bcReadDataExternal, address, length, 0);
}

TRequest TBinaryProtocol::WriteDataExternal(unsigned short deviceAddress, unsigned short address, unsigned short length, unsigned char* data)
{
	return SendCommand(deviceAddress, bcWriteDataExternal, address, length, 0, length, data);
}

TRequest TBinaryProtocol::OpenPort(unsigned short deviceAddress, unsigned short portAddress)
{
	return SendCommand(deviceAddress, bcOpenPort, portAddress);
}

TRequest TBinaryProtocol::ClosePort(unsigned short deviceAddress, unsigned short portAddress)
{
	return SendCommand(deviceAddress, bcOpenPort, portAddress);
}

TRequest TBinaryProtocol::WriteToPort(unsigned short deviceAddress, unsigned short portAddress, unsigned char* data, unsigned short dataLength)
{
	return SendCommand(deviceAddress, bcWriteToPort, portAddress, dataLength, 0, dataLength, data);
}

TRequest TBinaryProtocol::ReadFromPort(unsigned short deviceAddress, unsigned short portAddress, unsigned short maxDataLength, unsigned short timeOut)
{
	return SendCommand(deviceAddress, bcReadFromPort, portAddress, maxDataLength, timeOut);
}

TRequest TBinaryProtocol::SendCustomCommand(unsigned short command, unsigned short deviceAddress, unsigned short customParam1, unsigned short customParam2, unsigned short customParam3, unsigned char* data, unsigned short dataLength)
{
	return SendCommand(deviceAddress, bcReadFromPort, customParam1, customParam2, customParam3, dataLength, data);
}

bool TBinaryProtocol::Logout(unsigned short deviceAddress, int timeOut)
{
    //asks for nonce
    TRequest request = SendCommand(deviceAddress, bcLogout);
    if (request.error != crSuccess)
    {
        return false;
    }
    //wait for nonce
    TResponse response = WaitForSpecificResponse(request, timeOut);
    bool logoutSuccess = response.error == crSuccess;
    if (logoutSuccess)
    {
        m_loginComplete = false;
    }
    return logoutSuccess;
}


