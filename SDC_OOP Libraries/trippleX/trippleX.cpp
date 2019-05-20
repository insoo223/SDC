/*
 
 trippleX.cpp
 SDC
 
 Created by In soo Kim on 10/24/15.
 Release to Public domain
 
 Updated on 10/29/15
*/

#include "Arduino.h"
#include "trippleX.h"


//-----------------------------------------------
trippleX :: trippleX()
{
    _l = LATCHpin;
    _c = CLOCKpin;
    _d = DATApin;

    pinMode(_l,OUTPUT);
    pinMode(_c,OUTPUT);
    pinMode(_d,OUTPUT);
}//trippleX

//-----------------------------------------------
// maintain the most current top, middle, and bottom bytes of tripple 74HC595s
void trippleX :: updateX(uint8_t topX, uint8_t midX, uint8_t botX)
{
    gCurTopX = topX;
    gCurMidX = midX;
    gCurBotX = botX;
}//updateX

//-----------------------------------------------
// get the most current top, middle, and bottom bytes of tripple 74HC595s
void trippleX :: getCurrentX(uint8_t *topX, uint8_t *midX, uint8_t *botX)
{
    *topX = gCurTopX;
    *midX = gCurMidX;
    *botX = gCurBotX;
}//getCurrentX

//-----------------------------------------------
// byte-oriented control for 74HC595
void trippleX :: ctrlAll(uint8_t topX, uint8_t midX, uint8_t botX)
{
    digitalWrite(_l, LOW);
    shiftOut(_d, _c, MSBFIRST, botX);
    shiftOut(_d, _c, MSBFIRST, midX);
    shiftOut(_d, _c, MSBFIRST, topX);
    digitalWrite(_l, HIGH);
    
    //updateX(topX, midX, botX); //if execute, remnant display on K-unit
}//ctrlAll

//-----------------------------------------------
// bit-oriented control for 74HC595
// Control three 74HC595 output eXpansion chips
// created: Thu Sep24,2015
// input args
//    ctrlID: refer "defs.h", 74HC595 pin assignments
//    state: HIGH to set, LOW to clear a corresponding pin of a 74HC595
void trippleX :: ctrlSingle(uint8_t ctrlID, uint8_t state)
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
    updateX(topByte, middleByte, bottomByte);
            
    ctrlAll(gCurTopX, gCurMidX, gCurBotX);

}//ctrlSingle

//-------------------------------------------------
