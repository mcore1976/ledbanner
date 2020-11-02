# ledbanner

Simple ATTINY85 and 6 LED modules MAX7219 based LED banner with scroll

The code supports up to ~32 characters text length in ATTINY85 due to RAM size constraints.

upload to ATTINY85 , connect all  MAX7219 modules in "daisy chain" ( up to 16 supported )

Connections to be made :

connect first module to ATTINY85

MAX7219 DIN_PIN	to ATTINY85	PB0 pin

MAX7219 CLK_PIN	to ATTINY85	PB1 pin

MAX7219 CS_PIN	to ATTINY85 PB2 pin

Connect 5V, GND to both ATTINY85 & all MAX7219 modules

In the Source Code "main.c" please set following numbers according to your setup:

// declare number of letters in scrolled text here
#define NUMCHARS		26

// declare number of daisy chained MAX7219 modules here
#define MODULESNUMBER           4


Some portion of source code was borrowed from Łukasz Podkalicki :
https://github.com/lpodkalicki/blog/tree/master/avr/attiny13/040_random_flickering_pixels_on_max7219_8x8_led_display
