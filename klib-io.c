/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        Library Name:           
								Klib-io (for RealDigital Blackboard)
        
        Version:                
								1.1.1 (Source file)

        Contributor(s):         
								Koby Miller
                                Dr. Jacob Murray

        Date last modified:     
								February 14th, 2024

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

#include "klib-io.h"
#include <sys/_intsup.h>

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
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
bool bitIndex(int num, unsigned int index) {

	// Shift "num" over "index" amount of places, then mask to 1 bit, therefore limited to 1 or 0
	num = num >> (index) & 1;
	
    // Return the masked number
	return num;

}//end bitIndex()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        intPow();
		
        int num        Number to raise
        int power      value to raise number to
        
        intPow(2,4) means 2^4m
        intPow(7,3) means 7^3

            Used to do exponent operations
        
        Written by Koby Miller
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int intPow(int num,int power) {

    int result = 1; //start with 1

    if(power >= 0) { // negative exponents, multiply every iteration
        for(int c = 0;c < power;c++) {
            result *= num;
        }
    } else { // negative exponents, divide every iteration
        for(int c = 0;c < (-1 * power);c++) {
            result /= num;
        }
    }

    return result;
}

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getButtonStates();
            
            Returns button states as a 4-bit binary number
            
            Each digit corresponds to 1 of 4 buttons
            
        Written by Koby Miller
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int getButtonStates() {

	unsigned int button_states = 0;

    // read memory address of buttons
	button_states = *((unsigned int *)BUTTONS_ADDR);

	// bitwise-and to check with 4 digits for 4 buttons
    button_states &= 0b1111;

	return button_states;
}//end getButtonStates()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        getSwitchStates();
            
            Returns switch states as a 12-bit binary number
            
            Each digit corresponds to 1 of 12 buttons
            
        Written by Koby Miller
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int getSwitchStates() {

	unsigned int switch_states = 0;

    // read memory address of switches
	switch_states = *((unsigned int *)SWITCHES_ADDR);

	// bitwise-and to check with 12 digits for 12 buttons
    switch_states &= 0b111111111111;

	return switch_states;
}//end getSwitchStates()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        enableSevSeg();
		
        unsigned int mode       Determines whether the display operates in 'custom' or 'default' mode
            
            Used to enable the seven-segment display
            in either custom or default mode

            0 for default, 1 for custom
            
        Written by Koby Miller
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void enableSevSeg(unsigned int mode) {
    
    // if mode is set to 1, enable custom mode, else, leave it off
    // 1st bit enables, 2nd bit determines mode
    if(mode == 1) {
        *((unsigned int *)SEVSEG_CTRL_ADDR) = 0b11;
    } else {
        *((unsigned int *)SEVSEG_CTRL_ADDR) = 0b01;
    }

    return;
}//end enableSevSeg()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToSevSeg();
		
        int val                 Value to push to the seven-segment display
        unsigned int mode       Display mode. 0-hexadecimal, 1-decimal
            
            Used to display data on the seven-segment display
            
        Written by Koby Miller
        Last modified: February 14th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToSevSeg(int val, unsigned int mode) {
    
    enableSevSeg(0); // ensure sev-seg is in number mode

    unsigned int disp = 0;
    byte c;

    switch(mode) {
        default:
        case 0: //hexadecimal
            
            for(c = 0; c < 4; c++)
            {
                disp |= (val & 0xF) << (c * 8); // Shift over bits based on digit of display
                val >>= 4; // Shift out least significant digit (divide by 2^4)
            }
            
            break;
        
        case 1: //decimal
        
            for(c = 0; c < 4;c++) {
                disp |= (val % 10) << (c*8); // Extract and store ones digit to temporary variable
                val = val / 10;    // Remove ones digit
            }
            
            break;
    }
    
    // Turn of decimal points
    disp |= 0x80808080;

    // Write data to display data memory address
    *((unsigned int *)SEVSEG_DATA_ADDR) = disp;
            
    return;
}//end outputToSevSeg()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToSevSegCustom();
		
        const char* str         Input string, only first 4 characters are used    
            
            Used to display strings on the seven-segment display
            Any non-supported character will be blank. If a letter is only available
            in either upper or lower case, both upper and lower case inputs will 
            use the only available one.
            
        Written by Koby Miller
        Last modified: February 14th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToSevSegCustom(const char* str) {
    
    enableSevSeg(1); // ensure sev-seg is in custom mode

    unsigned int disp = 0; // what will be displayed
    unsigned int temp = 0; // stores what character to shift into sev-seg
    byte c; // iterator

                                                                                                                // [!] big long nasty switch-case comin' up [!]
    
    for(c = 0;c < 4;c++) {
        // Determine how to display indexed letter on sev-seg
        switch(str[c]) {                                                                                        // minimize this line ~
            case 'A':
            case 'a':
                temp = SEVSEG_CHAR_A;
                break;

            case 'B':
            case 'b':
                temp = SEVSEG_CHAR_B;
                break;

            case 'C':
                temp = SEVSEG_CHAR_C;
                break;

            case 'c':
                temp = SEVSEG_CHAR_c;
                break;

            case 'D':
            case 'd':
                temp = SEVSEG_CHAR_D;
                break;

            case 'E':
            case 'e':
                temp = SEVSEG_CHAR_E;
                break;

            case 'F':
            case 'f':
                temp = SEVSEG_CHAR_F;
                break;

            case 'G':
                temp = SEVSEG_CHAR_G;
                break;

            case 'g':
                temp = SEVSEG_CHAR_g;
                break;

            case 'H':
                temp = SEVSEG_CHAR_H;
                break;

            case 'h':
                temp = SEVSEG_CHAR_h;
                break;

            case 'I':
                temp = SEVSEG_CHAR_I;
                break;

            case 'i':
                temp = SEVSEG_CHAR_i;
                break;

            case 'J':
            case 'j':
                temp = SEVSEG_CHAR_J;
                break;

            case 'L':
                temp = SEVSEG_CHAR_L;
                break;

            case 'l':
                temp = SEVSEG_CHAR_l;
                break;

            case 'N':
                temp = SEVSEG_CHAR_N;
                break;

            case 'n':
                temp = SEVSEG_CHAR_n;
                break;

            case 'O':
                temp = SEVSEG_CHAR_O;
                break;

            case 'o':
                temp = SEVSEG_CHAR_o;
                break;

            case 'P':
            case 'p':
                temp = SEVSEG_CHAR_P;
                break;

            case 'Q':
            case 'q':
                temp = SEVSEG_CHAR_Q;
                break;

            case 'R':
            case 'r':
                temp = SEVSEG_CHAR_R;
                break;

            case 'S':
            case 's':
                temp = SEVSEG_CHAR_S;
                break;

            case 'T':
            case 't':
                temp = SEVSEG_CHAR_T;
                break;

            case 'U':
                temp = SEVSEG_CHAR_U;
                break;

            case 'u':
                temp = SEVSEG_CHAR_u;
                break;

            case 'Y':
            case 'y':
                temp = SEVSEG_CHAR_Y;
                break;
                
            case '1':
                temp = SEVSEG_CHAR_1;
                break;
                
            case '2':
                temp = SEVSEG_CHAR_2;
                break;
                
            case '3':
                temp = SEVSEG_CHAR_3;
                break;
                
            case '4':
                temp = SEVSEG_CHAR_4;
                break;
                
            case '5':
                temp = SEVSEG_CHAR_5;
                break;
                
            case '6':
                temp = SEVSEG_CHAR_6;
                break;
                
            case '7':
                temp = SEVSEG_CHAR_7;
                break;
                
            case '8':
                temp = SEVSEG_CHAR_8;
                break;
                
            case '9':
                temp = SEVSEG_CHAR_9;
                break;
                
            case '0':
                temp = SEVSEG_CHAR_0;
                break;
                
            case '-':
                temp = SEVSEG_CHAR_DASH;
                break;
                
            case '=':
                temp = SEVSEG_CHAR_EQUAL;
                break;

            default:
                temp = SEVSEG_CHAR_BLANK;
                break;
        }
        disp |= temp << (24 - c * 8);
    }
    
    // Disable decimal points
    disp |= 0x80808080;
    
    // Send display data to display memory address
    *((unsigned int *)SEVSEG_DATA_ADDR) = disp;

    return;
}//end outputToSevSegCustom()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToLEDs();
		
        int val       Value to push to the LEDs
            
            Used to display data on LEDs
            
        Written by Koby Miller
        Last modified: January 30th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToLEDs(int val) {
    
    // Where LEDs overlap with '1' bits
    val &= 0b1111111111;

    // Write to LEDs' address
	*((unsigned int *)LED_ADDR) = val;
    
    return;
}//end outputToLEDs()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        enableRGB();

        unsigned int select     Binary number to enable each LED    

            Used to enable RGB LEDs.
            enableRGB(0b11) enables both LEDs
            enableRGB(0b10) enables only second LED
            
        Written by Koby Miller
        Last modified: February 14th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void enableRGB(unsigned int select) {
    
    unsigned int rgb[2] = {bitIndex(select,0),bitIndex(select,1)}; //create array regarding the bit values of 'select' variable
    byte c = 0; //iterator

    for(c = 0; c < 3;c++) {
        *((unsigned int *)(RGB_EN_BASEADDR + c * 0x10)) = rgb[0]; 
    }
    
    for(c = 0; c < 3;c++) {
        *((unsigned int *)(RGB_EN_BASEADDR + c * 0x10 + 0x30)) = rgb[1];
    }

    return;
}//end enableRGB()

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        outputToRGB();
		
        unsigned int select         Picks which LED to send to 
        unsigned int color          HEX color to display
        float bright                0.0 - 1.0 value to determine brightness of LED

            Used to display light on RGB LEDs
            
        Written by Koby Miller
        Last modified: February 14th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void outputToRGB(unsigned int select, unsigned int color, float bright) {
    
    const unsigned int offset = select * 0x30; // offset address based on 'select' variable
    byte c = 0; // iterator
    
    // Mask color off to only valid HEX code
    color &= 0xFFFFFF;
    
    for(c = 0;c < 3;c++) {
        // Constant period
        // Offset pointer by 0x10 at a time for each color
        *(unsigned int*)(RGB_PERIOD_BASEADDR + 0x10 * c + offset) = 2550; 

        // Change width based on passed color HEX value
        // Offset pointer by 0x10 at a time for each color
        // Two least significant digits are blue, next two are green, two most significant are red
        *(unsigned int*)(RGB_WIDTH_BASEADDR + 0x10 * c + offset) = (unsigned int)(bright * 10 * ((color >> c * 8) % 256));
    }

    return;
}

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        setupPmod();
		
        bool io             read (0) or write (1)
        bool row            top (0) or bottom (1) row

            Enables the mode of PMOD B
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void setupPmod(bool io, bool row) {

    if(row == 0) {
        if(io == 0) {   // bits 7-10 of DIR_2 for top row should be 0000 for input
            *((unsigned int*)DIR_2) &= ~PMODB_TOP_MASK;
            *((unsigned int*)OE_2) &= ~PMODB_TOP_MASK;

        } else {        // bits 7-10 of DIR_2 for top row should be 1111 for output
            *((unsigned int*)DIR_2) |= PMODB_TOP_MASK;
            *((unsigned int*)OE_2) |= PMODB_TOP_MASK;

        }   

    } else {
        if(io == 0) {   // bits 11-14 of DIR_2 for bopttom row should be 0000 for input
            *((unsigned int*)DIR_2) &= ~PMODB_BOTTOM_MASK;
            *((unsigned int*)OE_2) &= ~PMODB_BOTTOM_MASK;

        } else {        // bits 11-14 of DIR_2 for bottom row should be 1111 for output
            *((unsigned int*)DIR_2) |= PMODB_BOTTOM_MASK;
            *((unsigned int*)OE_2) |= PMODB_BOTTOM_MASK;
            
        }
    }

    return;
}

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        pmodRead();
		
        bool row            top (0) or bottom (1) row

            Returns a 4-bit value from the PMOD B row selected
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
unsigned int pmodRead(bool row) {

    unsigned int data = 0;

    setupPmod(0, row); // ensure input mode

    // actual magic from Dr. Murray I guess
    if(row == 0) { // top row
        data = (*((unsigned int*)DATA_2_RO) & PMODB_TOP_MASK) >> 7; // mask off and shift to appropriate bit position
        
    } else { // bottom row
        data = (*((unsigned int*)DATA_2_RO) & PMODB_BOTTOM_MASK) >> 11; // mask off and shift to appropriate bit position

    }

    return data;
}

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        pmodWrite();
		
        bool row                top (0) or bottom (1) row
        unsigned int data       data to be written to the selected PMOD B row       

            Writes a 4-bit value to the PMOD B row selected
        
        Written by Koby Miller
        Originally written by Dr. Jacob Murray
        Last modified: February 9th, 2024
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
void pmodWrite(bool row, unsigned int data) {

    setupPmod(1, row); // ensure output mode

    // actual magic from Dr. Murray I guess
    if(row == 0) { // top row
        *((unsigned int*) DATA_2) = (*((unsigned int*)DATA_2) & ~PMODB_TOP_MASK) | ((data << 7) & PMODB_TOP_MASK);	

    } else { // bottom row
        *((unsigned int*) DATA_2) = (*((unsigned int*)DATA_2) & ~PMODB_BOTTOM_MASK) | ((data << 11) & PMODB_BOTTOM_MASK);

    }

    return;
}
