#include "UnitTests.h"

#include "tlz77.h"
#include "TLZ77Streamed.h"
#include "tmemorystream.h"
#include "stringutils.h"

class Test_LZ77 : public TestFixture<Test_LZ77>
{
  private:
    int dataLength;

  public:    

    TEST_FIXTURE( Test_LZ77 )
    {
        TEST_CASE( Compression );
        TEST_CASE( Decompression );
    }


    void Compression()
    {
        TMemoryStream* ms = new TMemoryStream();
        ASSERT(ms->GetPosition()==0);

		ms->WriteLine("<DataSet>");
		for(int i = 0; i<960; i++)
		{			
			ms->WriteLine("  <User>");
			ms->WritePlainText("    <ID>");
			char ids[20];
			LongIntToStr(i, ids, 20);
			ms->WritePlainText(ids);
			ms->WriteLine("</ID>");
			ms->WritePlainText("    <Name>");
			switch(i%32)
			{
				case  0: ms->WritePlainText("Adam"); break;
				case  1: ms->WritePlainText("Bedrich"); break;
				case  2: ms->WritePlainText("Borivoj"); break;
				case  3: ms->WritePlainText("Bohumil"); break;
				case  4: ms->WritePlainText("Cyril"); break;
				case  5: ms->WritePlainText("David"); break;
				case  6: ms->WritePlainText("Evzen"); break;
				case  7: ms->WritePlainText("Ferdinand"); break;
				case  8: ms->WritePlainText("Frantisek"); break;
				case  9: ms->WritePlainText("Horymir"); break;
				case 10: ms->WritePlainText("Ivan"); break; 
				case 11: ms->WritePlainText("Igor"); break;
				case 12: ms->WritePlainText("Jan"); break;
				case 13: ms->WritePlainText("Jaromir"); break;
				case 14: ms->WritePlainText("Jaroslav"); break;
				case 15: ms->WritePlainText("Karel"); break;
				case 16: ms->WritePlainText("Ladislav"); break;
				case 17: ms->WritePlainText("Martin"); break;
				case 18: ms->WritePlainText("Martinez Chuerez"); break;
				case 19: ms->WritePlainText("Miroslav"); break;
				case 20: ms->WritePlainText("Norbert"); break;
				case 21: ms->WritePlainText("Ota"); break;
				case 22: ms->WritePlainText("Pavel"); break;
				case 23: ms->WritePlainText("Petr"); break;
				case 24: ms->WritePlainText("Radek"); break;
				case 25: ms->WritePlainText("Radovan"); break;
				case 26: ms->WritePlainText("Rostislav"); break;
				case 27: ms->WritePlainText("Svatomir"); break;
				case 28: ms->WritePlainText("Svatopluk"); break;
				case 29: ms->WritePlainText("Teodor"); break;
				case 30: ms->WritePlainText("Tadeas"); break;			
				case 31: ms->WritePlainText("Zdenek"); break;
			}
			ms->WriteLine("</Name>");
			ms->WritePlainText("    <Surname>");
			switch(i%30)
			{
				case 0: ms->WritePlainText("Aulehla"); break;
				case 1: ms->WritePlainText("Blahousek"); break;
				case 2: ms->WritePlainText("Blb"); break;
				case 3: ms->WritePlainText("Borec"); break;
				case 4: ms->WritePlainText("Carda"); break;
				case 5: ms->WritePlainText("Dvorak"); break;
				case 6: ms->WritePlainText("Fristensky"); break;
				case 7: ms->WritePlainText("Horazny"); break;
				case 8: ms->WritePlainText("Martinez Chuerez Ferdinando");  break;
				case 9: ms->WritePlainText("Nehorazny"); break;
				case 10: ms->WritePlainText("Postoloprtovsky"); break;
				case 11: ms->WritePlainText("Prazak"); break;
				case 12: ms->WritePlainText("Prazansky"); break;
				case 13: ms->WritePlainText("Prebral"); break;
				case 14: ms->WritePlainText("Prepral"); break;
				case 15: ms->WritePlainText("Prerovsky"); break;
				case 16: ms->WritePlainText("Prostejovsky"); break;
				case 17: ms->WritePlainText("Priborsky"); break;
				case 18: ms->WritePlainText("Pytloun"); break;
				case 19: ms->WritePlainText("Kren"); break;
				case 20: ms->WritePlainText("Krepinsky"); break;
				case 21: ms->WritePlainText("Pirozinsky"); break;
				case 22: ms->WritePlainText("Pirda"); break;
				case 23: ms->WritePlainText("Resler"); break;
				case 24: ms->WritePlainText("Sip"); break;
				case 25: ms->WritePlainText("Skoda"); break;
				case 26: ms->WritePlainText("Trampovsky"); break;
				case 27: ms->WritePlainText("Zantovsky"); break;
				case 28: ms->WritePlainText("Zdenal"); break;
				case 29: ms->WritePlainText("Zloun"); break;
			}			
			ms->WriteLine("</Surname>");
			ms->WriteLine("  </User>");
		}
		ms->WriteLine("</DataSet>");
        unsigned char* pData = (unsigned char*) ms->ToPtr();
        dataLength = ms->GetPosition();

        FILE* hFile = fopen("LZ77.xml", "wb");
        fwrite(pData, 1, dataLength, hFile);
        fclose(hFile);

        unsigned char* pCompressedData = (unsigned char*)malloc(150000);
        unsigned long compressedDataLength = 150000;
        LZ77Error error = LZ77_Compress(pData, dataLength, pCompressedData, compressedDataLength);
        ASSERT(error==lzeSuccess);

        hFile = fopen("LZ77-1.xml", "wb");
        fwrite(pCompressedData, 1, compressedDataLength, hFile);
        fclose(hFile);

        ms->Close();
    }

    void Decompression()
    {
        unsigned char* pCompressedData = (unsigned char*)malloc(150000);

        FILE* hFile = fopen("LZ77-1.xml", "rb");
        int compressedDataLength = fread(pCompressedData, 1, 150000, hFile);
        fclose(hFile);

        unsigned char* pDecompressedData = (unsigned char*)malloc(150000);
        unsigned long decompressedDataLength = 150000;
        LZ77Error error = LZ77_Decompress(pCompressedData, compressedDataLength, pDecompressedData, decompressedDataLength );
        ASSERT(error==lzeSuccess);

        hFile = fopen("LZ77-2.xml", "wb");
        fwrite(pDecompressedData, 1, decompressedDataLength, hFile);
        fclose(hFile);

        ASSERT_EQUALS(dataLength, decompressedDataLength);
    }

};

REGISTER_FIXTURE( Test_LZ77 );


