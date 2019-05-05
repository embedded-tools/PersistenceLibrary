#include "minicppunit.hxx"
#include "tbinaryprotocol.h"

static unsigned char  dataInTheWire[512];
static unsigned short dataInTheWireLength;
static unsigned char  eeprom[256];
static unsigned char  incomingData[512];
static unsigned short incomingDataLength;
static unsigned short packetsReceived = 0;
static unsigned short packetsSent = 0;
static unsigned short packetErrors = 0;


class Test_TBinaryProtocol : public TestFixture<Test_TBinaryProtocol>
{

  private:
    	  

  public:


    TEST_FIXTURE( Test_TBinaryProtocol)
    {
        TEST_CASE( Construct );
        TEST_CASE( SendPing );
        TEST_CASE( ReceivePing );
		TEST_CASE( SendDataViaBuffer );
		TEST_CASE( ReceiveViaDataBuffer );
		TEST_CASE( SendDataWithoutBuffer );
		TEST_CASE( ReceiveDataWithoutBuffer );
    }

    static void SendByte(unsigned char c)
    {
        dataInTheWire[dataInTheWireLength] = c;
        dataInTheWireLength++;
    }

    static void CommandReceived(TBinaryProtocol* sender, unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data )
    {
        if (command & 0x8000)
        {
            return;
        }
        unsigned char* pOutputData = NULL;
        unsigned short outputDataSize = 0;
        unsigned short errorCode = 0;
        switch(command)
        {
            case bcLedGreenOn:
            case bcLedGreenOff:
            case bcLedGreenToggle:
            case bcLedRedOn:
            case bcLedRedOff:
            case bcLedRedToggle:
            case bcOutputOn:
            case bcOutputOff:
            case bcOutputToggle:
            case bcBeep:
            case bcReadConfiguration:
                {
                    pOutputData = NULL;
                    outputDataSize = 0;
                }
                break;
            case bcStoreConfiguration:
            case bcReadData:
                {
                    pOutputData = NULL;
                    outputDataSize = 0;
                }
                break;
            case bcWriteData:
            case bcOpenPort:
            case bcClosePort:
            case bcWriteToPort:
            case bcReadFromPort:
            case bcCustomCommand:
                {

                }
                break;
        }
        sender->SendCustomResponse(command, errorCode, customParam1, customParam2, pOutputData, outputDataSize);

		incomingDataLength = dataSize;
        packetsReceived++;
    }

    static void CommandReceivingError(TBinaryProtocol* sender, unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data )
    {
        packetErrors++;        
    }

    static void CommandSent(TBinaryProtocol* sender, unsigned short busAddress, unsigned short command, short customParam1, short customParam2, unsigned short dataSize, unsigned char* data )
    {
        packetsSent++;
    }

	static unsigned char ReadDataFromEEPROM(unsigned short address)
	{
		//reading data from source directly without any buffer
		return eeprom[address];
	}

	static void WriteDataToEEPROM(unsigned short address, unsigned char value)
	{
		if (address<0) return;
		if (address>=sizeof(eeprom)) return;

		eeprom[address] = value;
        //writing data to destination directly without any buffer
	}

