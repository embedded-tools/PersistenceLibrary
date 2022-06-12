#include "UnitTests.h"

extern "C"
{
	#include "sha1.h"
}

class Test_Sha1 : public TestFixture<Test_Sha1>
{
  public:

    TEST_FIXTURE( Test_Sha1 )
    {
		TEST_CASE( UInt32TypeCheck );

		TEST_CASE( Sha1_Init );
        TEST_CASE( CalculateTextSha1 );
		TEST_CASE( CalculateLongTextSha1 );
		TEST_CASE( CalculateBinaryDataSha1 );
    }

	void UInt32TypeCheck()
	{
		ASSERT_EQUALS(4, (int) sizeof(uint32_t));
	}

	void Sha1_Init()
	{
	    const char* text = "";
		int         textLength  = strlen(text);
	    const char* hash  = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
	    uint8_t result[20];
	    char hexresult[41];

		Sha1(text, textLength, result);

		for(int i = 0; i<sizeof(result); i++)
		{
			sprintf(hexresult + i*2, "%02x", result[i]);
		}	
		ASSERT(strcmp(hash, hexresult) == 0);
	}

    void CalculateTextSha1()
    {
	    const char* text = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
		int         textLength  = strlen(text);
	    const char* hash  = "84983e441c3bd26ebaae4aa1f95129e5e54670f1";


	    uint8_t result[20];	    
		Sha1(text, textLength, result);

		char hexresult[41];
		for(int i = 0; i<sizeof(result); i++)
		{
			sprintf(hexresult + i*2, "%02x", result[i]);
		}	
		ASSERT(strcmp(hash, hexresult) == 0);
    }

	void CalculateLongTextSha1()
	{
		const char* string1 = "abcdefghbcdefghicdefghijdefghijk";
		const char* string2 = "efghijklfghijklmghijklmnhijklmno";
		const char* string3 = "ijklmnopjklmnopqklmnopqrlmnopqrs";
		const char* string4 = "mnopqrstnopqrstu";

		const char* hash    = "a49b2446a02c645bf419f995b67091253a04a259";

	    uint8_t result[20];	    

		Sha1_CTX ctx;
	    Sha1Init(&ctx);
	    Sha1Update( &ctx, (const uint8_t*)string1, strlen(string1) );
		Sha1Update( &ctx, (const uint8_t*)string2, strlen(string2) );
		Sha1Update( &ctx, (const uint8_t*)string3, strlen(string3) );
		Sha1Update( &ctx, (const uint8_t*)string4, strlen(string4) );
	    Sha1Final ( &ctx, result);

		char hexresult[41];
		for(int i = 0; i<sizeof(result); i++)
		{
			sprintf(hexresult + i*2, "%02x", result[i]);
		}	
		ASSERT(strcmp(hash, hexresult) == 0);
	}

	void CalculateBinaryDataSha1()
	{
		uint8_t binarydata[256];
		for(int i = 0; i < 256; i++)
		{
			binarydata[i] = (uint8_t)i;
		}
		const char* hash = "4916d6bdb7f78e6803698cab32d1586ea457dfc8";

		uint8_t result[20];

		Sha1_CTX ctx;
	    Sha1Init(&ctx);
	    Sha1Update( &ctx, (const uint8_t*)binarydata, sizeof(binarydata) );
		Sha1Final ( &ctx, result);
		
	    char hexresult[41];
		for(int i = 0; i<sizeof(result); i++)
		{
			sprintf(hexresult + i*2, "%02x", result[i]);
		}	
		ASSERT(strcmp(hash, hexresult) == 0);


	}


};

REGISTER_FIXTURE( Test_Sha1 );


