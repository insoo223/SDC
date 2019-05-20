/*----------------------------------------------------------
File Name: 
	Four7segX.cpp
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
	May 08, 2019 (Wed) - 
		1) Class instance as call by referency 
		2) iterative logic to disp4digits, disp4chars, disp4digits_UpsideDown,
		3) Migrate global var. to corresponding class public var
			i.e. gNightMode --> nightMode in the class Four7segX
	May 06, 2019 (Mon) - parameterized 74HC595 pins for constructor  
		1) Four7segX(); --> Four7segX(byte A, byte B, byte C, byte D, byte E, byte F, byte G, byte DP,
				byte digitOnes, byte digitTens, byte digitHundreds, byte digitKilos); 
		2) #define DIGIT_K_PIN 3 --> byte _digitKilos; (as well as other digit pins)
	Oct 29, 2015 - optimization 1st round
Created: 
	By Insoo Kim (insoo@hotmail.com) on Oct 24, 2015
Advantage:
Limitation:
Ref:
----------------------------------------------------------*/
#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"

Four7segX :: Four7segX(byte* segPins) //May 9, 2019 
{
    _7segABC_Num[0] = 0b00111111; // DP_IDXGFEDCBA
    _7segABC_Num[1] = 0b00000110;
    _7segABC_Num[2] = 0b01011011;
    _7segABC_Num[3] = 0b01001111;
    _7segABC_Num[4] = 0b01100110;
    _7segABC_Num[5] = 0b01101101;
    _7segABC_Num[6] = 0b01111100;
    _7segABC_Num[7] = 0b00000111;
    _7segABC_Num[8] = 0b01111111;
    _7segABC_Num[9] = 0b01100111;
    _7segABC_Num[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Num_UpsideDown[0] = 0b00111111; //0
    _7segABC_Num_UpsideDown[1] = 0b00110000; //1
    _7segABC_Num_UpsideDown[2] = 0b01011011; //2
    _7segABC_Num_UpsideDown[3] = 0b01111001; //3
    _7segABC_Num_UpsideDown[4] = 0b01110100; //4
    _7segABC_Num_UpsideDown[5] = 0b01101101; //5
    _7segABC_Num_UpsideDown[6] = 0b01100111; //6
    _7segABC_Num_UpsideDown[7] = 0b00111000; //7
    _7segABC_Num_UpsideDown[8] = 0b01111111; //8
    _7segABC_Num_UpsideDown[9] = 0b01111100; //9
    _7segABC_Num_UpsideDown[10] = 0b00000000; //BLANK_IDX (10)
    _7segABC_Num_UpsideDown[11] = 0b10000000; //DP_IDX (11)
    
    _7segABC_Alpha[0] = 0b01110111; //A
    _7segABC_Alpha[1] = 0b01111100; //b //_7segABC_Alpha[1] = 0b01111111; //B
    _7segABC_Alpha[2] = 0b01011000; //c //_7segABC_Alpha[2] = 0b00111001; //C
    _7segABC_Alpha[3] = 0b01011110; //d
    _7segABC_Alpha[4] = 0b01111011; //e
    _7segABC_Alpha[5] = 0b01110001; //F
    _7segABC_Alpha[6] = 0b01101111; //g
    _7segABC_Alpha[7] = 0b01110100; //h
    _7segABC_Alpha[8] = 0b00000100; //i //_7segABC_Alpha[8] = 0b00000110; //I
    _7segABC_Alpha[9] = 0b00001110; //J
    _7segABC_Alpha[10] = 0b01110000; //k
    
    _7segABC_Alpha[11] = 0b00111000; //L
    _7segABC_Alpha[12] = 0b00100011; //m
    _7segABC_Alpha[13] = 0b01010100; //n
    _7segABC_Alpha[14] = 0b01011100; //o //_7segABC_Alpha[14] = 0b00111111; //O
    _7segABC_Alpha[15] = 0b01110011; //p
    _7segABC_Alpha[16] = 0b01100111; //q
    _7segABC_Alpha[17] = 0b01010000; //r //_7segABC_Alpha[17] = 0b01110111; //R
    _7segABC_Alpha[18] = 0b01101101; //S
    _7segABC_Alpha[19] = 0b01111000; //t //_7segABC_Alpha[19] = 0b00000111; //T
    _7segABC_Alpha[20] = 0b00011100; //u
    _7segABC_Alpha[21] = 0b01110010; //v
    _7segABC_Alpha[22] = 0b01001111; //W
    _7segABC_Alpha[23] = 0b01110110; //X
    _7segABC_Alpha[24] = 0b01100110; //y
    _7segABC_Alpha[25] = 0b01001001; //Z
    _7segABC_Alpha[26] = 0b00000000; //blank
    
	for(byte i=0; i<MAXSEG; i++)
		_segPins[i] = segPins[i];

	_nightMode = false;
    //_numDigits = MAXDIG7SEG;
	//_singleDigitDelay = SINGLE_DIGIT_DELAY;
}//Four7segX

void Four7segX :: setNumDigits(byte num)
{
	_numDigits = num;
}//setNumDigits

byte Four7segX :: getNumDigits()
{
	return(_numDigits);
}//getNumDigits

void Four7segX :: setDigitPins(byte* digitPins) // May 9, 2019 
{
	byte i;

	for(i=0; i<_numDigits; i++)
		_digitPins[i] = digitPins[i];
}//setNumDigits

void Four7segX :: setSingleDigitDelay(byte num)
{
	_singleDigitDelay = num;
}//setSingleDigitDelay
byte Four7segX :: getSingleDigitDelay()
{
	return(_singleDigitDelay);
}//getSingleDigitDelay

void Four7segX :: setNightMode(bool mode)
{
	_nightMode = mode;
}//setNightMode
bool Four7segX :: getNightMode()
{
	return(_nightMode);
}//getNightMode

/*----------------------------------------------------------
Function Name: 
	simplePow
Purpose: 
	Calculate N-based power and return as integer
How to:
	1. Get "num" of iteration to multiply by "base"
	2. Do for loop for iteration 
	3. Return the result as integer
Input Arguments: 
	byte num - Exponent of power
	byte base - Base of power 

Output Arguments: 
	Power of base by num
Updated: 
	May 6, 2019
Created: 
	May 6, 2019
Advantage:
	Much less flash footprint comparing with the Arduino standard "pow()"
	For example, in Arduino source code running the following,
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Limitation:
	Still, the manual & fixed routine to decompose an integer,found in "disp4digits_legacy" is less footprint size
	Harder to get it than the manual routine.
Ref:
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/

int Four7segX :: simplePow(byte base, byte num)			
{
	byte i;
	int val=1;
	for(i=0; i<num; i++)
		val *= base;
	return(val);
}//simplePow


/*----------------------------------------------------------
Function Name: 
	decomposeNum
Purpose: 
	Decompose an integer number and save each digit to a corresponding array.
How to:
	1. Get "num" of iteration to multiply by "base"
	2. Do for loop for iteration 
	3. Return the result as integer
Input Arguments: 
	int num - An integer number to be shown at multiple-digit 7 segment LED
	byte* eachDigit - An array to hold the value of each digit of a multiple-digit 7 segment LED

Output Arguments: 
	None
Updated: 
	May 9, 2019
Created: 
	May 8, 2019
Advantage:
	Regardless of how many digits of 7 segment LED, you can easily adapt its function 
		by changing the class var. "_numDigits" by calling setNumDigits (as of May 9, 2019)
		by changing the constant "MAXDIG7SEG" (as of May 8, 2019)
Limitation:
	Still, the manual & fixed routine to decompose an integer,found in "disp4digits_legacy" of a previou version is less footprint size
	Harder to get it than the manual routine.
Ref:
----------------------------------------------------------*/
void  Four7segX :: decomposeNum(int num, byte* eachDigit)
{
	byte i;
	byte arrEle;
	int val;
	byte k;
	byte p;

	/* Following sequence to decompose an integer number has been transformed into a loop
    thousands=num/1000;
    hundreds=(num - thousands*1000)/100;
    tens=(num - thousands*1000 - hundreds*100)/10;
    ones=num % 10;
	*/
	// Loop to determine each digit (May 6, 2019)
	for (i=0; i<_numDigits; i++)
	{
		arrEle = _numDigits-1-i;
		if (i == 0) // MSB position
			val = num;
		else if (i == _numDigits-1) // LSB position
			val = num % 10;
		else
		{
			k = i;
			p = 0;
			while (k > 0)
			{
				//val -= eachDigit[arrEle+1+p] * pow(10, arrEle+1+p);
				val -= eachDigit[arrEle+1+p] * simplePow(10, arrEle+1+p);
				
				k--;
				p++;
			}
			
		}
		//eachDigit[arrEle] = val / pow(10,arrEle);
		eachDigit[arrEle] = val / simplePow(10, arrEle);
		val = num;
	}// for i
}//decomposeNum

/*----------------------------------------------------------
Function Name: 
	dispAllSegs
Purpose: 
	Display every segment on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	trippleX* X - an instance of the class for handling 3 (or multiple) 74HC595 chips
	byte numUnit - digit position of 4 (or multiple) digits 7segment LED
Updated: 
Created: 
	May 9, 2019
Limitation:
Ref:
----------------------------------------------------------*/
void Four7segX :: dispAllSegs(trippleX* X, byte numUnit)
{
  byte m, aByte;
  for(m=0; m<MAXSEG; m++)
  {
	aByte = simplePow(2,_segPins[m]);
    X->ctrlAll_legacy(aByte, _BV(numUnit), 0);
    delay(SEG_DISP_DELAY);
    X->ctrlAll_legacy(0, _BV(numUnit), 0);
    delay(SEG_DISP_DELAY);
  }
}// dispSeg_trippleX  

/*----------------------------------------------------------
Function Name: 
	disp4digits
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	int num - Number to display
	byte pos - Position to display Decimal Point(DP)
	byte duration - Number of iteration for sigle digit duration
Updated: 
	May 8, 2019 (Wed)
		1) Give the Class instance as call-by reference argument, rather than creat another instance in the function.
		--> Save ATmega328P flash footprint by 38 bytes (HEX size from 3172 to 3134).
			By calling test_trippleX_7segNum(); and running _7X.disp4digits(&X, i,0, 50); than _7X.disp4digits(i, 0, 50); in the Arduino source.
		--> Change the calling pattern to the class because of using its instance as call-by reference argument.
			From X.getCurrentX(group595); to X->getCurrentX(group595);
		2)  Use iteration in "Four7segX :: disp4digits" function
		--> Save HEX size by 72 bytes: (HEX size from 3134 to 3062).

	May 6, 2019 (Mon)
Created: 
	Oct 29, 2015
Limitation:
	"disp4digits" function is using decomposition loop which is applicable regardless of number of digits of 7 segment LED,
	But, has bigger footprint in flash size. i.e.
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Ref:
	malloc() not recommended in an embedded system
		https://arduino.stackexchange.com/questions/682/is-using-malloc-and-free-a-really-bad-idea-on-arduino
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/
void Four7segX :: disp4digits(trippleX* X,int num, byte pos, byte duration)
{
    //Decomposition of input arg."num"
	byte eachDigit[MAXDIG7SEG]; //array element 0 for 10^0, 1 for 10^1, 2 for 10^2, 3 for 10^3
	//byte *eachDigit;
	int val;
	byte k;
	byte p;
	byte n;

    //Byte pattern of parallel pins of each 74HC595 chip
	byte group595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 

	//eachDigit = malloc(sizeof(byte) * (_numDigits*2));
	//for(k=0; k<MAXDIG7SEG; k++)
	//	eachDigit[k]=0;

	//Get the number for each digit position
	decomposeNum(num, eachDigit);
    
	//Get the byte patterns of the current 74HC595 parallel pins
	//These will be updated to show "num" on the 4digit7segment LED
	X->getCurrentX(group595);

	//Eliminate a remnant display at the 1000s digit
	//All the segments of the 7segment LEDare connected to the Top 74HC595, so 0x00 is engaged  at the Top 74HC595
	//This fuction is dependent on the schematic
	/*
	group595[MAXCHIP74HC595-1] = 0x00;
	group595[MAXCHIP74HC595-2] = _BV(_digitPins[MAXCHIP74HC595-1]);
	group595[MAXCHIP74HC595-3] = 0;
	X->ctrlAll(group595);
	*/
	//X->ctrlAll_legacy (0x00, _BV(_digitKilos), group595[0]);
	//X->ctrlAll_legacy (0x00, _BV(_digitPins[MAXCHIP74HC595-1]), group595[MAXCHIP74HC595-2]);
    
	//display 1 to _numDigits digit on 7 segment LED
	for(k=0; k<duration; k++)
    {
		//display each digit of 7 segment LED
		for(n=0; n < _numDigits; n++)
		{

			group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num, eachDigit[n]);

			//remove leading zeros for Tens and above
			val=0;
			if (n>0)
			{
				for(p=0; p<_numDigits-n+1; p++)
					val += eachDigit[_numDigits-p];

				if (val == 0)
					group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num, BLANK_IDX);
				else
					group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num, eachDigit[n]);
			}
			//check DP on or off
			if ( ((pos & simplePow(2, n)) >> n) == 1)
				group595[MAXCHIP74HC595-1] |= getTopX_Num(_7segABC_Num, DP_IDX);


			X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], _BV(n) | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
			
			if (_nightMode)
			{
				X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], ~_BV(n) & group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
				delay(NIGHT_BRIGHTNESS_DELAY);
			}

			delay(_singleDigitDelay);
		}//for n

	}//for k

    //eliminate a remnant display in K-unit
	//X->ctrlAll_legacy(0,_BV(_digitKilos)  | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
	//X->ctrlAll_legacy(0,_BV(_digitPins[MAXCHIP74HC595-1])  | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
	
}//disp4digits

