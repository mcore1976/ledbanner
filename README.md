# ledbanner

Simple ATTINY85 and daisy chained LED modules MAX7219 based LED banner with scroll

The source code file "main2.c" supports up to <250 characters text length in ATTINY85 due to RAM size constraints.

Upload the code to  ATTINY85 chip by "compileattinyXXX" script, connect all  MAX7219 modules in "daisy chain" ( up to 16 supported )

Connections to be made :

connect first module to ATTINY85

MAX7219 DIN_PIN	to ATTINY85	PB0 pin

MAX7219 CLK_PIN	to ATTINY85	PB1 pin

MAX7219 CS_PIN	to ATTINY85 PB2 pin

Connect 5V, GND to both ATTINY85 & all MAX7219 modules

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
