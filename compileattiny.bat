del  *.elf
del *.o
del  *.hex
avr-gcc -mmcu=attiny85 -std=gnu99 -Wall -Os -o main.elf main.c -w
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avr-size --mcu=attiny85 --format=avr main.elf
avrdude -c usbasp -p attiny85  -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U flash:w:"main.hex":a

