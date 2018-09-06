#include "UnitTests.h"
extern "C"
{
    #include "EscapeSequences.h"
};


class Test_EscapeSequences : public TestFixture<Test_EscapeSequences>
{
  public:

    TEST_FIXTURE( Test_EscapeSequences)
    {
        TEST_CASE( AddEscapeSequences );
        TEST_CASE( RemoveEscapeSequences );
    }

    void AddEscapeSequences()
    {
        const char* sourceText = "Name\tSurname\tAge\r\nJohn\tDoe\t33\r\n";
        char outputText[200];
        int  outputTextLength = 0;

        AddEscapedChars(sourceText, (int)strlen(sourceText), outputText, &outputTextLength, sizeof(outputText) );

        int n = (int)strlen(outputText);
        ASSERT_EQUALS(39, n);
        
        int slashCount = 0;
        int crCount  = 0;
        int lfCount  = 0;
        int tabCount = 0;
        for(int i = 0; i<outputTextLength; i++)
        {
            switch(outputText[i])
            {
                case 9:    tabCount++;   break;
                case 10:   lfCount++;    break;
                case 13:   lfCount++;    break;
                case '\\': slashCount++; break;
            }
        }
        ASSERT_EQUALS(0, tabCount);
        ASSERT_EQUALS(0, crCount);
        ASSERT_EQUALS(0, tabCount);
        ASSERT_EQUALS(8, slashCount);                
	}

    void RemoveEscapeSequences()
    {
        const char* sourceText = "Name\\tSurname\\tAge\\r\\nJohn\\tDoe\\t33\\r\\n";
        char outputText[200];
        int  outputTextLength = 0;

        RemoveEscapedChars(sourceText, (int)strlen(sourceText), outputText, &outputTextLength, sizeof(outputText) );

        int n = (int)strlen(outputText);
        ASSERT_EQUALS(31, n);

        int slashCount = 0;
        int crCount  = 0;
        int lfCount  = 0;
        int tabCount = 0;
        for(int i = 0; i<outputTextLength; i++)
        {
            switch(outputText[i])
            {
                case 9:    tabCount++;   break;
                case 10:   lfCount++;    break;
                case 13:   crCount++;    break;
                case '\\': slashCount++; break;
            }
        }
        ASSERT_EQUALS(4, tabCount);
        ASSERT_EQUALS(2, crCount);
        ASSERT_EQUALS(2, lfCount);
        ASSERT_EQUALS(0, slashCount);                
    }

};

REGISTER_FIXTURE( Test_EscapeSequences);



