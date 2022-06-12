#include "UnitTests.h"

#include "TEventSender.h"
#include "TEventReceiver.h"
#include "TEventSerializer.h"

class TMyReceiver : public TEventReceiver
{
    public:
 	    int Counter;
};


class Test_TEventSender : public TestFixture<Test_TEventSender>
{
  public:

    TEST_FIXTURE( Test_TEventSender)
    {
        TEST_CASE(SendEvent);
		TEST_CASE(SerializeEvent1);
		TEST_CASE(SerializeEvent2);
		TEST_CASE(DeserializeEvent1);
		TEST_CASE(DeserializeEvent2);
    }

    void SendEvent()
    {
		TEventSender sender;
		TMyReceiver receiver1;
		TMyReceiver receiver2;

		receiver1.Counter = 0;
		receiver2.Counter = 0;
		receiver1.connect(&sender, 9, Event_Callback);
		receiver2.connect(&sender, 9, Event_Callback);

		TEvent ev;
		memset(&ev, 0, sizeof(ev));
		ev.eventID = 9;

		sender.emit(ev);
		ASSERT_EQUALS(1, receiver1.Counter);
		ASSERT_EQUALS(1, receiver2.Counter);
	}

	static void Event_Callback(void* sender, void* receiver, const TEvent& event)
	{
		if(event.eventID==9)
		{
			TMyReceiver* myReceiver = (TMyReceiver*)receiver;
			myReceiver->Counter++;
		}
	}

	void SerializeEvent1()
	{
		TEvent ev;
		memset(&ev, 0, sizeof(TEvent));

		ev.eventID = 123;
		ev.paramL = -1234567890;
		ev.paramH = -12345;

		char buf[30];
		bool b = TEventSerializer::serialize(ev, buf, sizeof(buf));
		ASSERT(b);

		ASSERT(strcmp(buf, "123,-1234567890,-12345") == 0);
	}

	void SerializeEvent2()
	{
		TEvent ev;
		memset(&ev, 0, sizeof(TEvent));

		ev.eventID = 123;
		ev.paramL = -1234567890;
		ev.paramH = -12345;
		ev.dataLength = 2;
		ev.data = (unsigned char*)malloc(2);
		ev.data[0] = 0x54;
		ev.data[1] = 0xA9;

		char buf[30];
		bool b = TEventSerializer::serialize(ev, buf, sizeof(buf));
		ASSERT(b);

		ASSERT(strcmp(buf, "123,-1234567890,-12345,\"54A9\"") == 0);
	}

	void DeserializeEvent1()
	{
		const char* text = "123,-1234567890,-12345";

		TEvent ev;
		TEventSerializer::deserialize(text, strlen(text), ev);

		ASSERT_EQUALS(123, ev.eventID);
		ASSERT_EQUALS(-1234567890, ev.paramL);
		ASSERT_EQUALS(-12345, ev.paramH);
		ASSERT_EQUALS(0, ev.dataLength);
		ASSERT(ev.data==NULL);
	}

	void DeserializeEvent2()
	{
		const char* text = "123,-1234567890,-12345,\"54A9\"";

		TEvent ev;
		TEventSerializer::deserialize(text, strlen(text), ev);

		ASSERT_EQUALS(123, ev.eventID);
		ASSERT_EQUALS(-1234567890, ev.paramL);
		ASSERT_EQUALS(-12345, ev.paramH);
		ASSERT_EQUALS(2, ev.dataLength);
		ASSERT_EQUALS(84,  (int)ev.data[0]);
		ASSERT_EQUALS(169, (int)ev.data[1]);

	}



	void TestOfBufferLimits()
	{
		char buf2[1];
		buf2[0] = (char)0xAA;

		TEvent ev;
		memset(&ev, 0, sizeof(TEvent));

		ev.eventID = 123;
		ev.paramL  = -1234567890;
		ev.paramH  = -12345;
		ev.dataLength = 2;
		ev.data = (unsigned char*)malloc(2);
		ev.data[0] = 0x54;
		ev.data[1] = 0xA9;

		char buf[30];

		bool b;
		for (int i = 0; i < (int)sizeof(buf); i++)
		{
			b = TEventSerializer::serialize(ev, buf, i);
			ASSERT(!b);
		}
		b = TEventSerializer::serialize(ev, buf, sizeof(buf));
		ASSERT(b);

		ASSERT(strcmp(buf, "123,-1234567890,-12345,\"54A9\"") == 0);
	}


};

REGISTER_FIXTURE( Test_TEventSender);



