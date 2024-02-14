/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        Library Name:           Klib-io (for RealDigital Blackboard)
                                
        Version:                1.1.0 (Header file)

        Contributor(s):         Koby Miller
                                Dr. Jacob Murray

        Date last modified:     February 9th, 2024

        Description:            A library including useful functions concerning the RealDigital Blackboard, such as functions to use
                                on-board GPIO devices:
                                        - LEDs
                                        - Buttons
                                        - Switches
                                        - RGB LEDs
                                        - Seven-segment display
                                
                                Memory address macros were made by consulting the reference manual on the RealDigital website:
                                        - https://www.realdigital.org/hardware/blackboard
                                
                                Please read function descriptions below for use.

        Changelog:              Added functions:
                                        - setupPmod(); (for only PMOD B at the moment)
                                        - pmodRead();
                                        - pmodWrite();
                                Switched parameters where appropriate to 'bool' and 'byte' instead of 'unsigned int'


+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

#ifndef _BLACKBOARD_KLIB_IO
#define _BLACKBOARD_KLIB_IO

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  
        INCLUDE

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

#include <math.h>
#include <stdlib.h>
#include <sys/_intsup.h>
#include <unistd.h>
#include <time.h>

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  
        MACROS

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    Language
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
#define byte char // For when 8-bit number is sufficient
#define true 1
#define false 0
#define bool char // must. have. boolean. This is still 8-bits

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    Switches
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
#define BUTTONS_ADDR 0x41200000	        // Address of Buttons 
#define SWITCHES_ADDR 0x41220000        // Address of Switches 

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    Seven-segment display
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
#define SEVSEG_CTRL_ADDR 0x43C10000     // Address of Seven-segment Confiuration Register
#define SEVSEG_DATA_ADDR 0x43C10004     // Address of Seven-segment Display Data Register 

    #define SEVSEG_CHAR_A 0b0001000         // List of all available characters for seven-segment
    #define SEVSEG_CHAR_B 0b0000011
    #define SEVSEG_CHAR_C 0b1000110
    #define SEVSEG_CHAR_c 0b0100111
    #define SEVSEG_CHAR_D 0b0100001
    #define SEVSEG_CHAR_E 0b0000110
    #define SEVSEG_CHAR_F 0b0001110
    #define SEVSEG_CHAR_G 0b1000010
    #define SEVSEG_CHAR_g 0b0010000
    #define SEVSEG_CHAR_H 0b0001001
    #define SEVSEG_CHAR_h 0b0001011
    #define SEVSEG_CHAR_I 0b1111001
    #define SEVSEG_CHAR_i 0b1111011
    #define SEVSEG_CHAR_J 0b1100001
    #define SEVSEG_CHAR_L 0b1000111
    #define SEVSEG_CHAR_l 0b1001111
    #define SEVSEG_CHAR_N 0b1001000
    #define SEVSEG_CHAR_n 0b0101011
    #define SEVSEG_CHAR_O 0b1000000
    #define SEVSEG_CHAR_o 0b0100011
    #define SEVSEG_CHAR_P 0b0001100
    #define SEVSEG_CHAR_Q 0b0011000
    #define SEVSEG_CHAR_R 0b0101111
    #define SEVSEG_CHAR_S 0b0010010
    #define SEVSEG_CHAR_T 0b0000111
    #define SEVSEG_CHAR_U 0b1000001
    #define SEVSEG_CHAR_u 0b1100011
    #define SEVSEG_CHAR_Y 0b0010001
    #define SEVSEG_CHAR_BLANK 0b1111111
    #define SEVSEG_CHAR_0 0b1000000
    #define SEVSEG_CHAR_1 0b1111001
    #define SEVSEG_CHAR_2 0b0100100
    #define SEVSEG_CHAR_3 0b0110000
    #define SEVSEG_CHAR_4 0b0011001
    #define SEVSEG_CHAR_5 0b0010010
    #define SEVSEG_CHAR_6 0b0000010
    #define SEVSEG_CHAR_7 0b1111000
    #define SEVSEG_CHAR_8 0b0000000
    #define SEVSEG_CHAR_9 0b0010000
    #define SEVSEG_CHAR_DASH 0b0111111
    #define SEVSEG_CHAR_EQUAL 0b0110111

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    LEDs
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
#define LED_ADDR 0x41210000		        // Address of LEDs

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    RBG LEDs
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
#define RGB_EN_BASEADDR 0x43C00000
// increment by 0x10, 16, 0b00010000
    #define RGB_EN_1B_ADDR 0x43C00000             // Address of RGB LED ENABLE #1    BLUE
    #define RGB_EN_1G_ADDR 0x43C00010             //                                 GREEN
    #define RGB_EN_1R_ADDR 0x43C00020             //                                 RED
    #define RGB_EN_2B_ADDR 0x43C00030             // Address of RGB LED ENABLE #2    BLUE
    #define RGB_EN_2G_ADDR 0x43C00040             //                                 GREEN
    #define RGB_EN_2R_ADDR 0x43C00050             //                                 RED

