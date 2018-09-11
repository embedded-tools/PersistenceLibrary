#ifndef DATARECEIVEDCALLBACK___H
#define DATARECEIVEDCALLBACK___H

class TcpClient;

typedef void (*DataReceivedCallback)(TcpClient* sender, const char* data, int dataLength);

#endif


