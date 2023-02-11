#include "UnitTests.h"
#include "TTextProtocol.h"

class Test_TTextProtocol : public TestFixture<Test_TTextProtocol>
{
  public:

    TEST_FIXTURE( Test_TTextProtocol)
    {
        TEST_CASE( TestName1 );
    }


    void TestName1()
    {
        TTextProtocol prot;
	}

};

REGISTER_FIXTURE( Test_TTextProtocol);


