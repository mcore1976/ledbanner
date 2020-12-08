// ********************************************************************
// SCROLLING LED TICKER ARRAY from N 8x8 LED MAX7219 modules  
// connected to single ATTINY13 controlling chip
// (c) Adam Loboda '2020 
// find me at adam.loboda@wp.pl
// ********************************************************************


#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

# define	MAX7219_DIN_PIN		PB0
# define	MAX7219_CLK_PIN		PB1
# define	MAX7219_CS_PIN		PB2

#define MAX7219_REG_NOOP                (0x00)
#define MAX7219_REG_DIGIT0              (0x01)
#define MAX7219_REG_DIGIT1              (0x02)
#define MAX7219_REG_DIGIT2              (0x03)
#define MAX7219_REG_DIGIT3              (0x04)
#define MAX7219_REG_DIGIT4              (0x05)
#define MAX7219_REG_DIGIT5              (0x06)
#define MAX7219_REG_DIGIT6              (0x07)
#define MAX7219_REG_DIGIT7              (0x08)
#define MAX7219_REG_DECODEMODE		(0x09)
#define MAX7219_REG_INTENSITY		(0x0A)
#define MAX7219_REG_SCANLIMIT		(0x0B)
#define MAX7219_REG_SHUTDOWN		(0x0C)
#define MAX7219_REG_DISPLAYTEST		(0x0F)

#define MAX7219_DIN_HIGH()              (PORTB |= _BV(MAX7219_DIN_PIN))
#define MAX7219_DIN_LOW()               (PORTB &= ~_BV(MAX7219_DIN_PIN))
#define MAX7219_CLK_HIGH()              (PORTB |= _BV(MAX7219_CLK_PIN))
#define MAX7219_CLK_LOW()               (PORTB &= ~_BV(MAX7219_CLK_PIN))
#define MAX7219_CS_HIGH()               (PORTB |= _BV(MAX7219_CS_PIN))
#define MAX7219_CS_LOW()                (PORTB &= ~_BV(MAX7219_CS_PIN))

// declare number of letters in scrolled text here. Max is 100 letters limited by FLASH size
#define NUMCHARS	        28
// declare number of daisy chained MAX7219 modules here. Max is 4 modules here because of RAM shortage
#define MODULESNUMBER           4
// calculation of buffer length needed for displaying, because every font letter is 8 bytes long
#define BUFLENGTH		(MODULESNUMBER+1)*8

// ENTER YOUR TEXT TO BE DISPLAYED HERE ... ONLY CAPITAL LETTERS AND DIGITS SUPPORTED !!!
const uint8_t  sentence[] PROGMEM = { "    HAPPY NEW YEAR 2021     " };


// ATTINY13 chip has ony 64 bits of SRAM, these buffers consume most of it
// RAM buffer for manipulation and displaying characters on 4x 8x8 LED displays
static uint8_t outputbuf[BUFLENGTH] =
   { 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0, 
     0, 0, 0, 0, 0, 0, 0, 0 
 };

static uint8_t inputbuf[16] =
   { 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0
 };


// ASCII character definition for CP437 font - 8 bytes (rows) per each character
// due to memory constraints of ATTINY13 only some characters available - capital letters and digits

