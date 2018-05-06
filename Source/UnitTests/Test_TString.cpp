#include "UnitTests.h"
#include "TString.h"
#include <stdlib.h>

class Test_TString : public TestFixture<Test_TString>
{
public:
	TEST_FIXTURE( Test_TString)
	{
		TEST_CASE( TestSetters);
		TEST_CASE( TestComparations);
		TEST_CASE( TestStringOperations);
		TEST_CASE( TestStringFunctions);
        TEST_CASE( TestOperators);
		TEST_CASE( TestStringReallocations);
		TEST_CASE( TestStringPrintf );
		TEST_CASE( TestDelete );
		TEST_CASE( TestDestructors);
		TEST_CASE( TestSTLConstructor );
		TEST_CASE( TestSTLFunctions );		
		TEST_CASE( TestSTLIterators );
	}
	
	void TestSetters()
	{
		
        //constructor without parameters
        TString s1; 
        ASSERT(s1.Length()==0);        
		
        //constructor with const char* argument
        s1 = "123";
        ASSERT(s1=="123");
        ASSERT(s1.Length()==3);
		
        //constructor with TString argument
        TString s2 = s1;
        ASSERT(s2.Length()==3);
        ASSERT(s2=="123");
		
        //setter  with const char* argument
        s1 = "4567";
        ASSERT(s1=="4567");
        ASSERT(s1.Length()==4);
		
        //setter with long const char* argument
        s2="1234567890123456789012345678901234567890";
        ASSERT(s2=="1234567890123456789012345678901234567890");
		
        TString s3;
        //setter with TString argument
        s3  = s2;
        ASSERT(s3=="1234567890123456789012345678901234567890");
		
        //test of clear method
        s2.Clear();
        ASSERT(s2.Length()==0);   
		
        //test of setters after clearing content
        s2 = "123456";
        ASSERT(s2.Length()==6);
        ASSERT(s2=="123456");
	}
	
	void TestComparations()
	{
        TString s1 = "Charles";
		
        ASSERT(!(s1<"Ann"));
        ASSERT(!(s1<"C"));
        ASSERT(!(s1<="C"));        
        ASSERT(s1<="Charles");
        ASSERT(s1=="Charles");
        ASSERT(!(s1!="Charles"));
        ASSERT(s1<"Chris");
        ASSERT(s1<"David");
        ASSERT(s1<="John");
		
        ASSERT(s1>"Ann");
        ASSERT(s1>"C");
        ASSERT(s1>="C");
        ASSERT(s1>="Charles");
        ASSERT(s1=="Charles");
        ASSERT(!(s1!="Charles"));
        ASSERT(!(s1>"Chris"));
        ASSERT(!(s1>"David"));
        ASSERT(!(s1>="John"));
        
	}
	
	void TestStringOperations()
	{
        TString s1 = "ABCDEFGHIJ";
        ASSERT(s1.Length()==10);
		
        TString s2 = "KLMNOPQRST";
        ASSERT(s2.Length()==10);
		
        TString s3 = s1;
        ASSERT(s1.Length()==10);
		
        s3 += s2;
        ASSERT(s2.Length()==10);
        ASSERT(s3.Length()==20);
		
        s3 += '_';
        ASSERT(s3.Length()==21);
        ASSERT(s3=="ABCDEFGHIJKLMNOPQRST_");
		
        TString s4 = TString("(") + s3 + ")";
        ASSERT(s4=="(ABCDEFGHIJKLMNOPQRST_)");
		
        TString s5;
        s5 += s1[1];
        s5 += s2[2];
        ASSERT(s5=="BM");
    }
	
	void TestStringFunctions()
	{
		TString s1 = " !@#$%^&*()_+??+@ABYZ~abyz";
		ASSERT(s1==" !@#$%^&*()_+??+@ABYZ~abyz");
		
		TString s2 = s1;
		s2.LowerCase();
		ASSERT(s1==" !@#$%^&*()_+??+@ABYZ~abyz");
		ASSERT(s2==" !@#$%^&*()_+??+@abyz~abyz");
		
		TString s3 = s1;
		s3.UpperCase();
		ASSERT(s1==" !@#$%^&*()_+??+@ABYZ~abyz");
		ASSERT(s3==" !@#$%^&*()_+??+@ABYZ~ABYZ");
		
		TString s4 = "  123    ";
		TString s5 = s4;
		s5.Trim();
		ASSERT(s5=="123");
				
		TString s7 = s4.Trim();
		ASSERT(s7 =="123");
		
		TString s8 = "     ";
		ASSERT(s8.Length()==5);
		
		TString s9 = s8;
		s9.Trim();
		ASSERT(s8.Length()==5);
		ASSERT(s9.Length()==0);             
		
	}

