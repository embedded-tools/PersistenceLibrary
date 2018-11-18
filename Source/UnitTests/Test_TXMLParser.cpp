#include "UnitTests.h"
#include "TXMLCache.h"
#include "TXMLParser.h"


class Test_XMLParser : public TestFixture<Test_XMLParser>
{
  public:

    TEST_FIXTURE( Test_XMLParser)
    {
        TEST_CASE( ParseXMLTags );
		TEST_CASE( BeginTagNotFound);
		TEST_CASE( EndTagNotFound);
		TEST_CASE( UnterminatedTagName);
		TEST_CASE( InvalidAttribute);
		TEST_CASE( UnterminatedAttributeValue);
		TEST_CASE( BeginAndEndTagDiffers );
		TEST_CASE( UnclosedXmlTags );
    }


    void ParseXMLTags()
    {
		TXMLCache xmlCache;		
		TXMLParser parser;

		int n = sizeof(xmlCache);

		const char* xml1 =  "<Helper UID='12345' Type=\"ccType\" SubType='atNone'>\r\n"\
			                "  <Name>John</Name>\r\n"\
							"  <Surname>Doe</Surname>\r\n"\
							"  <Address>Oak 1</Address>\r\n"\
							"  <Town>Brno</Town>\r\n"\
							"  <Encoding>&nbsp;&lt;&gt;&quot;&amp;</Encoding>"\
							"</Helper>";

		parser.SetTalker((TXMLTalkerInterface*)&xmlCache);
		parser.Parse(xml1, (int)strlen(xml1));
		parser.FlushCache();

		ASSERT_EQUALS(3, (int)xmlCache.Attributes.Count());
		ASSERT(xmlCache.Attributes.ContainsKey("UID"));
		ASSERT(xmlCache.Attributes.ContainsKey("Type"));
		ASSERT(xmlCache.Attributes.ContainsKey("SubType"));

		ASSERT_EQUALS(5, (int)xmlCache.Values.Count());
		ASSERT(xmlCache.Values.ContainsKey("Name"));
		ASSERT(xmlCache.Values.ContainsKey("Surname"));
		ASSERT(xmlCache.Values.ContainsKey("Address"));
		ASSERT(xmlCache.Values.ContainsKey("Town"));
		ASSERT(xmlCache.Values.ContainsKey("Encoding"));
		ASSERT(xmlCache.Values["Name"]=="John");
		ASSERT(xmlCache.Values["Surname"]=="Doe");
		ASSERT(xmlCache.Values["Address"]=="Oak 1");
		ASSERT(xmlCache.Values["Town"]=="Brno");
		ASSERT(xmlCache.Values["Encoding"]==" <>\"&");

	}

	void BeginTagNotFound()
	{
		TXMLParser parser;
		const char* xml1 = "Hello!";
		parser.LastError = exeNoError;
		parser.Parse(xml1, (int)strlen(xml1));
		parser.FlushCache();
		ASSERT(parser.LastError == exeBeginTagNotFound);
	}

	void EndTagNotFound()
	{
		TXMLParser parser;
		const char* xml2 = "<Tag>Hello!";
		parser.Parse(xml2, (int)strlen(xml2));
		parser.FlushCache();
		ASSERT(parser.LastError == exeEndTagNotFound);
	}

	void UnterminatedTagName()
	{
		TXMLParser parser;
		const char* xml3 = "<Tag>Hello!<Tag2";
		parser.Parse(xml3, (int)strlen(xml3));
		parser.FlushCache();
		ASSERT(parser.LastError == exeUnterminatedTagName);
	}

	void InvalidAttribute()
	{
		TXMLParser parser;
		const char* xml4 = "<Tag UID=1>Hello!<Tag>";
		parser.Parse(xml4, (int)strlen(xml4));
		parser.FlushCache();
		ASSERT(parser.LastError == exeInvalidAttribute);
	}

	void UnterminatedAttributeValue()
	{
		TXMLParser parser;
		const char* xml5 = "<Tag UID='1>Hello!<Tag>";
		parser.Parse(xml5, (int)strlen(xml5));
		parser.FlushCache();
		ASSERT(parser.LastError == exeUnterminatedAttributeValue);
	}

	void BeginAndEndTagDiffers()
	{
		TXMLParser parser;
		const char* xml6 = "<Tag>Hello!</Tag2>";
		parser.Parse(xml6, (int)strlen(xml6));
		parser.FlushCache();
		ASSERT(parser.LastError == exeBeginAndEndTagDiffers);
	}

	void UnclosedXmlTags()
	{
		TXMLParser parser;
		const char* xml7 = "<Root><Tag>Hello!<Tag></Root>";
		parser.Parse(xml7, (int)strlen(xml7));
		parser.FlushCache();
		ASSERT(parser.LastError == exeBeginAndEndTagDiffers);
	}




};

REGISTER_FIXTURE( Test_XMLParser);


