#include <stdio.h>
#include <unistd.h>
#include "tcpclient_linux.h"
#include "TTextProtocol.h"

TTextProtocol* textProtocol;

int commandCounter = 0;
bool terminated = false;

void DataReceived(void* sender, const char* data, int dataLength)
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
}

int main(int argc, char **argv)
{    
    TcpClient client;

    textProtocol = new TTextProtocol(64, '\n');
    textProtocol->OnCommandReceived = ShowEachCommand;
    
    //sends three different commands
    textProtocol->PacketReceived(&client, "G X10 Y20 Z30\n");
    textProtocol->PacketReceived(&client, "SHOW 1:1");
    textProtocol->PacketReceived(&client, "\'Hello th");
    textProtocol->PacketReceived(&client, "ere'\nLAST COMMAN");
    textProtocol->PacketReceived(&client, "D\n");
    
    //textProtocol class finds command separators and shows each command    
    client.OpenAsync("192.168.1.3", 4000, DataReceived);    
    while(!terminated)
    {
        usleep(5000);
    }    
    client.Close();    
	return 0;
}