    void TestOperators()
    {
        TString s1 = "ABC";
        TString s2 = "123";

        TString s3 = s1 + s2;
        TString s4 = s3;
        s4 += s4;
        s4 += "!";
        s4 += '@';
        
    }
	
	void TestStringReallocations()
	{
		TString s1;
		for (int i = 0; i<1000; i++)
		{
            s1 += "1234567890ABCDEFGHIJ";            
		}
		ASSERT(s1.Length()==20000);
	}

	void TestStringPrintf()
	{
		char text[100];
		int  textLength = 0;

		TString s = "Hello world!";
		sprintf(text, "%s", s);
		
		ASSERT_EQUALS(12, (int)strlen(text));
	}

	void TestDelete()
	{
		TString s = "Text1, Text2, Text3";
		s.Delete(7,7);
		ASSERT(s=="Text1, Text3");

		s.Delete(5, 50);
		ASSERT(s=="Text1");

		s.Delete(0, 1);
		ASSERT(s=="ext1");

		s.Delete(0, 50);
		ASSERT(s.Length()==0);
	}		
	
	void TestDestructors()
	{
		int i;
		
		TString s1;
		for (i = 0; i<15; i++)
		{
			TString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		ASSERT(s1.Length()==0);
		
		
		s1 = "1234";
		for (i = 0; i<12; i++)
		{
			TString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		for (i = 1; i<14; i++)
		{
			TString s2;
			s2 = s1;
			s1 = s1 + s2;
		}
		
		ASSERT(s1.Length()==65534);
		
	}


#ifdef STL_STYLE

	void TestSTLConstructor()
	{
		TString s;
		ASSERT_EQUALS(0, s.size());
		ASSERT_EQUALS(0, s.length());
		ASSERT((s.max_size()>0) && (s.max_size()<65536));
		ASSERT(s.empty());
		ASSERT_EQUALS(0, (int)s.front());
		ASSERT_EQUALS(0, (int)s.back());
		ASSERT_EQUALS(0, (int)s.at(0));
		ASSERT(s.c_str()==NULL);
		ASSERT(s.data()==NULL);
		ASSERT_EQUALS(-1, s.find("a"));
		ASSERT_EQUALS(-1, s.find("a", 10));	
	}

	void TestSTLFunctions()
	{
		TString s;
		s.push_back('H');
		s.push_back('e');
		s.push_back('l');
		s.push_back('l');
		s.push_back('o');

		ASSERT_EQUALS(5, s.size());
		ASSERT_EQUALS(5, s.length());
		ASSERT(s.capacity()>5);
		ASSERT(!s.empty());
		ASSERT_EQUALS('H', s.at(0));
		ASSERT_EQUALS('e', s.at(1));
		ASSERT_EQUALS('l', s.at(2));
		ASSERT_EQUALS('l', s.at(3));
		ASSERT_EQUALS('o', s.at(4));
		ASSERT_EQUALS('o', s.back());
		ASSERT_EQUALS('H', s.front());
	
		s.append("world!");
		ASSERT_EQUALS(11, s.length());
		ASSERT_EQUALS(11, s.size());

		s.insert(5, " ");
		ASSERT_EQUALS(12, s.length());

		ASSERT(s=="Hello world!");

		ASSERT_EQUALS(-1, s.find("Canada"));
		ASSERT_EQUALS(0, s.find("Hello"));
		ASSERT_EQUALS(0, s.find("Hello"));
	}

	void TestSTLIterators()
	{
		TString s;

		char* it1 = s.begin();
		ASSERT(it1!=NULL);

		char* it2 = s.end();
		ASSERT(it2!=NULL);

		TString output;
		for(TString::iterator it3 = s.begin(); it3!=s.end(); it3++)
		{
			output += *it3;
		}
		ASSERT_EQUALS(0, output.length());

		s = "Hello world!";

		char* it4 = s.begin();
		ASSERT(it4!=NULL);

		char* it5 = s.end();
		ASSERT(it5!=NULL);

		output.clear();

		for(TString::iterator it6 = s.begin(); it6!=s.end(); it6++)
		{
			output += *it6;
		}
		ASSERT_EQUALS(12, output.length());
	}


#endif
	
};


REGISTER_FIXTURE( Test_TString);
