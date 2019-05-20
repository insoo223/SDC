
/*
 Four7segX.h
 SDC
 
 Created by In soo Kim on 10/25/15.
 Control 4 digits 7 segment LED on tripple 74HC595
 
 Release to Public domain
 
 Updated on 10/29/15
*/

#ifndef Four7segX_h
#define Four7segX_h

#include "Arduino.h"
#include "trippleX.h"

#define BLANK_IDX 10
#define DP_IDX 11

//----------------------------------------------
//--- Pin assignments of transistors driving common cathode of four digits of 7 segment LEDs.
//--- Each of the defined number represents Q-pin of the middle of tripple 74HC595s
//----------------------------------------------

#define DIGIT_K_PIN  3  // Kilo(1000s) digit control pin
#define DIGIT_H_PIN  2  // Hundres(100s) digit control pin
#define DIGIT_T_PIN  1  // Tens(10s) digit control pin
#define DIGIT_U_PIN  0  // Units(1s) digit control pin

//The minimum continuous time[ms] for the state, HIGH or LOW, of each digit with unnoticeable flickering.
//Four digits look like to be shown simultaneously for their fast round-robin displaying.
//#define SINGLE_DIGIT_DELAY 4 //role replaced by gSingleDigitDelay

//LED birightness is dimed in night time, by off the digit for this duration[ms]
//#define NIGHT_BRIGHTNESS_DELAY 5 //For 16Mhz, 5V Arduino Pro Mini (May 20, 2019)
#define NIGHT_BRIGHTNESS_DELAY 2 //For 8Mhz, 3.3V Arduino Pro Mini (May 20, 2019)

//----------------------------------------------
//--- Global variable declared at Arduino sketch "globals" file
//--- Using global var is NOT recommended to maintain debug-friendly code. Use with much caution!
//----------------------------------------------
// set by RTX.readDS3231time member function
extern bool gNightMode;
// set by RTX.readDS3231time member function
extern byte gSingleDigitDelay;

class Four7segX
{
public:
    Four7segX();
    void disp4digits(int num, int pos, int duration);
    void disp4chars(char* str, int duration);
    void disp4digits_UpsideDown(int num, int pos, int duration);
    
private:
    uint8_t getTopX_Num(uint8_t num);
    uint8_t getTopX_Num_UpsideDown(uint8_t num);
    uint8_t getTopX_ABC(char C);

    //uint8_t _l, _c, _d; //latch, clock, data of trippleX
    
    uint8_t _A, _B, _C, _D, _E, _F, _G, _DP;

    //----------------------------------------------
    //--- bit assignments for 7 segment LED
    //----------------------------------------------
    // A to G & DP pin assignments for common cathode 7 segment LED
    uint8_t _7segABC_Num[12];
    
    uint8_t _7segABC_Num_UpsideDown[12];
    
    // Fixed regardless of schematic
    uint8_t _7segABC_Alpha[27];
    
    // Variable depending on schematic
    // Q-pin assignments for top X in trippleX,
    //  beginning A to G & last one is for DP
    //uint8_t _74HC595pin[8] = {5, 6, 1, 7, 2, 4, 3, 0};  // ver01
    uint8_t _74HC595pin[8];  // ver02
};



#endif /* Four7segX_h */