    void Construct()
    {
        TBinaryProtocol prot;
        prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);
   	}

    void SendPing()
    {
        unsigned short busAddress = 0;

        dataInTheWireLength = 0;

        packetsReceived = 0;
        packetsSent = 0;
        packetErrors = 0;

        TBinaryProtocol prot;
        prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);
        prot.SendPing(35);
        for(int i = 0; i<100; i++)
        {
            prot.OnByteSent();
        }
        ASSERT(dataInTheWireLength==14);

        ASSERT(packetsReceived==0);
        ASSERT(packetErrors == 0);
        ASSERT(packetsSent == 1);
        
        unsigned char*  magicByte1    = &dataInTheWire[0];
        unsigned char*  magicByte2    = &dataInTheWire[1];
        unsigned short* command       = (unsigned short*)&dataInTheWire[2];
        unsigned short* deviceAddress = (unsigned short*)&dataInTheWire[4];        
        unsigned short* param1        = (unsigned short*)&dataInTheWire[6];
        unsigned short* param2        = (unsigned short*)&dataInTheWire[8];
        unsigned short* dataSize      = (unsigned short*)&dataInTheWire[10];
        unsigned short* headerCRC     = (unsigned short*)&dataInTheWire[12];

        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_1, (long)*magicByte1);
        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_2, (long)*magicByte2);
        ASSERT_EQUALS(BINPROTOCOL_PING, (long)*command);
        ASSERT_EQUALS(35, (long)*deviceAddress);        
        ASSERT_EQUALS(0, (long)*param1);
        ASSERT_EQUALS(0, (long)*param2);
        ASSERT_EQUALS(0, (long)*dataSize);
        ASSERT_EQUALS(52787, (long)*headerCRC);
    }

    void ReceivePing()
    {
        TBinaryProtocol prot;
        int i;
        prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);

        packetsReceived = 0;
        packetsSent = 0;
        packetErrors = 0;
        for(i = 0; i<dataInTheWireLength; i++)
        {
            prot.OnByteReceived(dataInTheWire[i]);
        }
        ASSERT(packetsReceived==0);
        ASSERT(packetErrors == 0);
        ASSERT(packetsSent == 0);

        prot.SetBusAddress(35);
        for(i = 0; i<dataInTheWireLength; i++)
        {
            prot.OnByteReceived(dataInTheWire[i]);
        }
        ASSERT(packetsReceived==1);
        ASSERT(packetErrors == 0);
        ASSERT(packetsSent == 0);
    }

	void SendDataViaBuffer()
	{
        TBinaryProtocol prot;
		prot.SetDataBuffer(incomingData, incomingDataLength);
        prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);

		dataInTheWireLength = 0;
        packetsReceived = 0;
        packetsSent = 0;
        packetErrors = 0;

		unsigned char outgoingData[4];
		unsigned short outgoingDataLength = 4;
		outgoingData[0] = 'D';
		outgoingData[1] = 'A';
		outgoingData[2] = 'T';
		outgoingData[3] = 'X';

		prot.SendCustomCommand(111, 1234, 0, 0, outgoingData, outgoingDataLength);
		for(int i = 0; i<100; i++)
		{
			prot.OnByteSent();
		}

		unsigned char*  magicByte1    = &dataInTheWire[0];
		unsigned char*  magicByte2    = &dataInTheWire[1];
        unsigned short* command       = (unsigned short*)&dataInTheWire[2];
		unsigned short* deviceAddress = (unsigned short*)&dataInTheWire[4];		
		unsigned short* param1        = (unsigned short*)&dataInTheWire[6];
		unsigned short* param2        = (unsigned short*)&dataInTheWire[8];
		unsigned short* dataSize      = (unsigned short*)&dataInTheWire[10];
		unsigned short* headerCRC     = (unsigned short*)&dataInTheWire[12];
        unsigned char   b1            = dataInTheWire[14];
        unsigned char   b2            = dataInTheWire[15];
        unsigned char   b3            = dataInTheWire[16];
        unsigned char   b4            = dataInTheWire[17];
        unsigned short*  dataCRC       = (unsigned short*)&dataInTheWire[18];


		ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_1, (long)*magicByte1);
		ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_2, (long)*magicByte2);		
		ASSERT_EQUALS(111, (long)*command);
        ASSERT_EQUALS(1234, (long)*deviceAddress);
		ASSERT_EQUALS(0, (long)*param1);
		ASSERT_EQUALS(0, (long)*param2);
		ASSERT_EQUALS(4, (long)*dataSize);
		ASSERT_EQUALS(51802, (long)*headerCRC);

		ASSERT_EQUALS(20,  (long)dataInTheWireLength);
		ASSERT_EQUALS('D', (long)b1);
		ASSERT_EQUALS('A', (long)b2);
		ASSERT_EQUALS('T', (long)b3);
		ASSERT_EQUALS('X', (long)b4);
        ASSERT_EQUALS(54578, (long)*dataCRC);

	}

	void ReceiveViaDataBuffer()
	{
		TBinaryProtocol prot;
        prot.SetBusAddress(1234);
		prot.SetDataBuffer(incomingData, sizeof(incomingData));
		prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);

		packetsReceived = 0;
		packetsSent = 0;
		packetErrors = 0;
		for(int i = 0; i<dataInTheWireLength; i++)
		{
			prot.OnByteReceived(dataInTheWire[i]);
		}
		ASSERT(incomingDataLength==4);
		ASSERT(incomingData[0]=='D');
		ASSERT(incomingData[1]=='A');
		ASSERT(incomingData[2]=='T');
		ASSERT(incomingData[3]=='X');
		ASSERT(packetsReceived==1);
		ASSERT(packetErrors == 0);
		ASSERT(packetsSent == 0);
	}

	void SendDataWithoutBuffer()
	{
		eeprom[0] = 11;
		eeprom[1] = 22;
		eeprom[2] = 33;
		eeprom[3] = 44;
		unsigned short eepromDataLength = 4;

		TBinaryProtocol prot;
		prot.SetOnFlyDataHandlers(WriteDataToEEPROM, ReadDataFromEEPROM);
		prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);

		dataInTheWireLength = 0;
		packetsReceived = 0;
		packetsSent = 0;
		packetErrors = 0;

		prot.SendCustomCommand(111, 1234, 0, 0, NULL, eepromDataLength);
		for(int i = 0; i<100; i++)
		{
			prot.OnByteSent();
		}

        unsigned char*  magicByte1    = &dataInTheWire[0];
        unsigned char*  magicByte2    = &dataInTheWire[1];
        unsigned short* command       = (unsigned short*)&dataInTheWire[2];
        unsigned short* deviceAddress = (unsigned short*)&dataInTheWire[4];        
        unsigned short* param1        = (unsigned short*)&dataInTheWire[6];
        unsigned short* param2        = (unsigned short*)&dataInTheWire[8];
        unsigned short* dataSize      = (unsigned short*)&dataInTheWire[10];
        unsigned short* headerCRC     = (unsigned short*)&dataInTheWire[12];
        unsigned char   b1            = dataInTheWire[14];
        unsigned char   b2            = dataInTheWire[15];
        unsigned char   b3            = dataInTheWire[16];
        unsigned char   b4            = dataInTheWire[17];
        unsigned short*  dataCRC       = (unsigned short*)&dataInTheWire[18];

        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_1, (long)*magicByte1);
        ASSERT_EQUALS(BINPROTOCOL_MAGIC_BYTE_2, (long)*magicByte2);        
        ASSERT_EQUALS(111, (long)*command);
        ASSERT_EQUALS(1234, (long)*deviceAddress);
        ASSERT_EQUALS(0, (long)*param1);
        ASSERT_EQUALS(0, (long)*param2);
        ASSERT_EQUALS(4, (long)*dataSize);
        ASSERT_EQUALS(51802, (long)*headerCRC);

        ASSERT_EQUALS(20,  (long)dataInTheWireLength);
        ASSERT_EQUALS(11, (long)b1);
        ASSERT_EQUALS(22, (long)b2);
        ASSERT_EQUALS(33, (long)b3);
        ASSERT_EQUALS(44, (long)b4);
        ASSERT_EQUALS(56430, (long)*dataCRC);

	}

	void ReceiveDataWithoutBuffer()
	{
		eeprom[0] = 0;
		eeprom[1] = 0;
		eeprom[2] = 0;
		eeprom[3] = 0;

		TBinaryProtocol prot;
        prot.SetBusAddress(1234);
		prot.SetOnFlyDataHandlers(WriteDataToEEPROM, ReadDataFromEEPROM);
		prot.SetEventHandlers(SendByte, CommandReceived, CommandReceivingError, CommandSent);

		packetsReceived = 0;
		packetsSent = 0;
		packetErrors = 0;
		for(int i = 0; i<dataInTheWireLength; i++)
		{
			prot.OnByteReceived(dataInTheWire[i]);
		}
		ASSERT(packetsReceived==1);
		ASSERT(packetErrors == 0);
		ASSERT(packetsSent == 0);
		ASSERT(eeprom[0]==11);
		ASSERT(eeprom[1]==22);
		ASSERT(eeprom[2]==33);
		ASSERT(eeprom[3]==44);

	}




};

REGISTER_FIXTURE( Test_TBinaryProtocol);


