#include "minicppunit.hxx"
#include "tbinaryprotocol.h"
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

TBinaryProtocol sender;
TBinaryProtocol receiver;

class Test_TBinaryProtocol : public TestFixture<Test_TBinaryProtocol>
{
  private:

	  unsigned char inputBuffer[2048];
	  unsigned char inputBuffer2[2048];
	  unsigned char outputBuffer[2048];
	  unsigned char outputBuffer2[2048];

	  static void SendPacket(unsigned char* data, unsigned short dataLength)
	  {
		 receiver.DataReceived(data, dataLength);
	  }

	  static void SendPacket2(unsigned char* data, unsigned short dataLength)
	  {
		  sender.DataReceived(data, dataLength);
	  }

	  static void SleepFunc(unsigned short time_ms)
	  {
		  #ifdef WIN32
		  Sleep(time_ms);
		  #else
		  usleep(time_ms * 1000);
	      #endif
	  }
    	  
  public:

    TEST_FIXTURE( Test_TBinaryProtocol)
    {
		TEST_CASE( ClassNotInitialized);
        TEST_CASE( Initialize );		
        TEST_CASE( InvalidResponse );
		TEST_CASE( SendPing );		
    }

	void ClassNotInitialized()
	{
		TRequest request = sender.SendPing(100);
		ASSERT(request.error == crClassNotInitialized);

		TResponse response = receiver.WaitForResponse(100);
		ASSERT(request.error == crClassNotInitialized);
	}

	void Initialize()
	{
		sender.SetBusAddress(11);
		sender.SetSleepCallback(SleepFunc);
		sender.SetSendPacketCallback(SendPacket);
		sender.SetInputBuffer(inputBuffer, sizeof(inputBuffer));
		sender.SetOutputBuffer(outputBuffer, sizeof(outputBuffer));

		receiver.SetBusAddress(22);
		receiver.SetSleepCallback(SleepFunc);
		receiver.SetSendPacketCallback(SendPacket2);
		receiver.SetInputBuffer(inputBuffer2, sizeof(inputBuffer2));
		receiver.SetOutputBuffer(outputBuffer2, sizeof(outputBuffer2));
	}

	void InvalidResponse()
	{
		TRequest request  = sender.SendPing(22);
		ASSERT(request.error == crSuccess);

		TResponse response = receiver.WaitForResponse(500);
		ASSERT(response.error == crTimeout);
	}

	void SendPing()
	{
		TRequest request  = sender.SendPing(22);
		ASSERT(request.error == crSuccess);

		TRequest request2 = receiver.WaitForRequest(500);
		ASSERT(request.error == crSuccess);

		ASSERT(request.packetId      == request2.packetId);
		ASSERT(request.senderAddress == request2.senderAddress);
		ASSERT(request.command       == request2.command);
		ASSERT(request.customParam1  == request2.customParam1);
		ASSERT(request.customParam2  == request2.customParam2);
		ASSERT(request.customParam3  == request2.customParam3);
		ASSERT(request.dataLength    == request2.dataLength);

		sender.ClearInputBuffer();

		TResponse response = receiver.SendResponse(request, crSuccess);
		ASSERT(response.error == crSuccess);
		
		TResponse response2 = sender.WaitForSpecificResponse(request, 1000);
		ASSERT(response2.error == crSuccess);
	}


    void SendPingWithError()
    {
		TRequest request = sender.SendPing(22);
		TResponse response = receiver.WaitForResponse(500);

		ASSERT(response.error == crTimeout);
	}


		/*
        ASSERT(packetsReceived==0);
        ASSERT(packetErrors == 0);
        ASSERT(packetsSent == 1);	       
        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_1, (long)*magicByte1);
        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_2, (long)*magicByte2);
        ASSERT_EQUALS(BINPROTOCOL_PING, (long)*command);
        ASSERT_EQUALS(35, (long)*deviceAddress);        
        ASSERT_EQUALS(0, (long)*param1);
        ASSERT_EQUALS(0, (long)*param2);
        ASSERT_EQUALS(0, (long)*dataSize);
        ASSERT_EQUALS(52787, (long)*headerCRC);
		*/

};

REGISTER_FIXTURE( Test_TBinaryProtocol);


