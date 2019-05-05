                                                
#include "UnitTests.h"
#include "TStringList.h"
#include "StringUtils.h"

class Test_TStringList : public TestFixture<Test_TStringList>
{
  public:

    TEST_FIXTURE( Test_TStringList)
    {
        TEST_CASE( AddStrings );
        TEST_CASE( SortStrings );        
        TEST_CASE( DelStrings );
        TEST_CASE( IndexOf );
    }


    void AddStrings()
    {
        TStringList sl;
        ASSERT(sl.Count()==0);
        ASSERT(sl.GetLength()==0);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("1");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==2);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("ABC");
        ASSERT(sl.Count()==2);
        ASSERT(sl.GetLength()==6);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("1234567890");
        ASSERT(sl.Count()==3);
        ASSERT(sl.GetLength()==17);
        ASSERT(sl.GetCapacity()==64);
        sl.Add("123456789012345678901234567890123456789012345");
        ASSERT(sl.Count()==4);
        ASSERT(sl.GetLength()==63);
        ASSERT(sl.GetCapacity()==64);
        ASSERT(sl.GetStringAsObject(0) == "1");
        ASSERT(sl.GetStringAsObject(1) == "ABC");
        ASSERT(sl.GetStringAsObject(2) == "1234567890");
        ASSERT(sl.GetStringAsObject(3) == "123456789012345678901234567890123456789012345");

