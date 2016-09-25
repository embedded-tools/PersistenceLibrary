#include "UnitTests.h"
#include "txmldoc.h"
#include "txmltagstaticpool.h"

class Test_TXMLDoc : public TestFixture<Test_TXMLDoc>
{
  public:
    
    TXMLTagStaticPool pool;    
	TXMLDoc xmlDoc;
    TString xmlBuffer;

    TEST_FIXTURE( Test_TXMLDoc)
    {
        
        TEST_CASE( LoadXML );
        
        TEST_CASE (SelectNode);
		TEST_CASE (SelectNodes);
		TEST_CASE (CountNodes);
		TEST_CASE (LoadXmlWithHeader1);
		TEST_CASE (LoadXmlWithHeader2);
        TEST_CASE( LoadXmlWithAttributes );        
        TEST_CASE( XmlTagCheck );

		/*
        TEST_CASE( XmlAttributeCheck );
        */
    }

    
    void LoadXML()
    {        
        TXMLDoc xmlDoc(pool);
        xmlBuffer = "<Data>\r\n"\
                    "  <User>\r\n"\
                    "    <UID>1</UID>\r\n"\
                    "    <Name>John</Name>\r\n"\
                    "    <Surname>Doe</Surname>\r\n"\
                    "    <Pin>1234</Pin>\r\n"\
                    "  </User>\r\n"\
                    "  <Group>\r\n"\
                    "    <UID>2</UID>\r\n"\
                    "    <Name>Group1</Name>\r\n"\
                    "    <CanAdd>false</CanAdd>\r\n"\
                    "    <CanEdit>true</CanEdit>\r\n"\
                    "    <CanDelete>false</CanDelete>\r\n"\
                    "  </Group>\r\n"\
                    "  <Schedule>\r\n"\
                    "    <Test />\r\n"\
                    "    <DayOfWeek>Monday</DayOfWeek>\r\n"\
                    "    <Test />\r\n"\
                    "    <DayOfWeek>Tuesday</DayOfWeek>\r\n"\
                    "    <DayOfWeek>Wednesday</DayOfWeek>\r\n"\
                    "    <Test />\r\n"\
                    "    <Test />\r\n"\
                    "    <Test />\r\n"\
                    "    <DayOfWeek>Sunday</DayOfWeek>\r\n"\
                    "  </Schedule>\r\n"\
                    "</Data>\r\n";
        const char* text = xmlBuffer.ToPChar();
                
        bool res = xmlDoc.LoadFromString(xmlBuffer);
		ASSERT(res);
		ASSERT(xmlDoc.Root()!=NULL);
		ASSERT(xmlDoc.Header()==NULL);

        const char* textAfterLoadXML = 
                            "<Data\0\0\n"\
                            "  <User\0\0\n"\
                            "    <UID\01\0/UID>\r\n"\
                            "    <Name\0John\0/Name>\r\n"\
                            "    <Surname\0Doe\0/Name>\r\n"\
                            "    <Pin\01234\0/Pin>\r\n"\
                            "  </User>\r\n"\
                            "  <Group\0\0\n"\
                            "    <UID\02\0/UID>\r\n"\
                            "    <Name\0Group1\0/Name>\r\n"\
                            "    <CanAdd\0false\0/CanAdd>\r\n"\
                            "    <CanEdit\0true\0/CanEdit>\r\n"\
                            "    <CanDelete\0false\0/CanDelete>\r\n"\
                            "  </Group>\r\n"\
                            "  <Schedule\0\r\n"\
                            "    <Test\0/>\r\n"\
                            "    <DayOfWeek\0Monday\0/DayOfWeek>\r\n"\
                            "    <Test\0/>\r\n"\
                            "    <DayOfWeek\0Tuesday\0/DayOfWeek>\r\n"\
                            "    <DayOfWeek\0Wednesday\0/DayOfWeek>\r\n"\
                            "    <Test\0/>\r\n"\
                            "    <Test\0/>\r\n"\
                            "    <Test\0/>\r\n"\
                            "    <DayOfWeek\0Sunday\0/DayOfWeek>\r\n"\
                            "  </Schedule>\r\n"\
                            "</Data>\r\n";
        //remark: xmlBuffer is overwritten and contains multiple zero terminated strings
        //        therefore TXMLDoc class has extremelly low memory requirements 
        //        (because it stores just pointers to original xml buffer)        
		ASSERT(strcmp(text, textAfterLoadXML)==0);
	}

    
    void SelectNode()
    {
		xmlDoc.SetPool(&pool);
		TXMLTag* user1 = xmlDoc.SelectNode("/Data/User");
		ASSERT(user1!=NULL);

		TXMLTag* user1UID = user1->SelectNode("UID");
		ASSERT(user1UID!=NULL);
		ASSERT(user1UID->GetValueAsShortInt()==1);

		TXMLTag* user1Name = user1->SelectNode("Name");
		ASSERT(user1Name!=NULL);
		ASSERT(user1Name->GetValueAsString()=="John");

		TXMLTag* user1Surname = user1->SelectNode("Surname");
		ASSERT(user1Surname!=NULL);
		ASSERT_EQUALS("Doe", user1Surname->GetValue());

		TXMLTag* user1Pin = user1->SelectNode("Pin");
		ASSERT(user1Pin!=NULL);
		ASSERT_EQUALS("1234", user1Pin->GetValue());

    }

