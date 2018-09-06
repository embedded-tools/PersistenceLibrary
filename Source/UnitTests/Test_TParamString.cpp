#include "UnitTests.h"
#include "tparamstring.h"
#include "tstring.h"
#include "tshortstring.h"

class Test_TParamString : public TestFixture<Test_TParamString>
{
  public:

    TEST_FIXTURE( Test_TParamString)
    {
        TEST_CASE( TestConstructor1 );
        TEST_CASE( TestReadOne );
        TEST_CASE( TestReadMany );        
    }

    void TestConstructor1()
    {
        TParamString params("TEXT = Hello; AUDIO=Ding.wav; VIDEO = Dong.avi; VOLUME = 255 ; STEREO = false", 0, '=', ';');
	}

    void TestReadOne()
    {
        TParamString params("AUDIO=Ding.wav");

        TString text;

        ASSERT(params.GetStringValue("AUDIO",  false, text) );
        ASSERT(text=="Ding.wav");
    }

    void TestReadMany()
    {
        TParamString params("TEXT=\"Hello\";AUDIO=Ding.wav;VIDEO=\"Dong.avi\";VOLUME=255;STEREO=true");

        bool stereo = false;
        int  volume = 0;
        TString text;
        TShortString text2;
        char    text3[100];
        int     text3Length;

        ASSERT( params.GetBoolValue  ("STEREO", stereo) );
        ASSERT( params.GetIntValue   ("VOLUME", volume) );
        ASSERT( params.GetStringValue("TEXT",  true,  text) );    
        ASSERT( params.GetStringValue("AUDIO", false, text2) );
        ASSERT( params.GetStringValue("VIDEO", true,  text3, sizeof(text3), text3Length) );
        
        ASSERT( stereo );
        ASSERT_EQUALS(255, volume);
        ASSERT(text=="Hello");
        ASSERT(text2=="Ding.wav");
        ASSERT(strcmp(text3, "Dong.avi")==0);
    }
};

REGISTER_FIXTURE( Test_TParamString);




