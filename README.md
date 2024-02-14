Name:                   
                        klib_io (written in C)

Version:                
                        1.1.0

Contributor(s):         
                        Koby Miller
                        Dr. Jacob Murray

Date last modified:     
                        February 9th, 2024

Description:            
                        A library including useful functions concerning the RealDigital Blackboard, such as functions to use
                        on-board GPIO devices:
                                - LEDs
                                - Buttons
                                - Switches
                                - RGB LEDs
                                - Seven-segment display
                        
                        Memory address macros were made by consulting the reference manual on the RealDigital website:
                                - https://www.realdigital.org/hardware/blackboard

Changelog:              
                        Added functions:
                                - setupPmod(); (for only PMOD B at the moment)
                                - pmodRead();
                                - pmodWrite();
                        Switched parameters where appropriate to 'bool' and 'byte' instead of 'unsigned int'