/*----------------------------------------------------------
Function Name: 
	setChip595
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. 
	2. 
	3. 
Arguments:
	Output
		byte* getGroup595pattern - byte array for multiple 74HC595
	Input
		trippleX* X - Class instance for multiple 74HC595
		byte* segVal - Array of segment values for numbers
		byte num - Number to be displayed on a sigle digit 
Updated: 
	May 8, 2019 (Wed)
Created: 
	May 13, 2019 (Mon)
Limitation:
Ref:
----------------------------------------------------------*/

void Four7segX :: setChip595(trippleX* X, byte chipID, byte pinID, byte val)
{
	byte grp595[MAXCHIP74HC595];
	
	X->getCurrentX(grp595);
	if (val == 1)
		//grp595[MAXCHIP74HC595-chipID] |= (1<<pinID);
		grp595[MAXCHIP74HC595-chipID] |= _BV(pinID);
	else
		grp595[MAXCHIP74HC595-chipID] &= ~_BV(pinID);

	X->updateX(grp595);

}//setChip595

/*----------------------------------------------------------
Function Name: 
	getGroup595pattern
Purpose: 
	Display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
How to:
	1. 
	2. 
	3. 
Arguments:
	Output
		byte* getGroup595pattern - byte array for multiple 74HC595
	Input
		trippleX* X - Class instance for multiple 74HC595
		byte* segVal - Array of segment values for numbers
		byte num - Number to be displayed on a sigle digit 
Updated: 
	May 8, 2019 (Wed)
Created: 
	May 13, 2019 (Mon)
Limitation:
Ref:
----------------------------------------------------------*/

