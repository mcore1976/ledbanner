del *.elf
del *.o
del *.hex
avr-gcc -mmcu=attiny85 -std=gnu99 -Wall -Os -o main2.elf main2.c -w
avr-objcopy -j .text -j .data -O ihex main2.elf main2.hex
avr-size --mcu=attiny85 --format=avr main2.elf
avrdude -c usbasp -p attiny85  -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U flash:w:"main2.hex":a

