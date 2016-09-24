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
		TWideString s1 = L" !@#$%^&*()_+??+@ABYZ~abyz";
		ASSERT(s1==L" !@#$%^&*()_+??+@ABYZ~abyz");
		
		TWideString s2 = s1.ToLower();
		ASSERT(s1==L" !@#$%^&*()_+??+@ABYZ~abyz");
		ASSERT(s2==L" !@#$%^&*()_+??+@abyz~abyz");
		
		TWideString s3 = s1.ToUpper();
		ASSERT(s1==L" !@#$%^&*()_+??+@ABYZ~abyz");
		ASSERT(s3==L" !@#$%^&*()_+??+@ABYZ~ABYZ");
		
		TWideString s4 = L"  123    ";
		TWideString s5 = s4.TrimStart();
		ASSERT(s5==L"123    ");
		
		TWideString s6 = s4.TrimEnd();
		ASSERT(s6 ==L"  123");
		
		TWideString s7 = s4.Trim();
		ASSERT(s7 ==L"123");
		
		TWideString s8 = L"     ";
		ASSERT(s8.Length()==5);
		
		TWideString s9 = s8.Trim();
		ASSERT(s8.Length()==5);
		ASSERT(s9.Length()==0);             
		
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