        sl.Clear();
        ASSERT(sl.Count()==0);
        sl.Add("1234567890123456789012345678901234567890123456789012345678901");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==62);
        ASSERT(sl.GetCapacity()==64);
        ASSERT(sl.GetStringAsObject(0) == "1234567890123456789012345678901234567890123456789012345678901");

        sl.Clear();
        ASSERT(sl.Count()==0);
        sl.Add("12345678901234567890123456789012345678901234567890123456789012");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==63);
        ASSERT(sl.GetCapacity()==64);
        ASSERT(sl.GetStringAsObject(0) == "12345678901234567890123456789012345678901234567890123456789012");

        sl.Clear();
        ASSERT(sl.Count()==0);
        sl.Add("123456789012345678901234567890123456789012345678901234567890123");
        ASSERT(sl.Count()==1);
        ASSERT(sl.GetLength()==64);
        ASSERT(sl.GetCapacity()>64);
        ASSERT(sl.GetStringAsObject(0) == "123456789012345678901234567890123456789012345678901234567890123");
	}

    void AddNumbers()
    {
        TStringList sl;

        char s[20];
        for(char c1 = 1; c1<=100; c1++)
        {            
            LongIntToStr(c1, s, 20);
            sl.Add(s);
        }
        for(char c2 = 0; c2<100; c2++)
        {
            LongIntToStr(c2+1, s, 20);
            ASSERT(sl.GetStringAsObject(c2)==(const char*)&s);
        }
    }

    void SortStrings()
    {
        TStringList sl(true);

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");
        ASSERT(sl.GetStringAsObject(0)=="Aaron");
        ASSERT(sl.GetStringAsObject(1)=="Abel");        
        ASSERT(sl.GetStringAsObject(2)=="Charles");
        ASSERT(sl.GetStringAsObject(3)=="Darryl");
        ASSERT(sl.GetStringAsObject(4)=="Eva");
        ASSERT(sl.GetStringAsObject(5)=="Frank");
        ASSERT(sl.GetStringAsObject(6)=="Franklin");
        ASSERT(sl.GetStringAsObject(7)=="Irene");
        ASSERT(sl.GetStringAsObject(8)=="Rachel");
      
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        ASSERT(sl.GetStringAsObject(0)=="Aaron");
        ASSERT(sl.GetStringAsObject(1)=="Abe");
        ASSERT(sl.GetStringAsObject(2)=="Abel");
        ASSERT(sl.GetStringAsObject(3)=="Bessie");
        ASSERT(sl.GetStringAsObject(4)=="Charles");
        ASSERT(sl.GetStringAsObject(5)=="Darryl");
        ASSERT(sl.GetStringAsObject(6)=="Eva");
        ASSERT(sl.GetStringAsObject(7)=="Evan");
        ASSERT(sl.GetStringAsObject(8)=="Frank");
        ASSERT(sl.GetStringAsObject(9)=="Franklin");
        ASSERT(sl.GetStringAsObject(10)=="Gabriel");
        ASSERT(sl.GetStringAsObject(11)=="Hannah");
        ASSERT(sl.GetStringAsObject(12)=="Irene");
        ASSERT(sl.GetStringAsObject(13)=="Jerald");
        ASSERT(sl.GetStringAsObject(14)=="Kate");
        ASSERT(sl.GetStringAsObject(15)=="Linda");
        ASSERT(sl.GetStringAsObject(16)=="Miles");
        ASSERT(sl.GetStringAsObject(17)=="Naomi");
        ASSERT(sl.GetStringAsObject(18)=="Phillip");
        ASSERT(sl.GetStringAsObject(19)=="Phillippa");
        ASSERT(sl.GetStringAsObject(20)=="Quentin");
        ASSERT(sl.GetStringAsObject(21)=="Rachel");
        ASSERT(sl.GetStringAsObject(22)=="Ralph");

    }

    void DelStrings()
    {
        TStringList sl(true);

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");     
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        sl.Delete(22);
        sl.Delete(21);
        sl.Delete(17);
        sl.Delete(13);
        sl.Delete(12);
        sl.Delete(10);
        sl.Delete(8);
        sl.Delete(7);
        sl.Delete(3);
        sl.Delete(2);
        sl.Delete(1);
        sl.Delete(0);
        sl.Delete(0);
        sl.Delete(0);

        ASSERT(sl.GetStringAsObject(0)=="Eva");
        ASSERT(sl.GetStringAsObject(1)=="Franklin");
        ASSERT(sl.GetStringAsObject(2)=="Hannah");
        ASSERT(sl.GetStringAsObject(3)=="Kate");
        ASSERT(sl.GetStringAsObject(4)=="Linda");
        ASSERT(sl.GetStringAsObject(5)=="Miles");
        ASSERT(sl.GetStringAsObject(6)=="Phillip");
        ASSERT(sl.GetStringAsObject(7)=="Phillippa");
        ASSERT(sl.GetStringAsObject(8)=="Quentin");


        sl.Delete(8);
        ASSERT(sl.Count()==8);
        sl.Delete(9);
        ASSERT(sl.Count()==8);
        sl.Delete(-1);
        ASSERT(sl.Count()==8);
        sl.Delete(7);
        ASSERT(sl.Count()==7);
        sl.Delete(6);
        ASSERT(sl.Count()==6);
        sl.Delete(0);
        ASSERT(sl.Count()==5);
        sl.Delete(4);
        ASSERT(sl.Count()==4);
        sl.Delete(1);
        ASSERT(sl.Count()==3);
        
        ASSERT(sl.GetStringAsObject(0)=="Franklin");
        ASSERT(sl.GetStringAsObject(1)=="Kate");
        ASSERT(sl.GetStringAsObject(2)=="Linda");

        sl.Delete(2);
        ASSERT(sl.Count()==2);
        sl.Delete(0);
        ASSERT(sl.Count()==1);
        sl.Delete(0);
        ASSERT(sl.Count()==0);

        sl.Clear();
    }

    void IndexOf()
    {
        TStringList sl;

        sl.Clear();
        sl.Add("Aaron");
        sl.Add("Eva");
        sl.Add("Charles");
        sl.Add("Abel");
        sl.Add("Franklin");
        sl.Add("Frank");        
        sl.Add("Irene");
        sl.Add("Darryl");
        sl.Add("Rachel");      
        sl.Add("Bessie");
        sl.Add("Evan");
        sl.Add("Abe");
        sl.Add("Gabriel");
        sl.Add("Jerald");
        sl.Add("Hannah");
        sl.Add("Ralph");
        sl.Add("Kate");
        sl.Add("Linda");
        sl.Add("Miles");
        sl.Add("Naomi");
        sl.Add("Phillip");
        sl.Add("Phillippa");
        sl.Add("Quentin");

        ASSERT(sl.IndexOf("Aaron")==0);
        ASSERT(sl.IndexOf("Eva")==1);
        ASSERT(sl.IndexOf("Charles")==2);
        ASSERT(sl.IndexOf("Abel")==3);
        ASSERT(sl.IndexOf("Franklin")==4);
        ASSERT(sl.IndexOf("Frank")==5);        
        ASSERT(sl.IndexOf("Irene")==6);
        ASSERT(sl.IndexOf("Darryl")==7);
        ASSERT(sl.IndexOf("Rachel")==8);      
        ASSERT(sl.IndexOf("Bessie")==9);
        ASSERT(sl.IndexOf("Evan")==10);
        ASSERT(sl.IndexOf("Abe")==11);
        ASSERT(sl.IndexOf("Gabriel")==12);
        ASSERT(sl.IndexOf("Jerald")==13);
        ASSERT(sl.IndexOf("Hannah")==14);
        ASSERT(sl.IndexOf("Ralph")==15);
        ASSERT(sl.IndexOf("Kate")==16);
        ASSERT(sl.IndexOf("Linda")==17);
        ASSERT(sl.IndexOf("Miles")==18);
        ASSERT(sl.IndexOf("Naomi")==19);
        ASSERT(sl.IndexOf("Phillip")==20);
        ASSERT(sl.IndexOf("Phillippa")==21);
        ASSERT(sl.IndexOf("Quentin")==22);

    }

};

REGISTER_FIXTURE( Test_TStringList);


