# PersistenceLibrary
Set of classes storing basic data types like string, date, time and serialization to file, xml, data compression etc. All classes are designed for using in embeeded systems. In this case embedded system means a system based on 32 bit MCUs like ARM Cortex M3 (with 8kB - 256kB of RAM). Some classes work with even less memory and with 8 bit CPU like Atmel AVR.

In such systems you should follow these rules:

-you cant allocate too big buffer at stack (stack size is often 256 or 512 bytes, therefore you can hardly allocate 512 bytes or even more at stack!!!)

-you cant use std:: namespace!!! This namespace is often not available

-you cant use any helper library like Boost! Bootst includes up to 150MB of source codes. It can never fit in such small ROM.

-you should allocate memory statically whenever it is possible. Use dynamic memory allocation if there really is no other choice. Always make a notice in a description that particular class uses dynamic memory allocation.
