#include "UnitTests.h"
#include "TCachedWindowsBmpFile.h"
#include "TCanvas.h"

class Test_CachedWindowsBmp : public TestFixture<Test_CachedWindowsBmp>
{
  public:

    TEST_FIXTURE( Test_CachedWindowsBmp)
    {
        TEST_CASE( ConstructorDestructor );
		TEST_CASE( OpenFile );
		TEST_CASE( DrawEllipse );
		
    }

    void ConstructorDestructor()
    {
		TCachedWindowsBmpFile cachedBmp;	
	}

	void OpenFile()
	{
 		TCachedWindowsBmpFile cachedBmp;	
		cachedBmp.OpenFile("./TestData/CachedImage.bmp");
		cachedBmp.CloseFile();
	}

	void DrawEllipse()
	{
		TCachedWindowsBmpFile cachedBmp;	
		TCanvas canvas(&cachedBmp);
		cachedBmp.OpenFile("./TestData/CachedImage.bmp");

		canvas.DrawGradient( 
			                 TRectangle(0,0,cachedBmp.GetWidth(),cachedBmp.GetHeight()),
			                 TColorRGB(255, 0, 0), TColorRGB(0, 255, 0),
							 TColorRGB(0, 0, 255), TColorRGB(255, 255, 255)
 							);
		
		cachedBmp.CloseFile();
	}

};

REGISTER_FIXTURE( Test_CachedWindowsBmp);


