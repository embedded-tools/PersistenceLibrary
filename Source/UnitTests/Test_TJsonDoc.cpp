#include "UnitTests.h"
#include "TJsonDoc.h"
#include "TJsonTag.h"
#include "TJsonTagStaticPool.h"
#include "TJsonTagDynamicPool.h"
#include "TFileStream.h"

class Test_TJsonDoc : public TestFixture<Test_TJsonDoc>
{
  public:
    
    TEST_FIXTURE( Test_TJsonDoc)
    {
	TEST_CASE( JsonDoc_ConstructorDestructor );
	TEST_CASE( JsonPool_ConstructorDestructor );
	TEST_CASE( ParseString );
	TEST_CASE( ParseNumber );
	TEST_CASE( ParseBoolean );
	
	TEST_CASE( OverallTest_Static );
        TEST_CASE( OverallTest_Dynamic );
    }

	void JsonDoc_ConstructorDestructor()
	{
		TJsonDoc doc;	
	}

	void JsonPool_ConstructorDestructor()
	{
	 	TJsonTagStaticPool pool;  
        ASSERT_EQUALS(0, (int)pool.GetChildCount(NULL));
	}

    void ParseNumber()
    {
        char jsonData[] = "123456";

        TJsonTagStaticPool tagPool;
        TJsonDoc           doc(tagPool);

        doc.LoadFromBuffer(jsonData);
        ASSERT_EQUALS(1, (int)tagPool.GetChildCount(NULL));
        
        TJsonTag* tag = tagPool.GetChild(NULL, 0);
        ASSERT(tag!=NULL);
        ASSERT(tag->GetType()==TJsonTag::JSONNumber);
        ASSERT(tag->GetValueAsString()=="123456");
        ASSERT_EQUALS(123456, tag->GetValueAsLongInt());
        ASSERT_EQUALS(-1, (int) tag->GetValueAsShortInt(-1));
    }

    void ParseString()
    {
        char jsonData[] = "\"123456\"";

        TJsonTagStaticPool tagPool;
        TJsonDoc           doc(tagPool);

        doc.LoadFromBuffer(jsonData);
        ASSERT_EQUALS(1, (int)tagPool.GetChildCount(NULL));

        TJsonTag* tag = tagPool.GetChild(NULL, 0);
        ASSERT(tag->GetType()==TJsonTag::JSONString);
        ASSERT(tag!=NULL);
        ASSERT(tag->GetValueAsString()=="123456");
    }

    void ParseBoolean()
    {
        char jsonData[] = "[false,true]";

        TJsonTagStaticPool tagPool;
        TJsonDoc           doc(tagPool);

        doc.LoadFromBuffer(jsonData);
        ASSERT_EQUALS(1, (int)tagPool.GetChildCount(NULL));

        TJsonTag* tagArray = tagPool.GetChild(NULL, 0);
        ASSERT(tagArray!=NULL);

        TJsonTag* tag1     = tagPool.GetChild(tagArray, 0);
        ASSERT(tag1!=NULL);
        TJsonTag* tag2     = tagPool.GetChild(tagArray, 1);
        ASSERT(tag2!=NULL);
                       
        ASSERT(tag1->GetType()==TJsonTag::JSONBoolean);
        ASSERT(!tag1->GetValueAsBoolean());
        ASSERT(tag2->GetType()==TJsonTag::JSONBoolean);
        ASSERT(tag2->GetValueAsBoolean());

    }



	void OverallTest_Static()
	{
		TJsonTagStaticPool tagPool;
		TJsonDoc           doc(tagPool);		

		char jsonData[] =
		"{\r\n"\
		"	response: {\r\n"\
		"		locations: {\r\n"\
		"			location: [\r\n"\
		"			{\r\n"\
		"				id: 12,\r\n"\
		"					name: \"Hello\",\r\n"\
		"					statusid: 78\r\n"\
		"			},\r\n"\
		"			{\r\n"\
		"				id: \"5\",\r\n"\
		"					name: \"Ann\",\r\n"\
		"					statusid: \"8\"\r\n"\
		"				}\r\n"\
		"			]\r\n"\
		"		},\r\n"\
        "       numbers: [ 10, 20, 30, 40, 50 ],\r\n"\
        "       bools:   [ false, true, true, false, false, null],\r\n"\
        "       names:   [ \"Karel\tEgon\", \"Franta\tJosef\", \"Pepa\"\r\n"\
		"			error: \"404 error\"\r\n"\
		"	}\r\n"\
		"}\r\n";

		doc.LoadFromBuffer(jsonData, sizeof(jsonData));

        TFileStream fs("json.txt", efmCreate);
        doc.SaveToStream(fs);
        fs.Close();
	}
   
    void OverallTest_Dynamic()
    {
        TJsonTagDynamicPool tagPool;
        TJsonDoc           doc(tagPool);		

        char jsonData[] =
            "{\r\n"\
            "	response: {\r\n"\
            "		locations: {\r\n"\
            "			location: [\r\n"\
            "			{\r\n"\
            "				id: 12,\r\n"\
            "					name: \"Hello\",\r\n"\
            "					statusid: 78\r\n"\
            "			},\r\n"\
            "			{\r\n"\
            "				id: \"5\",\r\n"\
            "					name: \"Ann\",\r\n"\
            "					statusid: \"8\"\r\n"\
            "				}\r\n"\
            "			]\r\n"\
            "		},\r\n"\
            "       numbers: [ 10, 20, 30, 40, 50 ],\r\n"\
            "       bools:   [ false, true, true, false, false, null],\r\n"\
            "       names:   [ \"Karel\tEgon\", \"Franta\tJosef\", \"Pepa\"\r\n"\
            "			error: \"404 error\"\r\n"\
            "	}\r\n"\
            "}\r\n";

        doc.LoadFromBuffer(jsonData, sizeof(jsonData));

        TFileStream fs("json1.txt", efmCreate);
        doc.SaveToStream(fs);
        fs.Close();
    }


};

REGISTER_FIXTURE( Test_TJsonDoc);

