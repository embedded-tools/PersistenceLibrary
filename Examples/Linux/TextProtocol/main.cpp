#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "TcpClient_Linux.h"
#include "TTextProtocol.h"

TTextProtocol* textProtocol;

int commandCounter = 0;
bool terminated = false;

void DataReceived(TcpClient* sender, const char* data, int dataLength)
{
    TcpClient* client  = (TcpClient*) sender;
    textProtocol->PacketReceived(client, data, dataLength);    
}

void ShowEachCommand(void* sender, const char* command, int commandLength)
{   
    commandCounter++;
 
    char buf[256];
    sprintf(buf, "Command %i: %%.%is\n", commandCounter, commandLength);    
    printf(buf, command);
    
    TcpClient* client = (TcpClient*)sender;
    
    char response[256];    
    sprintf(response, "Command %i received.",  commandCounter);
    client->SendData(response);    
	
	if (strcmp(command, "exit")==0)
	{
		//exit = command to shut down client
		terminated = true;
	}
}

int main(int argc, char **argv)
{    
    TcpClient_Linux client;
	
    //textProtocol class finds command separators and shows each command    
    if (!client.OpenAsync("192.168.11.3", 4000, DataReceived))
	{
		printf("Cannot connect to server\n");
		return -1;
	}
	

    textProtocol = new TTextProtocol(64, '\n');
    textProtocol->OnCommandReceived = ShowEachCommand;
		
    
    //sends different commands
	//end of each commands is marked by LF
	//(you can use semicolon as a separator if you like)
    textProtocol->PacketReceived(&client, "G X10 Y20 Z30\n");
    textProtocol->PacketReceived(&client, "SHOW 1");
	textProtocol->PacketReceived(&client, ":1\n");
    textProtocol->PacketReceived(&client, "\'Hello th");
    textProtocol->PacketReceived(&client, "ere'\nLAST COMMAN");
    textProtocol->PacketReceived(&client, "D\n");
    
    while(!terminated)
    {
        usleep(5000);
    }    
    client.Close();    
	return 0;
}
