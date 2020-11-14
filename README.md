# ledbanner

Simple ATTINY85 and daisy chained LED modules MAX7219 based LED banner / LED ticker with scrolling text

The source code file "main2.c" supports :
- text even 5000 characters long (limited by ATTINY FLASH program memory capacity, the program takes up to 2,7KB )
- Daisy chained LED 8x8 matrix up to 32 modules (limited by ATTINY85 512 bytes of SRAM memory).

The "main.c" was a prototype code used for YT video, please do not use it...

Upload the code to  ATTINY85 chip by "compileattinyXXX" script, connect all  MAX7219 modules in "daisy chain".

Connections to be made :

connect first module to ATTINY85, next connect in daisy chain 

MAX7219 DIN_PIN	to ATTINY85	PB0 pin

MAX7219 CLK_PIN	to ATTINY85	PB1 pin

MAX7219 CS_PIN	to ATTINY85 PB2 pin

Connect 5V, GND to both ATTINY85 & all MAX7219 modules

The 5V power source may be from USB powerbak or DC-DC Buck converter (recommended).  In the schematic there is LM7805 voltage stabilizer but I do not recommend it due to high heat generated by this chip. 

In the Source Code "main2.c" / "main.c" please set following numbers according to your setup:

// declare number of letters in scrolled text here

#define NUMCHARS		26

// declare number of daisy chained MAX7219 modules here

#define MODULESNUMBER           4


Link to video showing this LED ticker working : https://www.youtube.com/watch?v=mL6LaHe1yhY

Link to video how to program the ATTINY85 chip with USBASP programmer AVRDUDE app and AVR-GCC environment : https://www.youtube.com/watch?v=7klgyNzZ2TI

compileattiny / compileattiny2  scripts are used for Linux AVR-GCC code compilation & flashing the chip by AVRDUDE

compileattiny.bat / compileattiny2.bat  scripts are used for Windows AVR-GCC code compilation & flashing the chip by AVRDUDE

---------------------------

Some portion of source code was borrowed & modified from Łukasz Podkalicki sources:
https://github.com/lpodkalicki/blog/tree/master/avr/attiny13/040_random_flickering_pixels_on_max7219_8x8_led_display
