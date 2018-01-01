#include "unittests.h"
#include <stdio.h>
#include "Collections.h"

int main(int argc, char* argv[])
{
    char c;
    int result = TestFixtureFactory::theInstance().runTests() ? 0 : -1;
    scanf("%c", &c);
	return 0;
}