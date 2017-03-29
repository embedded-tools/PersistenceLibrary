Converts Keil MDK-ARM 4 project (.uvproj) to ARM GCC makefile.
Supports ARM Cortex MCUs (M0, M3, M4) by ST and NXP.
Tested with:
  NXP LCP11C24
  ST  STM32F010  
  ST  STM32F103
  ST  STM32F207
  ST  STM32F429

Syntax:

keil2gcc <soubor Keil projektu> <relative path to makefile>

Example:

keil2gcc D:\Test\MDK-ARM\Test.uvproj ..\

- creates a makefile and stores it to D:\Test directory

or

keil2gcc D:\Test\MDK-ARM\Test.uvproj

- creates a makefile and stores it to D:\Test\MDK-ARM directory