#include "UnitTests.h"

class Test_ClassName : public TestFixture<Test_ClassName>
{
  public:

    TEST_FIXTURE( Test_ClassName)
    {
        TEST_CASE( TestName1 );
    }


    void TestName1()
    {

	}

};

//REGISTER_FIXTURE( Test_ClassName);