	void SelectNodes()
	{
		TXMLTagList* iterSchedules = xmlDoc.SelectNodes("Data/Schedule/DayOfWeek");
		TXMLTag* dayofweek1 = iterSchedules->First();
		TXMLTag* dayofweek2 = iterSchedules->Next();
		TXMLTag* dayofweek3 = iterSchedules->Next();
		TXMLTag* dayofweek4 = iterSchedules->Next();
		TXMLTag* dayofweek5 = iterSchedules->Next();
		TXMLTag* dayofweek6 = iterSchedules->Next();
		TXMLTag* dayofweek7 = iterSchedules->Next();

		ASSERT_EQUALS("Monday",    dayofweek1->GetValue());
		ASSERT_EQUALS("Tuesday",   dayofweek2->GetValue());
		ASSERT_EQUALS("Wednesday", dayofweek3->GetValue());
		ASSERT_EQUALS("Sunday",    dayofweek4->GetValue());
		ASSERT(dayofweek5==NULL);
		ASSERT(dayofweek6==NULL);
		ASSERT(dayofweek7==NULL);  
	}

	void CountNodes()
	{
		TXMLDoc  xmlDoc(pool);
		TXMLTag* user1 = xmlDoc.SelectNode("/Data/User");
		ASSERT(user1!=NULL);
		ASSERT_EQUALS("User", user1->GetName());

		TXMLTag* group1 = xmlDoc.SelectNode("/Data/Group");
		ASSERT(group1!=NULL);
		ASSERT_EQUALS("Group", group1->GetName());

		TXMLTag* schedule1 = xmlDoc.SelectNode("/Data/Schedule");
		ASSERT  (schedule1!=NULL);
		ASSERT_EQUALS("Schedule", schedule1->GetName());
		
		unsigned short n1 = xmlDoc.CountNodes("/Data/Schedule/Test");
		unsigned short n2 = schedule1->CountNodes("Test");
		ASSERT(n1==5);
		ASSERT(n2==5);

		unsigned short n3 = xmlDoc.CountNodes("/Data/Schedule/DayOfWeek");
		unsigned short n4 = schedule1->CountNodes("DayOfWeek");
		ASSERT(n3==4);
		ASSERT(n4==4);
	}

	void LoadXmlWithHeader1()
	{
		xmlBuffer = 
			"<?xml version='1.0'>"\
			"<Data>\r\n"\
			"  <UserManager Id='1' Type='123'>\r\n"\
			"     <PinLength>6</PingLength>\r\n"\
			"     <Users>\r\n"\
			"       <User Id='111' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None'>\r\n"\
			"         <UserGroupID>1</UserGroupID>\r\n"\
			"         <Name test='&quot;Yes&quot;'>&quot;&gt;Naz&amp;dar&lt;&quot;</Name>\r\n"\
			"       </User>\r\n"\
			"       <User Id='112' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None' />\r\n"\
			"    </Users>\r\n"\
			"  </UserManager>\r\n"\
			"  <UserGroupManager Id='2' Type='456'>\r\n"\
			"    <DefaultRights>1234</DefaultRights>\r\n"\
			"    <UserGroups>\r\n"\
			"      <UserGroup Id='1' Type='1234' Name='Group1' Attr1='1' Attr2='2' Attr3='3' Attr4='4' Attr5='5' Attr6='6' Attr7='7' Attr8='8' Attr9='9' Attr10='10' Attr11='11' Attr12='12' Attr13='13' Attr14='14' Attr15='15' />\r\n"\
			"      <UserGroup Id='2' Type='1234' Name='Group2' Attr1='11' Attr2='12' Attr3='13' Attr4='14' Attr5='15' Attr6='16' Attr7='17' Attr8='18' Attr9='19' Attr10='20' Attr11='21' Attr12='22' Attr13='23' Attr14='24' Attr15='25'>\r\n"\
			"      </UserGroup>\r\n"\
			"    </UserGroups>\r\n"\
			"  </UserGroupManager>\r\n"\
			"</Data>\r\n";

		bool res = xmlDoc.LoadFromString(xmlBuffer);
		ASSERT(res);
		ASSERT(xmlDoc.Root()!=NULL);
		ASSERT(xmlDoc.Header()!=NULL);
	}

