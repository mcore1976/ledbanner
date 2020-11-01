# ledbanner

Simple ATTINY85 and 6 LED modules MAX7219 based LED banner with scroll
The code supports up to 20 characters text length.

upload to ATTINY85 , connect all 6 MAX7219 modules in "daisy chain"
connect first module to ATTINY85

MAX7219 DIN_PIN	to ATTINY85	PB0 pin
MAX7219 CLK_PIN	to ATTINY85	PB1 pin
MAX7219 CS_PIN	to ATTINY85 PB2 pin

Connect 5V, GND to both ATTINY85 & all MAX7219 modules

