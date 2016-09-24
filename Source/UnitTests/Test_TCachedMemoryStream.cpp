#include "UnitTests.h"

#include "TCachedStream.h"
#include "TMemoryStream.h"

class Test_TCachedMemoryStream : public TestFixture<Test_TCachedMemoryStream>
{
  private:
        TMemoryStream* ms;
        TCachedStream*  cs;


  public:

    TEST_FIXTURE( Test_TCachedMemoryStream)
    {
        TEST_CASE( OpenStream );
        TEST_CASE( WriteByteByByte );
        TEST_CASE( WriteLongByLong );
        TEST_CASE( WriteBigBuffer );
        TEST_CASE( CloseStream );
    }

    void OpenStream()
    {
        ms = new TMemoryStream(16384);
        cs = new TCachedStream(ms,64);

        ASSERT(ms->GetPosition()==0);
        ASSERT(cs->GetPosition()==0);
	}

    void WriteByteByByte()
    {
        int pos1 = cs->GetPosition();
        for(unsigned char i=0; i<129; i++)
        {
            cs->WriteByte(i);
        }                        
        char* buffer = (char*) ms->ToPtr();
        ASSERT(buffer[0]==0);
        ASSERT(buffer[1]==1);
        ASSERT(buffer[30]==30);
        ASSERT(buffer[60]==60);
        ASSERT(buffer[90]==90);

        int pos2 = cs->GetPosition();
        ASSERT((pos2-pos1)==129);
        
    }

    void WriteLongByLong()
    {
        int* buffer = (int*)( ((char*)ms->ToPtr()) + ms->GetPosition());
        
        int pos1 = cs->GetPosition();
        for(long i = 0; i<32; i++)
        {
            cs->WriteLong(i);
        }
        int pos2 = cs->GetPosition();
        ASSERT((pos2-pos1)==128);        
    }

    void WriteBigBuffer()
    {
        char buf1[300];        
        char buf2[300];

        for(unsigned short i = 0; i<300; i++)
        {
            buf1[i] = i + 1;
            buf2[i] = ~i + 2;
        }
        
        int pos1, pos2, pos3;
        for(char c = 0; c<24; c++)
        {
            pos1 = cs->GetPosition();
            cs->WriteBuffer((char*)&buf1, 300);   
            pos2 = cs->GetPosition();
            cs->WriteBuffer((char*)&buf2, 299);               
			pos3 = cs->GetPosition();
			ASSERT((pos2-pos1)==300);
			ASSERT((pos3-pos2)==299);
            
        }
    }

    void CloseStream()
    {
		int csPos1 = cs->GetPosition();
		int msPos1 = ms->GetPosition();
		ASSERT(msPos1<csPos1);
			
        cs->FlushCache();      
		int csPos2 = cs->GetPosition();
		int msPos2 = ms->GetPosition();

		ASSERT(csPos1==csPos2);
		ASSERT(csPos2==msPos2);

        cs->Close();
    }

};

REGISTER_FIXTURE( Test_TCachedMemoryStream);


