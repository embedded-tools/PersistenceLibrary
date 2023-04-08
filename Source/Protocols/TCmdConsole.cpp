#include "TCmdConsole.h"
#include "TList.h"
#include <stdio.h>
#include <string.h>

TCmdConsole::TCmdConsole(TSendDataCallback callback)
:m_userManager(NULL),
 m_sendDataCallback(callback),
 m_state(ccsEnteringUserName),
 m_userNamePos(0),
 m_passwordPos(0),
 m_logged(false),
 m_cmdLinePos(0),
 m_sendBufferPos(0)
{
	memset(m_cmdLine, 0, sizeof(m_cmdLine));
	memset(m_sendBuffer, 0, sizeof(m_sendBuffer));
    memset(m_userName, 0, sizeof(m_userName));
    memset(m_password, 0, sizeof(m_password));
    strcpy(m_deviceName, "this");
}

void TCmdConsole::BindUserManager(TUserManager* userManager)
{
    m_userManager = userManager;
}

bool TCmdConsole::RegisterCommand(TCmdDefinition definition)
{
    return m_commands.Add(definition);
}

void TCmdConsole::ConnectionEstablished(const char* welcomeMessage)
{
	Clear();

	unsigned short len = (unsigned short)strlen(welcomeMessage);
	RemotePrint(welcomeMessage, len);

	if (welcomeMessage[len-1] != 10)
	{
		RemotePrint(13);
		RemotePrint(10);
	}

	if (m_userManager != NULL)
	{
        //login required
		m_state = ccsEnteringUserName;
		RemotePrint("Login required first.\r\n\r\nUsername: ");
		RemotePrintFlush();
	} 
	else 
	{
        RemotePrint("\r\n");
        strcpy(m_userName, "anonymous");
		m_state = ccsEnteringCommands;
        NewCommand();
	}
}

void TCmdConsole::NewCommand()
{
    m_sendBufferPos = 0;
    RemotePrint(m_userName);
    RemotePrint("@");
    RemotePrint(m_deviceName);
    RemotePrint(":~$ ");
    RemotePrintFlush();
}

bool TCmdConsole::IsUserLogged()
{
    return m_logged;
}

bool TCmdConsole::ReceiveData(const char* data, unsigned short dataLength)
{
    if (dataLength == 2)
    {
        if ((data[0] == 13) && (data[1] == 10))
        {
            //empty ACK message is not accepted
            return true;
        }
    }
    if (((unsigned char)data[0]) == 0xFF)
    {
        //telnet commands are not accepted
        return true;
    }

    bool result = true;
    for (unsigned short i = 0; i < dataLength; i++)
    {
        result &= ReceiveChar(data[i]);
        if (!result)
        {
            break;
        }
    }
    if (result)
    {
        result &= ReceiveChar(13);
    }
    return result;
}


bool TCmdConsole::ReceiveChar(char c)
{
	switch (m_state)
	{
		case ccsEnteringUserName:
		{
            if (c == 13)
            {                
                m_passwordPos = 0;
                RemotePrint("Password: ");
                RemotePrintFlush();
                m_state = ccsEnteringPassword;
                return false;
            } 
            else if (m_userNamePos < TUserManager::USER_NAME_LENGTH)
			{
				m_userName[m_userNamePos] = c;
				m_userNamePos++;
                return true;
			}
            return false;
		}
		break;

		case ccsEnteringPassword:
		{
            if (c == 13)
            {                
                if (m_userManager)
                {
                    if (m_userManager->CredentialsValid(m_userName, m_password))
                    {
                        m_logged = true;

                        RemotePrint("User logged.\r\n\r\n");

                        NewCommand();
                        m_state = ccsEnteringCommands;                        
                    }
                    else
                    {                        
                        RemotePrint("Password: ");
                        RemotePrintFlush();
                    }
                    m_passwordPos = 0;
                    memset(m_password, 0, sizeof(m_password));
                }
                return false;
            } 
            else if (m_passwordPos < TUserManager::PASSWORD_LENGTH)
			{
				m_password[m_passwordPos] = c;
				m_passwordPos++;
                return true;
			}
            return false;
		}
		break;

		case ccsEnteringCommands:
		{
            if (c == 13)
            {
                m_state = ccsTransitionState;
                Execute();
                NewCommand();
                m_state = ccsEnteringCommands;
                return false;
            }
            else            
			if (m_cmdLinePos < CMD_LINE_LENGTH)
			{
				m_cmdLine[m_cmdLinePos++] = c;
                return true;
            }
			return false;	
		}
		break;

        default:
        {
            return false;
        }
        break;
	}
}

void TCmdConsole::Clear()
{
    m_userNamePos = 0;
    m_passwordPos = 0;
    m_sendBufferPos = 0;
    memset(m_userName, 0, sizeof(m_userName));
    memset(m_password, 0, sizeof(m_password));
}

void TCmdConsole::Execute()
{
	for(unsigned short i = 0; m_commands.Count(); i++)
	{
		if (strcmp(m_commands[i].command, m_cmdLine) == 0)
		{
			if (m_commands[i].callback != NULL)
			{
				m_commands[i].callback(this, m_cmdLine, m_cmdLinePos);
			}
		}
	}
}

void TCmdConsole::RemotePrint(char c)
{
	if (m_sendBufferPos < SEND_BUFFER_SIZE)
	{
		m_sendBuffer[m_sendBufferPos++] = c;
	}
	if ((c==10) || (m_sendBufferPos == SEND_BUFFER_SIZE))
	{
		RemotePrintFlush();
	}
}

void TCmdConsole::RemotePrint(const char* text)
{
	unsigned short len = (unsigned short)strlen(text);
	RemotePrint(text, len);
}

void TCmdConsole::RemotePrint(const char* text, unsigned short textLength)
{	
	for(unsigned short i = 0; i < textLength; i++)
	{
		RemotePrint(text[i]);
	}
}

void TCmdConsole::RemotePrintFlush()
{
    if (m_sendDataCallback != NULL)
    {
        m_sendDataCallback(m_sendBuffer, m_sendBufferPos);
    }	
    m_sendBufferPos = 0;
}


void TCmdConsole::PrintCommands()
{
	unsigned short len = 0;
	unsigned short n = 0;

	bool cr_needed = false;
	for(unsigned short i = 0; i < m_commands.Count(); i++)
	{
		len = (unsigned short) strlen(m_commands[i].command);
		for(unsigned short j = 0; j <sizeof(m_commands[i].command); j++)
		{
			if (m_commands[i].command[j] >= 32)
			{
				RemotePrint(m_commands[i].command[j]);
			} else {
				RemotePrint(32);
			}
		}
		RemotePrint(32);
		n++;
		if ((n % 4) == 0)
		{
			RemotePrint(13);
			RemotePrint(10);
			cr_needed = false;
		} 
		else 
		{
			cr_needed = true;
		}
	}
	if (cr_needed)
	{
		RemotePrint(13);
		RemotePrint(10);
	}
}