#define RGB_PERIOD_BASEADDR 0x43C00004
// increment by 0x10, 16, 0b00010000
    #define RGB_PERIOD_1B_ADDR 0x43C00004         // Address of RBG LED PERIOD #1    BLUE
    #define RGB_PERIOD_1G_ADDR 0x43C00014         //                                 GREEN
    #define RGB_PERIOD_1R_ADDR 0x43C00024         //                                 RED
    #define RGB_PERIOD_2B_ADDR 0x43C00034         // Address of RBG LED PERIOD #2    BLUE
    #define RGB_PERIOD_2G_ADDR 0x43C00044         //                                 GREEN
    #define RGB_PERIOD_2R_ADDR 0x43C00054         //                                 RED

#define RGB_WIDTH_BASEADDR 0x43C00008 
// increment by 0x10, 16, 0b00010000
    #define RGB_WIDTH_1B_ADDR 0x43C00008          // Address of RBG LED WIDTH #1     BLUE
    #define RGB_WIDTH_1G_ADDR 0x43C00018          //                                 GREEN
    #define RGB_WIDTH_1R_ADDR 0x43C00028          //                                 RED
    #define RGB_WIDTH_2B_ADDR 0x43C00038          // Address of RBG LED WIDTH #2     BLUE
    #define RGB_WIDTH_2G_ADDR 0x43C00048          //                                 GREEN
    #define RGB_WIDTH_2R_ADDR 0x43C00058          //                                 RED

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    PMOD
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

// PMOD B
#define DATA_2 0xE000A048
#define DATA_2_RO 0xE000A068
#define DIR_2 0xE000A284
#define OE_2 0xE000A288

#define PMODB_TOP_MASK 0x0780
#define PMODB_BOTTOM_MASK 0x7800

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  
        FUNCTION PROTOTYPES

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        bitIndex();
		
        int num                 Number to analyze
        unsigned int index      Index of bit in number
        
        bitIndex(0b1010,1) returns 1
        bitIndex(0b1010,2) returns 0

            Used to determine the value of a bit
            of a variable at a specified digit.

            Index starts at 0 and increases 
            left to right (least significant
            to most significant digit).
            
            Returns 1 or 0.
        
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
bool bitIndex(int num, unsigned int place);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        intPow();
		
        int num        Number to raise
        int power      value to raise number to
        
        intPow(2,4) means 2^4
        intPow(7,3) means 7^3

            Used to do exponent operations
        
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int intPow(int num,int power);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getButtonStates();
            
            Returns button states as a 4-bit binary number
            
            Each digit corresponds to 1 of 4 buttons
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int getButtonStates();

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getSwitchStates();
            
            Returns switch states as a 12-bit binary number
            
            Each digit corresponds to 1 of 12 buttons
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int getSwitchStates();

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        enableSevSeg();
		
        unsigned int mode       Determines whether the display operates in 'custom' or 'default' mode
            
            Used to enable the seven-segment display
            in either custom or default mode

            0 for default, 1 for custom
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void enableSevSeg(unsigned int mode);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToSevSeg();
		
        int val                 Value to push to the seven-segment display
        unsigned int mode       Display mode. 0-hexadecimal, 1-decimal
            
            Used to display data on the seven-segment display
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToSevSeg(int val, unsigned int mode);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToSevSegCustom();
		
        const char* str         Input string, only first 4 characters are used    
            
            Used to display strings on the seven-segment display
            Any non-supported character will be blank. If a letter is only available
            in either upper or lower case, both upper and lower case inputs will 
            use the only available one.
            
        Written by Koby Miller
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToSevSegCustom(const char* str);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToLEDs();
		
        int val       Value to push to the LEDs
            
            Used to display data on LEDs
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToLEDs(int val);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        enableRGB();

        unsigned int select     Binary number to enable each LED    

            Used to enable RGB LEDs.
            enableRGB(0b11) enables both LEDs
            enableRGB(0b10) enables only second LED
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void enableRGB(unsigned int select);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToRGB();
		
        unsigned int select         Picks which LED to send to 
        unsigned int color          HEX color to display (0xNNNNNN)
        float bright                0.0 - 1.0 value to determine brightness of LED

            Used to display light on RGB LEDs
            
        Written by Koby Miller
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToRGB(unsigned int select, unsigned int color, float bright);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        setupPmod();
		
        bool io             read (0) or write (1)
        bool row            top (0) or bottom (1) row

            Enables the mode of PMOD B
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void setupPmod(bool io, bool row);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        pmodRead();
		
        bool row            top (0) or bottom (1) row

            Returns a 4-bit value from the PMOD B row selected
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int pmodRead(bool row);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        pmodWrite();
		
        bool row                top (0) or bottom (1) row
        unsigned int data       data to be written to the selected PMOD B row       

            Writes a 4-bit value to the PMOD B row selected
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void pmodWrite(bool row, unsigned int data);

#endif