void Four7segX :: setGroup595pattern(trippleX* X, byte* segVal, byte num, byte digit)
{
	byte i;
	byte chip595;
	byte pin595;
	byte aByte;

	aByte = segVal[num];
	for(i=0; i<MAXSEG; i++)
	{
		//if ( ( (segVal[num] >> i) & 0x01) == 1)
		//if ( ( (0b01001111 >> i ) & 0x01) == 1)
		chip595 = (_segPins[i] >> SECTOR_BIT_LOCATION_74HC595) & 0x0F;
		pin595 = _segPins[i] & 0x0F;
		if ( ( (aByte >> i) & 0x01) == 1)
			setChip595(X, chip595, pin595, 1);
		else
			setChip595(X, chip595, pin595, 0);

	}
	setChip595(X, 2, digit, 1);
}//setGroup595pattern

void Four7segX :: dispMdigits(trippleX* X,int num, byte pos, byte duration)
{
    //Decomposition of input arg."num"
	byte eachDigit[MAXDIG7SEG]; //array element 0 for 10^0, 1 for 10^1, 2 for 10^2, 3 for 10^3
	//byte *eachDigit;
	int val;
	byte k;
	byte p;
	byte d;

    //Byte pattern of parallel pins of each 74HC595 chip
	byte group595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 

	//Get the number for each digit position
	decomposeNum(num, eachDigit);
    
	//Get the byte patterns of the current 74HC595 parallel pins
	//These will be updated to show "num" on the 4digit7segment LED
	//X->getCurrentX(group595);

	//display 1 to _numDigits digit on 7 segment LED
	for(k=0; k<duration; k++)
    {
		//display each digit of 7 segment LED
		for(d=0; d < _numDigits; d++)
		{
			setGroup595pattern(X, _7segABC_Num, eachDigit[d], d);
			//X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], _BV(n) | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
			//group595[MAXCHIP74HC595-1] = 0xFF; //getTopX_Num(_7segABC_Num,2);
			//group595[MAXCHIP74HC595-1] = 0b01011011;
			/*
			group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num,0);
			group595[MAXCHIP74HC595-2] = simplePow(2,d);
			group595[MAXCHIP74HC595-3] = 0x00;
			X->updateX(group595);
			*/

			X->ctrlAll();
			//X->ctrlAll_legacy(group595[MAXCHIP74HC595-1], group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);

			delay(_singleDigitDelay);
		}//for n

	}//for k
	
}//dispMdigits