const uint8_t  fontdef[] PROGMEM = 
  { 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' '  ASCII 32

   0x3E, 0x7F, 0x71, 0x59, 0x4D, 0x7F, 0x3E, 0x00, // '0'  ASCII 48
   0x40, 0x42, 0x7F, 0x7F, 0x40, 0x40, 0x00, 0x00, // '1'
   0x62, 0x73, 0x59, 0x49, 0x6F, 0x66, 0x00, 0x00, // '2'
   0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, // '3'
   0x18, 0x1C, 0x16, 0x53, 0x7F, 0x7F, 0x50, 0x00, // '4'
   0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00, 0x00, // '5'
   0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30, 0x00, 0x00, // '6'
   0x03, 0x03, 0x71, 0x79, 0x0F, 0x07, 0x00, 0x00, // '7'
   0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, // '8'
   0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E, 0x00, 0x00, // '9'

   0x7C, 0x7E, 0x13, 0x13, 0x7E, 0x7C, 0x00, 0x00, // 'A'  ASCII 65
   0x41, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, // 'B'
   0x1C, 0x3E, 0x63, 0x41, 0x41, 0x63, 0x22, 0x00, // 'C'
   0x41, 0x7F, 0x7F, 0x41, 0x63, 0x3E, 0x1C, 0x00, // 'D'
   0x41, 0x7F, 0x7F, 0x49, 0x5D, 0x41, 0x63, 0x00, // 'E'
   0x41, 0x7F, 0x7F, 0x49, 0x1D, 0x01, 0x03, 0x00, // 'F'
   0x1C, 0x3E, 0x63, 0x41, 0x51, 0x73, 0x72, 0x00, // 'G'
   0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00, 0x00, // 'H'
   0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00, 0x00, // 'I'
   0x30, 0x70, 0x40, 0x41, 0x7F, 0x3F, 0x01, 0x00, // 'J'
   0x41, 0x7F, 0x7F, 0x08, 0x1C, 0x77, 0x63, 0x00, // 'K'
   0x41, 0x7F, 0x7F, 0x41, 0x40, 0x60, 0x70, 0x00, // 'L'
   0x7F, 0x7F, 0x0E, 0x1C, 0x0E, 0x7F, 0x7F, 0x00, // 'M'
   0x7F, 0x7F, 0x06, 0x0C, 0x18, 0x7F, 0x7F, 0x00, // 'N'
   0x1C, 0x3E, 0x63, 0x41, 0x63, 0x3E, 0x1C, 0x00, // 'O'
   0x41, 0x7F, 0x7F, 0x49, 0x09, 0x0F, 0x06, 0x00, // 'P'
   0x1E, 0x3F, 0x21, 0x71, 0x7F, 0x5E, 0x00, 0x00, // 'Q'
   0x41, 0x7F, 0x7F, 0x09, 0x19, 0x7F, 0x66, 0x00, // 'R'
   0x26, 0x6F, 0x4D, 0x59, 0x73, 0x32, 0x00, 0x00, // 'S'
   0x03, 0x41, 0x7F, 0x7F, 0x41, 0x03, 0x00, 0x00, // 'T'
   0x7F, 0x7F, 0x40, 0x40, 0x7F, 0x7F, 0x00, 0x00, // 'U'
   0x1F, 0x3F, 0x60, 0x60, 0x3F, 0x1F, 0x00, 0x00, // 'V'
   0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F, 0x00, // 'W'
   0x43, 0x67, 0x3C, 0x18, 0x3C, 0x67, 0x43, 0x00, // 'X'
   0x07, 0x4F, 0x78, 0x78, 0x4F, 0x07, 0x00, 0x00, // 'Y'
   0x47, 0x63, 0x71, 0x59, 0x4D, 0x67, 0x73, 0x00, // 'Z'
};



// -----------------------------------------------------------------------------------------
// -------- Modified MAX7219 procedures to handle 'N' character daisy chain set ------------
// ---------Enable CS signal, then send 'N' commands , then disable CS signal   ------------
// -----------------------------------------------------------------------------------------

// writes a single byte to MAX7219 using DIN & CLK line
static void
MAX7219_write(uint8_t value)
{
        uint8_t i;

        __asm("nop");
        for (i = 0; i < 8; ++i, value <<= 1) {
                MAX7219_CLK_LOW();
                __asm("nop");
                if (value & 0x80) {
                        MAX7219_DIN_HIGH();
                } else {
                        MAX7219_DIN_LOW();
                }
                MAX7219_CLK_HIGH();
        }
}


// sends the 2 byte command from table 'commands' to set of N modules MAX7219 connected in the same time
static void
MAX7219_sendN(uint8_t N, uint8_t commands[])
{
        uint8_t  i;

        MAX7219_CS_HIGH();
        for (i=0; i<N; i++)
        {   
           MAX7219_write(commands[(i*2)]);
           MAX7219_write(commands[(i*2)+1]);
        };
        MAX7219_CS_LOW();
        __asm("nop");
        MAX7219_CS_HIGH();
};



// initiates set of 'N' MAX7219 modules at once
static void
MAX7219_initN(uint8_t N)
{
	uint8_t commands[MODULESNUMBER * 2];
        uint8_t  i ; 

        DDRB |= _BV(MAX7219_DIN_PIN)|_BV(MAX7219_CLK_PIN)|_BV(MAX7219_CS_PIN);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_DECODEMODE;
           commands[(i*2)+1] = 0x00;
        };
        MAX7219_sendN(N, commands);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_SCANLIMIT;
           commands[(i*2)+1] = 0x07;
        };
        MAX7219_sendN(N, commands);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_INTENSITY;
           commands[(i*2)+1] = 0x0f;
        };
        MAX7219_sendN(N, commands);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_DISPLAYTEST;
           commands[(i*2)+1] = 0x00;
        };
        MAX7219_sendN(N, commands);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_SHUTDOWN;
           commands[(i*2)+1] = 0x01;
        };
        MAX7219_sendN(N, commands);

        for (i=0; i<N; i++)
        {   
           commands[(i*2)] = MAX7219_REG_INTENSITY;
           commands[(i*2)+1] = 8;
        };
        MAX7219_sendN(N, commands);

};


// -----------------------------------------------------------------
// -------------------------------- MAIN CODE ----------------------
// -----------------------------------------------------------------