	void LoadXmlWithHeader2()
	{
		xmlBuffer = 
			"<?xml version='1.0' ?>"\
			"<Data>\r\n"\
			"  <UserManager Id='1' Type='123'>\r\n"\
			"     <PinLength>6</PingLength>\r\n"\
			"     <Users>\r\n"\
			"       <User Id='111' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None'>\r\n"\
			"         <UserGroupID>1</UserGroupID>\r\n"\
			"         <Name test='&quot;Yes&quot;'>&quot;&gt;Naz&amp;dar&lt;&quot;</Name>\r\n"\
			"       </User>\r\n"\
			"       <User Id='112' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None' />\r\n"\
			"    </Users>\r\n"\
			"  </UserManager>\r\n"\
			"  <UserGroupManager Id='2' Type='456'>\r\n"\
			"    <DefaultRights>1234</DefaultRights>\r\n"\
			"    <UserGroups>\r\n"\
			"      <UserGroup Id='1' Type='1234' Name='Group1' Attr1='1' Attr2='2' Attr3='3' Attr4='4' Attr5='5' Attr6='6' Attr7='7' Attr8='8' Attr9='9' Attr10='10' Attr11='11' Attr12='12' Attr13='13' Attr14='14' Attr15='15' />\r\n"\
			"      <UserGroup Id='2' Type='1234' Name='Group2' Attr1='11' Attr2='12' Attr3='13' Attr4='14' Attr5='15' Attr6='16' Attr7='17' Attr8='18' Attr9='19' Attr10='20' Attr11='21' Attr12='22' Attr13='23' Attr14='24' Attr15='25'>\r\n"\
			"      </UserGroup>\r\n"\
			"    </UserGroups>\r\n"\
			"  </UserGroupManager>\r\n"\
			"</Data>";

		bool res = xmlDoc.LoadFromString(xmlBuffer);
		ASSERT(res);
		ASSERT(xmlDoc.Root()!=NULL);
		ASSERT(xmlDoc.Header()!=NULL);
	}

    void LoadXmlWithAttributes()
    {
        xmlBuffer = "<Data>\r\n"\
                    "  <UserManager Id='1' Type='123'>\r\n"\
                    "     <PinLength>6</PingLength>\r\n"\
                    "     <Users>\r\n"\
                    "       <User Id='111' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None'>\r\n"\
                    "         <UserGroupID>1</UserGroupID>\r\n"\
                    "         <Name test='&quot;Yes&quot;'>&quot;&gt;Naz&amp;dar&lt;&quot;</Name>\r\n"\
                    "       </User>\r\n"\
                    "       <User Id='112' Type='Normal' Name='John' Surname='Doe' Address='112 WallStreet' City='Phoenix' State='Arizona' Country='United States' PhoneNumber='0012345678' CanLogin='true' Remark='None' />\r\n"\
                    "    </Users>\r\n"\
                    "  </UserManager>\r\n"\
                    "  <UserGroupManager Id='2' Type='456'>\r\n"\
                    "    <DefaultRights>'1234'</DefaultRights\r\n"\
                    "    <UserGroups>\r\n"\
                    "      <UserGroup Id='1' Type='1234' Name='Group1' Attr1='1' Attr2='2' Attr3='3' Attr4='4' Attr5='5' Attr6='6' Attr7='7' Attr8='8' Attr9='9' Attr10='10' Attr11='11' Attr12='12' Attr13='13' Attr14='14' Attr15='15' />\r\n"\
                    "      <UserGroup Id='2' Type='1234' Name='Group2' Attr1='11' Attr2='12' Attr3='13' Attr4='14' Attr5='15' Attr6='16' Attr7='17' Attr8='18' Attr9='19' Attr10='20' Attr11='21' Attr12='22' Attr13='23' Attr14='24' Attr15='25'>\r\n"\
                    "      </UserGroup>\r\n"\
                    "    </UserGroups>\r\n"\
                    "  </UserGroupManager>\r\n"\
                    "</Data>";

        bool res = xmlDoc.LoadFromString(xmlBuffer);
		ASSERT(res);
		ASSERT(xmlDoc.Root()!=NULL);
		ASSERT(xmlDoc.Header()==NULL);
    }