/*----------------------------------------------------------
Function Name: 
	disp4chars
Purpose: 
	Display a character string on 4 (or multiple) digits 7segment  LED of common cathode type.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	char* str - Character string to display
	byte duration - Number of iteration for sigle digit duration
Updated: 
	May 8, 2019 (Wed)
		1) Give the Class instance as call-by reference argument, rather than creat another instance in the function.
		--> Save ATmega328P flash footprint by 400 bytes (HEX size from 3212 to 2812).
			By calling test_trippleX_7segChar(); and running _7X.disp4chars(&X, "ABCD", 200); than _7X.disp4chars_legacy("ABCD", 200); in the Arduino source.
		--> Change the calling pattern to the class because of using its instance as call-by reference argument.
			From X.getCurrentX(group595); to X->getCurrentX(group595);
		2)  Use iteration in "Four7segX :: disp4digits" function
		--> Save HEX size by ?? bytes: (HEX size from ?? to ??).

	May 6, 2019 (Mon)
Created: 
	Oct 29, 2015
Limitation:
	"disp4digits" function is using decomposition loop which is applicable regardless of number of digits of 7 segment LED,
	But, has bigger footprint in flash size. i.e.
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Ref:
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/
void Four7segX :: disp4chars(trippleX* X, char* str, int duration)
{
    //byte topX, midX, botX;
	byte group595[MAXCHIP74HC595];
	byte k;
	byte n;
    
    //trippleX X;
    
    //X.getCurrentX_legacy(&topX, &midX, &botX);
	X->getCurrentX(group595);
    
    for (k=0; k<duration; k++)
    {
		//display each char of 7 segment LED
		for(n=0; n<_numDigits; n++)
		{
			group595[MAXCHIP74HC595-1] = getTopX_ABC(str[_numDigits-1-n]);
			X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], _BV(n) | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);

			if (_nightMode)
			{
				X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], ~_BV(n) & group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
				delay(NIGHT_BRIGHTNESS_DELAY);
			}

			delay(_singleDigitDelay);
		}//for n
    }//for k

	//eliminate a remnant display in K-unit
    //X->ctrlAll_legacy(0,_BV(_digitKilos)  | group595[1], group595[0]);
	X->ctrlAll_legacy(0,_BV(_digitPins[MAXCHIP74HC595-1])  | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
}//disp4chars

/*----------------------------------------------------------
Function Name: 
	disp4digits_UpsideDown
Purpose: 
	Upsid-down display an integer on 4 (or multiple) digits 7segment LED of common cathode type.
	When using a person's height measure using the ultrasound transducer, you need to hold the SDC upside-down,
		so as the display shoud be upside-down.
How to:
	1. An input "num" will be decomposed as thousands, hundres, tens and ones.
	2. The decomposed numbers will be shown up at each digit position very short time in a round-robin manner.
	3. Human eyes recognize the four digits are lit up simultaneously rather than on & off sequentially.
Arguments: 
	int num - Number to display
	byte pos - Position to display Decimal Point(DP)
	byte duration - Number of iteration for sigle digit duration
Updated: 
	May 8, 2019 (Wed)
		1) Give the Class instance as call-by reference argument, rather than creat another instance in the function.
		--> Save ATmega328P flash footprint by ?? bytes (HEX size from ?? to ??).
			By calling test_trippleX_7segNum_UpsideDown(); and running _7X.disp4digits_UpsideDown(&X, i,0, 50); than _7X.disp4digits(i, 0, 50); in the Arduino source.
		--> Change the calling pattern to the class because of using its instance as call-by reference argument.
			From X.getCurrentX(group595); to X->getCurrentX(group595);
		2)  Use iteration in "Four7segX :: disp4digits_UpsideDown" function
		--> Save HEX size by ?? bytes: (HEX size from ?? to ??).

	May 6, 2019 (Mon)
Created: 
	Oct 29, 2015
Limitation:
	"disp4digits" function is using decomposition loop which is applicable regardless of number of digits of 7 segment LED,
	But, has bigger footprint in flash size. i.e.
		_7X.disp4digits(5678,0, 40); // HEX size: 3172 bytes using decimalPow, 5138 bytes using Arduino std pow
		_7X.disp4digits_legacy(5678,0, 40); // HEX size: 2868 bytes
Ref:
	Arduino power fuction https://www.arduino.cc/reference/en/language/functions/math/pow/
----------------------------------------------------------*/

