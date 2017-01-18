#include "UnitTests.h"
#include "TWideString.h"

class Test_TWideString : public TestFixture<Test_TWideString>
{
public:
	TEST_FIXTURE( Test_TWideString)
	{
		TEST_CASE( TestSetters);
		TEST_CASE( TestComparations);
		TEST_CASE( TesTWideStringOperations);
		TEST_CASE( TesTWideStringFunctions);
        TEST_CASE( TestOperators);
        TEST_CASE( TestUTF8Conversion1);
        TEST_CASE( TestUTF8Conversion2);
		TEST_CASE( TesTWideStringReallocations);
		TEST_CASE( TestDestructors);
	}
	
	void TestSetters()
	{
		
        //constructor without parameters
        TWideString s1; 
        ASSERT(s1.Length()==0);        
		
        //constructor with const char* argument
        s1 = L"123";
        ASSERT(s1==L"123");
        ASSERT(s1.Length()==3);
		
        //constructor with TWideString argument
        TWideString s2 = s1;
        ASSERT(s2.Length()==3);
        ASSERT(s2==L"123");
		
        //setter  with const char* argument
        s1 = L"4567";
        ASSERT(s1==L"4567");
        ASSERT(s1.Length()==4);
		
        //setter with long const char* argument
        s2=L"1234567890123456789012345678901234567890";
        ASSERT(s2==L"1234567890123456789012345678901234567890");
		
        TWideString s3;
        //setter with TWideString argument
        s3  = s2;
        ASSERT(s3==L"1234567890123456789012345678901234567890");
		
        //test of clear method
        s2.Clear();
        ASSERT(s2.Length()==0);   
		
        //test of setters after clearing content
        s2 = L"123456";
        ASSERT(s2.Length()==6);
        ASSERT(s2==L"123456");
	}
	
	void TestComparations()
	{
        TWideString s1 = L"Charles";
		
        ASSERT(!(s1<L"Ann"));
        ASSERT(!(s1<L"C"));
        ASSERT(!(s1<=L"C"));        
        ASSERT(s1<=L"Charles");
        ASSERT(s1==L"Charles");
        ASSERT(!(s1!=L"Charles"));
        ASSERT(s1<L"Chris");
        ASSERT(s1<L"David");
        ASSERT(s1<=L"John");
		
        ASSERT(s1>L"Ann");
        ASSERT(s1>L"C");
        ASSERT(s1>=L"C");
        ASSERT(s1>=L"Charles");
        ASSERT(s1==L"Charles");
        ASSERT(!(s1!=L"Charles"));
        ASSERT(!(s1>L"Chris"));
        ASSERT(!(s1>L"David"));
        ASSERT(!(s1>=L"John"));
        
	}
	
	void TesTWideStringOperations()
	{
        TWideString s1 = L"ABCDEFGHIJ";
        ASSERT(s1.Length()==10);
		
        TWideString s2 = L"KLMNOPQRST";
        ASSERT(s2.Length()==10);
		
        TWideString s3 = s1;
        ASSERT(s1.Length()==10);
		
        s3 += s2;
        ASSERT(s2.Length()==10);
        ASSERT(s3.Length()==20);
		
        s3 += '_';
        ASSERT(s3.Length()==21);
        ASSERT(s3==L"ABCDEFGHIJKLMNOPQRST_");
		
        TWideString s4 = TWideString(L"(") + s3 + L")";
        ASSERT(s4==L"(ABCDEFGHIJKLMNOPQRST_)");
		
        TWideString s5;
        s5 += s1[1];
        s5 += s2[2];
        ASSERT(s5==L"BM");
    }
	
	void TesTWideStringFunctions()
	{
		TWideString s1 = L"\t !@#$%^&*()_+??+@ABYZ~abyz\t \t   \r\n";
		s1.Trim();

		ASSERT(s1==L"!@#$%^&*()_+??+@ABYZ~abyz");
		
		s1.LowerCase();
		ASSERT(s1==L"!@#$%^&*()_+??+@abyz~abyz");
		
		s1.UpperCase();
		ASSERT(s1==L"!@#$%^&*()_+??+@ABYZ~ABYZ");
		
		
		TWideString s2 = L"             ";
        s2.Trim();
		ASSERT(s2.Length()==0);             		
	}

    void TestOperators()
    {
        TWideString s1 = L"ABC";
        TWideString s2 = L"123";

        TWideString s3 = s1 + s2;
        TWideString s4 = s3;
        s4 += s4;
        s4 += L"!";
        s4 += L'@';
        
    }

    void TestUTF8Conversion1()
    {
        unsigned char utf8String[4];
        utf8String[0] = 'A';
        utf8String[1] = 'b';
        utf8String[2] = 'c';
        utf8String[3] =  0;

        TWideString ws;
        ws = (const char*)utf8String;
        ASSERT_EQUALS(3, (int)ws.Length());
        ASSERT(ws[0]=='A');
        ASSERT(ws[1]=='b');
        ASSERT(ws[2]=='c');
    }


    void TestUTF8Conversion2()
    {
        unsigned char utf8String[20];
        utf8String[0] = 0xC5;
        utf8String[1] = 0xBD;
        utf8String[2] = 0x6C;
        utf8String[3] = 0x75;
        utf8String[4] = 0xC5;
        utf8String[5] = 0xA5;
        utf8String[6] = 0x6F;
        utf8String[7] = 0x75;
        utf8String[8] = 0xC4;
        utf8String[9] = 0x8D;
        utf8String[10]= 0x6B;
        utf8String[11]= 0xC3;
        utf8String[12]= 0xBD;
        utf8String[13]= 0x20;
        utf8String[14]= 0x6B;
        utf8String[15]= 0xC5;
        utf8String[16]= 0xAF;
        utf8String[17]= 0xC5;
        utf8String[18]= 0x88;
        utf8String[19]= 0x00;

        TWideString ws;
        
        ws = (const char*) utf8String; //unicodeString = "éluùouËk˝ k˘Ú" (Zlutoucky kun)

        ASSERT_EQUALS(13, (int)ws.Length());
        ASSERT(ws[0]==0x17D);
        ASSERT(ws[1]==0x06C);
        ASSERT(ws[2]==0x075);
        ASSERT(ws[3]==0x165);
        ASSERT(ws[4]==0x06F);
        ASSERT(ws[5]==0x075);
        ASSERT(ws[6]==0x10D);
        ASSERT(ws[7]==0x06B);
        ASSERT(ws[8]==0x0FD);
        ASSERT(ws[9]==0x020);
        ASSERT(ws[10]==0x06B);
        ASSERT(ws[11]==0x16F);
        ASSERT(ws[12]==0x148);        

    }
	
	void TesTWideStringReallocations()
	{
		TWideString s1;
		for (int i = 0; i<1000; i++)
		{
            s1 += L"1234567890ABCDEFGHIJ";            
		}
		ASSERT(s1.Length()==20000);
	}
	
	void TestDestructors()
	{
		int i;
		
		TWideString s1;
		for (i = 0; i<15; i++)
		{
			TWideString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		ASSERT(s1.Length()==0);
		
		
		s1 = L"1234";
		for (i = 0; i<12; i++)
		{
			TWideString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		for (i = 1; i<14; i++)
		{
			TWideString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		
		ASSERT(s1.Length()==65534);		
	}
	
};


REGISTER_FIXTURE( Test_TWideString);
