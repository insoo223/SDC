/*----------------------------------------------------------
File Name: 
	trippleX.cpp
Purpose: 
	Control 4 digits 7 segment LED on tripple 74HC595
Updated: 
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


//-----------------------------------------------
//trippleX :: trippleX()
trippleX :: trippleX(byte latchPin, byte clockPin, byte dataPin)
{
    _l = latchPin;
    _c = clockPin;
    _d = dataPin;

    pinMode(_l,OUTPUT);
    pinMode(_c,OUTPUT);
    pinMode(_d,OUTPUT);

	//for (byte i=0; i<MAXCHIP74HC595; i++)
	//	_curX[i] = 0;
	_curX[0] = 0;
	_curX[1] = 0;
	_curX[2] = 0;

}//trippleX

//-----------------------------------------------
// maintain the most current top, middle, and bottom bytes of tripple 74HC595s
//void trippleX :: updateX(uint8_t topX, uint8_t midX, uint8_t botX)
void trippleX :: updateX(byte* num74HC595)
{
	for (byte i=0; i<MAXCHIP74HC595; i++)
		_curX[i] = num74HC595[i];
	/*
    gCurTopX = topX;
    gCurMidX = midX;
    gCurBotX = botX;
	*/
}//updateX

//-----------------------------------------------
// get the most current top, middle, and bottom bytes of tripple 74HC595s
//void trippleX :: getCurrentX(uint8_t *topX, uint8_t *midX, uint8_t *botX)
void trippleX ::getCurrentX(byte *num74HC595)
{
	for (byte i=0; i<MAXCHIP74HC595; i++)
		num74HC595[i] = _curX[i];
}//getCurrentX

//-----------------------------------------------
// get the most current top, middle, and bottom bytes of tripple 74HC595s
void trippleX ::getCurrentX_legacy(uint8_t *topX, uint8_t *midX, uint8_t *botX)
{
    *topX = gCurTopX;
    *midX = gCurMidX;
    *botX = gCurBotX;
}//getCurrentX_legacy
//-----------------------------------------------
// byte-oriented control for 74HC595
//void trippleX :: ctrlAll(uint8_t topX, uint8_t midX, uint8_t botX)
void trippleX :: ctrlAll(byte *pos74HC595)
{
	byte i;
    
	digitalWrite(_l, LOW);
	for(i=0; i<MAXCHIP74HC595; i++)
		shiftOut(_d, _c, MSBFIRST, pos74HC595[i]);
    digitalWrite(_l, HIGH);
    
    //updateX(topX, midX, botX); //if execute, remnant display on K-unit
}//ctrlAll

//-----------------------------------------------
// byte-oriented control for 74HC595
void trippleX :: ctrlAll_legacy(uint8_t topX, uint8_t midX, uint8_t botX)
{
    digitalWrite(_l, LOW);
    shiftOut(_d, _c, MSBFIRST, botX);
    shiftOut(_d, _c, MSBFIRST, midX);
    shiftOut(_d, _c, MSBFIRST, topX);
    digitalWrite(_l, HIGH);
    
    //updateX(topX, midX, botX); //if execute, remnant display on K-unit
}//ctrlAll_legacy
//-----------------------------------------------
// bit-oriented control for 74HC595
// Control three 74HC595 output eXpansion chips
// created: Thu Sep24,2015
// input args
//    ctrlID: refer "defs.h", 74HC595 pin assignments
//    state: HIGH to set, LOW to clear a corresponding pin of a 74HC595

void trippleX :: ctrlSingle(uint8_t ctrlID, uint8_t state)
{
    //uint8_t bottomByte, middleByte, topByte;
	byte num74HC595[MAXCHIP74HC595];

    uint8_t sectorX, dataX;
    
    // The first bit  in the upper nibble, bit 4, of ctrlID represents
    // which of three 74HC595s, called "sectorX", holds the corresponding pin
    // sectorX 0 for bottom, 1 for middle, and 2 for top sector of three 74HC595s
    sectorX = ctrlID >> SECTOR_BIT_LOCATION_74HC595;
    
    // The bits in the lower nibble, bit3 to bit0, of ctrlID represent
    // the corresponding pin of the 74HC595 chip of ctrlID
    dataX = ctrlID & DATA_BITS_MASK_74HC595;

	for (byte i=0; i<MAXCHIP74HC595; i++)
	{
		if (i == sectorX)
			if  (state == HIGH)
				num74HC595[i] = _curX[i] | _BV(dataX);
			else
				num74HC595[i] = _curX[i] & ~_BV(dataX);
	}

    /*
    switch (sectorX)
    {
        case 0: //bottom 74HC595
            if  (state == HIGH)
                bottomByte = gCurBotX | _BV(dataX);
            else
                bottomByte = gCurBotX & ~_BV(dataX);
            middleByte = gCurMidX;
            topByte = gCurTopX;
            break;
            
        case 1: //middle 74HC595
            bottomByte = gCurBotX;
            if  (state == HIGH)
                middleByte = gCurMidX | _BV(dataX);
            else
                middleByte = gCurMidX & ~_BV(dataX);
            topByte = gCurTopX;
            break;
            
        case 2: //top 74HC595
            bottomByte = gCurBotX;
            middleByte = gCurMidX;
            if  (state == HIGH)
                topByte = gCurTopX | _BV(dataX);
            else
                topByte = gCurTopX & ~_BV(dataX);
            break;
    }//switch(sectorX)
    */


    //update the current bytes of tripple 74HC595s
    //updateX(topByte, middleByte, bottomByte);
	updateX(num74HC595);
            
    //ctrlAll_legacy(gCurTopX, gCurMidX, gCurBotX);
	ctrlAll(num74HC595);

}//ctrlSingle

/*
//-------------------------------------------------
void trippleX :: ctrlSingle_legacy(uint8_t ctrlID, uint8_t state)
{
    uint8_t bottomByte, middleByte, topByte;
    uint8_t sectorX, dataX;
    
    // The first bit  in the upper nibble, bit 4, of ctrlID represents
    // which of three 74HC595s, called "sectorX", holds the corresponding pin
    // sectorX 0 for bottom, 1 for middle, and 2 for top sector of three 74HC595s
    sectorX = ctrlID >> SECTOR_BIT_LOCATION_74HC595;
    
    // The bits in the lower nibble, bit3 to bit0, of ctrlID represent
    // the corresponding pin of the 74HC595 chip of ctrlID
    dataX = ctrlID & DATA_BITS_MASK_74HC595;
    
    switch (sectorX)
    {
        case 0: //bottom 74HC595
            if  (state == HIGH)
                bottomByte = gCurBotX | _BV(dataX);
            else
                bottomByte = gCurBotX & ~_BV(dataX);
            middleByte = gCurMidX;
            topByte = gCurTopX;
            break;
            
        case 1: //middle 74HC595
            bottomByte = gCurBotX;
            if  (state == HIGH)
                middleByte = gCurMidX | _BV(dataX);
            else
                middleByte = gCurMidX & ~_BV(dataX);
            topByte = gCurTopX;
            break;
            
        case 2: //top 74HC595
            bottomByte = gCurBotX;
            middleByte = gCurMidX;
            if  (state == HIGH)
                topByte = gCurTopX | _BV(dataX);
            else
                topByte = gCurTopX & ~_BV(dataX);
            break;
    }//switch(sectorX)
    
    //update the current bytes of tripple 74HC595s
    //updateX(topByte, middleByte, bottomByte);
            
    ctrlAll_legacy(gCurTopX, gCurMidX, gCurBotX);

}//ctrlSingle_legacy
*/
