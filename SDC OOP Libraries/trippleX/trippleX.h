/*
  trippleX.h
  SDC

  Created by In soo Kim on 10/24/15.
  Control tripple 74HC595 output expansion chips
 
  Release to Public domain

 Updated on 10/29/15
*/

#ifndef trippleX_h
#define trippleX_h

#include "Arduino.h"

//----------------------------------------------
//--- Pin assignments to direct pins of Arduino
//----------------------------------------------
#define LATCHpin 6
#define CLOCKpin 7
#define DATApin  8

//----------------------------------------------
//--- Pin assignments for tripple 74HC595s
//----------------------------------------------

// Bit location of 74HC595 sector
// A "sector" tells which of three 74HC595 chips holds a corresponding signal: top, middle, or bottom
#define SECTOR_BIT_LOCATION_74HC595   4

// bit mask for a corresponding signal ID
#define DATA_BITS_MASK_74HC595   0x07

// global variable declared at Arduino sketch "globals" file 
extern uint8_t gCurTopX, gCurMidX, gCurBotX;

class trippleX
{
public:
    trippleX();
    void updateX(uint8_t topX, uint8_t midX, uint8_t botX);
    void getCurrentX(uint8_t *topX, uint8_t *midX, uint8_t *botX);
    void ctrlAll(uint8_t topX, uint8_t midX, uint8_t botX);
    void ctrlSingle(uint8_t ctrlID, uint8_t state);
private:
    uint8_t _l, _c, _d;
};


#endif /* trippleX_h */