    void XmlTagCheck()
    {
		TXMLTag* rootTag = xmlDoc.Root();
		ASSERT(rootTag!=NULL);
		ASSERT_EQUALS(2, (long)rootTag->GetChildCount());

		TXMLTag* userManagerTag = rootTag->SelectNode("UserManager");
		ASSERT(userManagerTag!=NULL);
		ASSERT_EQUALS("UserManager", userManagerTag->GetName());
		ASSERT_EQUALS(1, (long)userManagerTag->GetAttributeValueAsShortInt("Id"));
		ASSERT_EQUALS(123, (long)userManagerTag->GetAttributeValueAsShortInt("Type"));

		TXMLTag* pinLengthTag = userManagerTag->SelectNode("PinLength");
		ASSERT(pinLengthTag!=NULL);
		ASSERT_EQUALS("PinLength", pinLengthTag->GetName());
		ASSERT_EQUALS(6, (long)pinLengthTag->GetValueAsShortInt());

		TXMLTagList* iterUsers = userManagerTag->SelectNodes("Users/User");
		TXMLTag* user1Tag = iterUsers->First();

		ASSERT_EQUALS(111, user1Tag->GetAttributeValueAsLongInt("Id"));
		ASSERT_EQUALS("Normal", user1Tag->GetAttributeValue("Type"));
		ASSERT_EQUALS("John", user1Tag->GetAttributeValue("Name"));
		ASSERT_EQUALS("Doe", user1Tag->GetAttributeValue("Surname"));
		ASSERT_EQUALS("112 WallStreet", user1Tag->GetAttributeValue("Address"));
		ASSERT_EQUALS("Phoenix", user1Tag->GetAttributeValue("City"));
		ASSERT_EQUALS("Arizona", user1Tag->GetAttributeValue("State"));
		ASSERT_EQUALS("United States", user1Tag->GetAttributeValue("Country"));
		ASSERT_EQUALS("0012345678", user1Tag->GetAttributeValue("PhoneNumber"));
		ASSERT_EQUALS("true", user1Tag->GetAttributeValue("CanLogin"));
		ASSERT_EQUALS("None", user1Tag->GetAttributeValue("Remark"));

		TXMLTag* user1GroupTag = user1Tag->SelectNode("UserGroupID");
		ASSERT_EQUALS("1", user1GroupTag->GetValue());

		TXMLTag* user1NameTag = user1Tag->SelectNode("Name");
		ASSERT_EQUALS("Name", user1NameTag->GetName());
		const char* user1Name = user1NameTag->GetValue();
		ASSERT_EQUALS("\">Naz&dar<\"", user1Name);
		const char* user1Test = user1NameTag->GetAttributeValue("test");
		ASSERT_EQUALS("\"Yes\"", user1Test);
		
		TXMLTag* user2Tag = iterUsers->Next();
		ASSERT_EQUALS(112, user2Tag->GetAttributeValueAsLongInt("Id"));
		ASSERT_EQUALS("Normal", user2Tag->GetAttributeValue("Type"));
		ASSERT_EQUALS("John", user2Tag->GetAttributeValue("Name"));
		ASSERT_EQUALS("Doe", user2Tag->GetAttributeValue("Surname"));
		ASSERT_EQUALS("112 WallStreet", user2Tag->GetAttributeValue("Address"));
		ASSERT_EQUALS("Phoenix", user2Tag->GetAttributeValue("City"));
		ASSERT_EQUALS("Arizona", user2Tag->GetAttributeValue("State"));
		ASSERT_EQUALS("United States", user2Tag->GetAttributeValue("Country"));
		ASSERT_EQUALS("0012345678", user2Tag->GetAttributeValue("PhoneNumber"));
		ASSERT_EQUALS("true", user2Tag->GetAttributeValue("CanLogin"));
		ASSERT_EQUALS("None", user2Tag->GetAttributeValue("Remark"));


		TXMLTag* user3Tag = iterUsers->Next();
		ASSERT(user3Tag==NULL);

		TXMLTag* userGroupManagerTag = rootTag->SelectNode("UserGroupManager");
		ASSERT(userGroupManagerTag!=NULL);
		ASSERT_EQUALS("UserGroupManager", userGroupManagerTag->GetName());
		ASSERT_EQUALS(2, (long)userGroupManagerTag->GetAttributeValueAsShortInt("Id"));
		ASSERT_EQUALS(456, userGroupManagerTag->GetAttributeValueAsLongInt("Type"));
        
    }

    void XmlAttributeCheck()
    {

    }

};

REGISTER_FIXTURE( Test_TXMLDoc);

