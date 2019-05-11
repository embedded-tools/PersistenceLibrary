#include "UnitTests.h"
#include "TMemoryStream.h"
#include  "TShortString.h"


class Test_TMemoryStream : public TestFixture<Test_TMemoryStream>
{
  public:

    TEST_FIXTURE( Test_TMemoryStream)
    {
        TEST_CASE( TestBinaryData );
        TEST_CASE( TestTextData );
    }


    void TestBinaryData()
    {
        TMemoryStream* fs = new TMemoryStream(128);
        fs->WriteChar('P');
        fs->WriteChar('K');
        fs->WriteByte(1);
        fs->WriteWord(1234);
        fs->WriteUWord(1235);
        fs->WriteInt(12345678);
        fs->WriteUInt(12345679);   
        fs->WriteBinaryText("John Doe");
        
        signed char c1, c2;
        unsigned char c3;
        short c4;
        unsigned short c5;
        int c6;
        unsigned int c7;
        TShortString name;

        fs->Seek(0, esoFromBeginning);
        fs->ReadChar(c1);
        fs->ReadChar(c2);
        fs->ReadByte(c3);
        fs->ReadWord(c4);
        fs->ReadUWord(c5);
        fs->ReadInt(c6);
        fs->ReadUInt(c7);
        fs->ReadBinaryText(&name, name.MaxLength());

        ASSERT(c1=='P');
        ASSERT(c2=='K');
        ASSERT(c3==1);
        ASSERT(c4==1234);
        ASSERT(c5==1235);
        ASSERT(c6==12345678);
        ASSERT(c7==12345679);   
        ASSERT(name=="John Doe");
        delete fs;               
	}

    void TestTextData()
    {
        TMemoryStream* fs = new TMemoryStream(128);
        fs->WriteLine("<Root>");
        fs->WriteLine("  <Array>");
        fs->WriteLine("    <Value>1</Value>");
        fs->WriteLine("    <Value>2</Value>");
        fs->WriteLine("    <Value>3</Value>");
        fs->WriteLine("    <Value>4</Value>");
        fs->WriteLine("  </Array>");
        fs->WriteLine("</Root>");

        TString line;     
        fs->Seek(0, esoFromBeginning);
        fs->ReadLine(line); ASSERT(line=="<Root>");
        fs->ReadLine(line); ASSERT(line=="  <Array>");
        fs->ReadLine(line); ASSERT(line=="    <Value>1</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>2</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>3</Value>");
        fs->ReadLine(line); ASSERT(line=="    <Value>4</Value>");
        fs->ReadLine(line); ASSERT(line=="  </Array>");
        bool res = fs->ReadLine(line); ASSERT(line=="</Root>");
        ASSERT(res);
        res = fs->ReadLine(line);
        ASSERT(!res);
        ASSERT(line.Length()==0);
        
    }


};

REGISTER_FIXTURE( Test_TMemoryStream);