void Four7segX :: disp4digits_UpsideDown(trippleX* X, int num, int pos, int duration)
{
    //Decomposition of input arg."num"
	byte eachDigit[MAXDIG7SEG]; //array element 0 for 10^0, 1 for 10^1, 2 for 10^2, 3 for 10^3
	//byte *eachDigit;
	int val;
	byte k;
	byte p;
	byte n;
    //Byte pattern of 74HC595 parallel pins
	byte group595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 
    
	//eachDigit = malloc(sizeof(byte) * (_numDigits+1));//HEX 3130 -> 3664 by using malloc than fixed array
	//for(k=0; k<_numDigits+1; k++)
	//	eachDigit[k]=0;

	//Get the number for each digit position
	decomposeNum(num, eachDigit);
    
	//Get the byte patterns of the current 74HC595 parallel pins
	//These will be updated to show "num" on the 4digit7segment LED
	X->getCurrentX(group595);

	//Do not need to eliminate a remnant display at the 1000s digit
	//For some reason, there's NO residual display.
    
	//display 1 to MAXDIG7SEG digit on 7 segment LED
	for(k=0; k<duration; k++)
    {
		//display each digit of 7 segment LED
		for(n=0; n<_numDigits; n++)
		{

			group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num_UpsideDown, eachDigit[n]);

			//remove leading zeros for Tens and above
			//NOT yet removed as of May 8, 2019
			val=0;
			if (n<MAXCHIP74HC595-1)
			{
				for(p=0; p<_numDigits-n+1; p++)
					val += eachDigit[_numDigits-1-p];

				if (val == 0)
					group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num_UpsideDown, BLANK_IDX);
				else
					group595[MAXCHIP74HC595-1] = getTopX_Num(_7segABC_Num_UpsideDown, eachDigit[n]);
			}
			//check DP on or off
			if ( ((pos & simplePow(2, n)) >> n) == 1)
				group595[MAXCHIP74HC595-1] |= getTopX_Num(_7segABC_Num_UpsideDown, DP_IDX);


			X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], _BV(_numDigits-1-n) | group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);

			if (_nightMode)
			{
				X->ctrlAll_legacy (group595[MAXCHIP74HC595-1], ~_BV(n) & group595[MAXCHIP74HC595-2], group595[MAXCHIP74HC595-3]);
				delay(NIGHT_BRIGHTNESS_DELAY);
			}

			delay(_singleDigitDelay);
		}//for n

	}//for k

	//Do not need to eliminate a remnant display at the 1000s digit
	//For some reason, there's NO residual display.

}//disp4digits_UpsideDown


//-------------------------------------------------
byte Four7segX :: getTopX_Num(byte* numArray, byte num)
{
    byte targetByte, topByte=0;
    byte i;
    
    // get A to G & DP_IDX bit pattern for "num"
	targetByte = numArray[num];
    
    // convert 7seg ABC byte into the byte for 74HC595 Q-pin
    for (i=0; i<8; i++)
        if ( (targetByte >> i) & 0x01 )
		//if (targetByte >> i)
            topByte |= 1 << _segPins[i];
    
    return (topByte);
} //getTopX_Num

byte Four7segX :: getTopX_ABC(char C)
{
    byte targetByte, topByte=0;
    byte i, arrIndex;
    
    //check space or blank char
    if ((int)C == 32)
        arrIndex = 26;
    else
        arrIndex = C - 'A';
    // get A to G & DP_IDX bit pattern for "num"
    targetByte = _7segABC_Alpha[arrIndex];
    
    // convert 7seg ABC byte into the byte for 74HC595 Q-pin
    for (i=0; i<8; i++)
        if ( (targetByte >> i) & 0x01 )
            topByte |= 1 << _segPins[i];
    
    return (topByte);
} //getTopX_ABC