int   main(void)
{
     uint8_t  i, j, k;                   // for bitmap manipulation
     uint8_t  letter;                         // current character taken from defined text
     uint8_t  currentcolumn;                        // offset for columns 
     uint8_t  currentstart;                   // offset for current character in flash memory of the chip
     uint8_t  commands[MODULESNUMBER * 2];

 
     // initialize set of N daisy chained modules with MAX7219
     MAX7219_initN(MODULESNUMBER);

     // neverending loop for scrolling
     while (1)
     {
 
	// loop for scrolling 1 times - as many times as NUMCHARS !
       currentstart = 0;

       // here we count sliding window start for scrolling
       while ( currentstart <  ( NUMCHARS - MODULESNUMBER)  )
       {

            //  scroll by 8 columns - single character scroll
            currentcolumn = 0; 

            while (currentcolumn < 8)
              {

                     // DELAY FOR TUNING SCROLLING SPEED 
                     // Assembly code auto-generated
                     // by utility from Bret Mulvey
                     // Delay 24 000 cycles
                     // 20ms at 1.2 MHz

                      asm volatile ( 
                                    "    ldi  r18, 32	\n"
                                    "    ldi  r19, 42	\n"
                                    "1:  dec  r19	\n"
                                    "    brne 1b	\n"
                                    "    dec  r18	\n"
                                    "    brne 1b	\n"
                                    "    nop	\n"
                                   );


                     // convert 'sentence' characters into bitmaps 8x8 by program memory 
                     // table lookup and put it into buffer for further manipulation
                     // we are converting only 2 characters from sliding window to conserve SRAM 
                     // just for single character scroll and then we shift offset


                      // convert rows of N displays into columns ( 90 degrees rotation )
                      for (letter=0; letter< MODULESNUMBER; letter++)
                      {
                      // offset = letter * 8;  // ofset to current character in 8x8 font matrix

                        // j is current character in PROGMEM for manipulation, counting from SPACE 32 ASCII 
                        j = pgm_read_byte(sentence + currentstart + letter );
                        // correction for position in lookup table
                        if (j>64 && j<90)
                              {  j = j-54; }   // correction for capital letters

                        else if (j>47 && j<58)  
                              { j = j-47; }     // correction for numbers

                        else
                            j = 0;             // other chrs interpreted as SPACE


                         // copy 8 rows of this ASCII joined ( k + j ) bitmap to buffer      
                         for(i=0; i<8; i++)
                         { 
                          inputbuf[i] = pgm_read_byte(fontdef + (j*8) + i ) ;
                          };  // end of 'i' loop


                        // proceed with next character in PROGMEM for manipulation, counting from SPACE 32 ASCII 
                        j = pgm_read_byte(sentence + currentstart + letter + 1);
                        // correction for position in lookup table
                        if (j>64 && j<90)
                              {  j = j-54; }   // correction for capital letters

                        else if (j>47 && j<58)  
                              { j = j-47; }     // correction for numbers

                        else
                            j = 0;             // other chrs interpreted as SPACE

                         // copy next 8 rows of this ASCII joined ( k + j ) bitmap to buffer      
                         for(i=8; i<16; i++)
                         { 
                          inputbuf[i] = pgm_read_byte(fontdef + (j*8) + (i-8) ) ;
                          };  // end of 'i' loop

           
                         // transpose single 8x8bit block - matrix by 90 degrees
                         for(int i = 0; i < 8; i++) 
                           {
                              for(int j = 7; j > -1; j--) 
                                {   
  
                                 //    transpose columns to rows
                          outputbuf[i+(letter << 3)] = (outputbuf[i+(letter << 3)] << 1) | (( inputbuf[j + currentcolumn] >> (7 - i)) & 0x01);
                                };  // end of 'j' loop
                            }; // end of 'i'loop
                      
                       }; // end of 'letter' loop 
                 

                     // sending 8 rows data to N character display from 'outputbuf' array       
                      for(i=0; i<8; i++)
                      { 
                      // send bitmap buffer content to N modules at once, first module is last in sequence 
                         for (j=0; j < MODULESNUMBER ; j++)
                            {   
                               commands[(j*2)] = i+1;
                               commands[(j*2)+1] = outputbuf[i + ((MODULESNUMBER - j -1 ) << 3)];
                            };  // end of 'j' loop
                         MAX7219_sendN(MODULESNUMBER, commands);
                       };  // end of 'i' loop

                 currentcolumn++; // increase current column for scrolling

	       }; // end of 'currentcolumn' WHILE LOOP for scrolling single character

          currentstart++;  // increase current position on text to scroll

       };// end of LOOP for 'currentstart'  for scrolling all the text

     }; // END OF NEVERENDING LOOP

}
// end of MAIN function

