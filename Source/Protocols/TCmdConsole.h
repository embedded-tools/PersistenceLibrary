/*
 * Persistence Library / Protocols / TCmdConsole
 *
 * Copyright (c) 2016-2022 Ondrej Sterba <osterba@atlas.cz>
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

#ifndef TCMDCONSOLE___H
#define TCMDCONSOLE___H

#include "TList.h"
#include "TUserManager.h"

typedef void (*ExecuteCommandCallback)(void* sender, const char* command, int commandLength);

struct TCmdDefinition
{
    char                   command[16];
    ExecuteCommandCallback callback;
};

enum TCmdConsoleState {ccsEnteringUserName, ccsEnteringPassword, ccsEnteringCommands, ccsTransitionState};

class TCmdConsole
{
	public:
	    typedef void (*TSendDataCallback)(const char* data, unsigned short dataLength);

    private:
		static const unsigned short CMD_LINE_LENGTH    = 240;
		static const unsigned short SEND_BUFFER_SIZE   = 84;
        static const unsigned short DEVICE_NAME_LENGTH = 24;
       

        TUserManager*     m_userManager;
		TSendDataCallback m_sendDataCallback;

		//registered command
		TList<TCmdDefinition> m_commands;

		TCmdConsoleState m_state;
		unsigned char m_userNamePos;
        unsigned char m_passwordPos;
        bool          m_logged;

        char          m_deviceName[DEVICE_NAME_LENGTH];
		char          m_userName [TUserManager::USER_NAME_LENGTH+1];		
		char          m_password [TUserManager::PASSWORD_LENGTH+1];

		//buffer for data displayed in console
		char  m_cmdLine   [CMD_LINE_LENGTH];
		//output buffer for sending data back to the connected client
		char  m_sendBuffer[SEND_BUFFER_SIZE];
		//position in console buffer
		unsigned short m_cmdLinePos;
		//position in output sending buffer
		unsigned short m_sendBufferPos;
        
		//clears console and command line buffer
		void Clear();

		//method for receiving keyboard input from remote client
		bool ReceiveChar(char c);

		//sends text to remotely connected client
		void RemotePrint(char c);

		//sends text to remotely connected client
		void RemotePrint(const char* text);
		void RemotePrint(const char* text, unsigned short textLength);

        //sends buffer content and clears buffer
        void RemotePrintFlush();

		void NewCommand();		
		void Execute();
		void PrintCommands();

    public:
        TCmdConsole(TSendDataCallback callback);

        //method binds usermanager if you want user to login first
        void BindUserManager(TUserManager* userManager);

		//method for registering commands - all commands need to be registered
		//before method ConnectionEstablished is called
		bool RegisterCommand(TCmdDefinition definition);

		//method for receiving data from keyboard of remote client
		bool ReceiveData(const char* data, unsigned short dataLength);

		//method should be called immediatelly after opening port
		//or establishing remote connection
		void ConnectionEstablished(const char* welcomeMessage);

        //returns true if user entered valid user name and password
        bool IsUserLogged();



};

#endif