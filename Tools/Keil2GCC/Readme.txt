Converts Keil MDK-ARM 4 project to ARM GCC makefile.

Syntax:

keil2gcc <soubor Keil projektu> <relative path to makefile>

Example:

keil2gcc D:\Test\MDK-ARM\Test.uvproj ..\

- creates a makefile and stores it to D:\Test directory

or

keil2gcc D:\Test\MDK-ARM\Test.uvproj

- creates a makefile and stores it to D:\Test\MDK-ARM directory