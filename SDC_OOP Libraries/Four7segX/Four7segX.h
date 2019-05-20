/*----------------------------------------------------------
File Name: 
	Four7segX.h
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
	May 06, 2019 (Mon) - parameterized 74HC595 pins for constructor  
		1) Four7segX(); --> Four7segX(byte A, byte B, byte C, byte D, byte E, byte F, byte G, byte DP,
				byte digitOnes, byte digitTens, byte digitHundreds, byte digitKilos); 
		2) #define DIGIT_K_PIN 3 --> byte _digitKilos; (as well as other digit pins)
	10/29/15 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/

#ifndef Four7segX_h
#define Four7segX_h

#include "Arduino.h"
#include "trippleX.h"

#define BLANK_IDX 10
#define DP_IDX 11
#define MAXDIG7SEG 4 //number of digits in your multiple digit 7 segment LED
//----------------------------------------------
//--- Pin assignments of transistors driving common cathode of four digits of 7 segment LEDs.
//--- Each of the defined number represents Q-pin of the middle of tripple 74HC595s
//----------------------------------------------


//The minimum continuous time[ms] for the state, HIGH or LOW, of each digit with unnoticeable flickering.
//Four digits look like to be shown simultaneously for their fast round-robin displaying.
//#define SINGLE_DIGIT_DELAY 4 //role replaced by gSingleDigitDelay

//LED birightness is dimed in night time, by off the digit for this duration[ms]
#define NIGHT_BRIGHTNESS_DELAY 3

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
	Four7segX(byte A, byte B, byte C, byte D, byte E, byte F, byte G, byte DP,
				byte digitOnes, byte digitTens, byte digitHundreds, byte digitKilos); // May 6, 2019 (int type makes Arduino comile-time error  
	
	void disp4digits(trippleX* X, int num, byte pos, byte duration);
    void disp4chars(trippleX* X, char* str, int duration);
	void disp4digits_UpsideDown(trippleX* X, int num, int pos, int duration);

	bool nightMode;// true to dim the brightness of 7 segment LED
    
private:
	int simplePow(byte base, byte num);			
	void decomposeNum(int num, byte* eachDigit);
    byte getTopX_Num(byte* numArray, byte num);
    byte getTopX_ABC(char C);

   
    byte _A, _B, _C, _D, _E, _F, _G, _DP; // segment control pin

	byte _digitOnes;		// Units(1s) digit control pin
	byte _digitTens;		// Tens(10s) digit control pin
	byte _digitHundreds;  // Hundres(100s) digit control pin
	byte _digitKilos;		// Kilo(1000s) digit control pin

    
    //----------------------------------------------
    //--- bit assignments for 7 segment LED
    //----------------------------------------------
    // A to G & DP pin assignments for common cathode 7 segment LED
    byte _7segABC_Num[12];
    
    byte _7segABC_Num_UpsideDown[12];
    
    // Fixed regardless of schematic
    byte _7segABC_Alpha[27];
    
    // Variable depending on schematic
    // Q-pin assignments for top X in trippleX,
    //  beginning A to G & last one is for DP
    //byte _74HC595pin[8] = {5, 6, 1, 7, 2, 4, 3, 0};  // ver01
    byte _74HC595pin[8];  // ver02
};

#endif /* Four7segX_h */
