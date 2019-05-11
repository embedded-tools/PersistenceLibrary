#include "UnitTests.h"
#include "MainObject.h"
#include "TMemoryStream.h"
#include "TFileStream.h"

class Test_Persistence : public TestFixture<Test_Persistence>
{
  public:

    MainObject mainObject;

    TEST_FIXTURE( Test_Persistence)
    {
		TEST_CASE( InsertData );

		TEST_CASE( SerializationToXSD );

        TEST_CASE( FullSerializationToXML );
		TEST_CASE( UpdateSerializationToXML );

		TEST_CASE( SerializationFromXML );

		TEST_CASE( FullSerializationToBinaryData );
		TEST_CASE( UpdateSerializationToBinaryData );

		TEST_CASE( SerializationFromBinaryData );
		
    }

	void InsertData()
	{
		ClassD* keypad1 = (ClassD*) mainObject.MainManager.ManagerD.CreateItem(11);
		keypad1->KeypadName = "Keypad01";
		keypad1->Rights = 17;
		mainObject.MainManager.ManagerD.AddItem(keypad1);
		ClassD* keypad2 = (ClassD*) mainObject.MainManager.ManagerD.CreateItem(12);
		keypad2->KeypadName = "Keypad02";
		keypad2->Rights = 18;
		mainObject.MainManager.ManagerD.AddItem(keypad2);
		ClassD* keypad3 = (ClassD*) mainObject.MainManager.ManagerD.CreateItem(13);
		keypad3->KeypadName = "Keypad03";
		keypad3->Rights = 20;
		mainObject.MainManager.ManagerD.AddItem(keypad3);

		ClassB* user1 = (ClassB*) mainObject.MainManager.ManagerB.CreateItem(1);
		mainObject.MainManager.ManagerB.AddItem(user1);
		user1->AssignedKeypads.Add(keypad1);
		user1->AssignedKeypads.Add(keypad2);
		user1->AssignedKeypads.Add(keypad3);

		ClassB* user2 = (ClassB*) mainObject.MainManager.ManagerB.CreateItem(2);
		mainObject.MainManager.ManagerB.AddItem(user2);
		user2->SetState(eisAdded);

		ClassC* group1 = (ClassC*) mainObject.MainManager.ManagerC.CreateItem(3);
		mainObject.MainManager.ManagerC.AddItem(group1);

		user1->AssignedGroup = group1;
	}

	void SerializationToXSD()
	{
		TFileStream* fs = new TFileStream("./TestData/Test.xsd", efmCreate);
		mainObject.SaveToXSD(fs, "DataSet");
		fs->Close();
	}

    void FullSerializationToXML()
    {
		TFileStream* fs = new TFileStream("./TestData/Test.xml", efmCreate);
		mainObject.SaveAllDataToXML(fs, "DataSet");
		fs->Close();
	}

    void UpdateSerializationToXML()
    {
		TFileStream* fs = new TFileStream("./TestData/TestI.xml", efmCreate);
		mainObject.SaveDataUpdateToXML(fs, "DataSet");
		fs->Close();
	}
	
    void SerializationFromXML()
    {
		TFileStream* fs = new TFileStream("./TestData/Test.xml", efmOpenRead);
		mainObject.LoadAllDataFromXML(fs);
		fs->Close();

		TFileStream* fs2 = new TFileStream("./TestData/Test2.xml", efmCreate);
		mainObject.SaveAllDataToXML(fs2, "DataSet");
		fs2->Close();
	}

	void FullSerializationToBinaryData()
	{
		TFileStream* fs = new TFileStream("./TestData/Test.dat", efmCreate);
		mainObject.SaveAllDataToBinary(fs);
		fs->Close();
	}

	void UpdateSerializationToBinaryData()
	{
		TFileStream* fs = new TFileStream("./TestData/TestInc.dat", efmCreate);
		mainObject.SaveDataUpdateToBinary(fs);
		fs->Close();
	}

	void SerializationFromBinaryData()
	{		
		TFileStream* fs = new TFileStream("./TestData/Test.dat", efmOpenRead);
		mainObject.LoadAllDataFromBinary(fs);
		fs->Close();

		TFileStream* fs2 = new TFileStream("./TestData/Test2.dat", efmCreate);
		mainObject.SaveAllDataToBinary(fs2);
		fs2->Close();
	}

};

REGISTER_FIXTURE( Test_Persistence);


