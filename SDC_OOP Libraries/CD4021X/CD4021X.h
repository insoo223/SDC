/*
 CD4021X.h
 SDC
 
 Created by In soo Kim on 10/25/15.
 Release to Public domain
 Updated on 10/29/15
 */

#ifndef CD4021X_h
#define CD4021X_h

#include "Arduino.h"
#include "trippleX.h"

//----------------------------------------------
//--- Pin assignments of latch, clock, and data toCD4021BE input expansion chip
//----------------------------------------------
#define LATCH4021pin 3
#define CLOCK4021pin 4
#define DATA4021pin  5

//----------------------------------------------
//--- Pin assignments of parallel inputs to CD4021BE input expansion chip
//----------------------------------------------
#define PGM_MODEpinCD4021           0
#define TactileButtonPinCD4021      1
#define SlideSwitch2ndPinCD4021     2


class CD4021X
{
    
public:
    CD4021X();
    uint8_t readPGM_MODEslideSW();
    uint8_t readTactileButton();
    uint8_t read2ndSlideSW();

private:
    uint8_t SingleReadCD4021BE(uint8_t pinPosition);
    void readCD4021BE(uint8_t* inputStatus);
    uint8_t shiftIn(int myDataPin, int myClockPin);
    
    uint8_t _l4021, _c4021, _d4021; //latch, clock, data of CD4021BE input expansion chip
   
};
#endif /* CD4021X_h */
