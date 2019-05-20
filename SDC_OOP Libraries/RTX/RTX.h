/*
 RTX.h
 SDC
 
 Created by In soo Kim on 10/25/15.
 Release to Public domain

 Updated on 10/29/15
 */

#ifndef RTX_h
#define RTX_h

#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"

#include "Wire.h"

#define DS3231_I2C_ADDRESS 0x68

#define DISP_4digit7seg_ITERATION_DATE       50
#define DISP_4digit7seg_ITERATION_DAY        50
#define DISP_4digit7seg_ITERATION_TIME       1000

//The minimum continuous time[ms] for the state, HIGH or LOW, of each digit with unnoticeable flickering.
//Four digits look like to be shown simultaneously for their fast round-robin displaying.
#define SINGLE_DIGIT_DELAY 4 

// global variable declared at Arduino sketch "globals" file
extern bool gNightMode;
extern byte gSingleDigitDelay;

class RTX
{
    
public:
    RTX();
    void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                       dayOfMonth, byte month, byte year);
    void readDS3231time(byte *second,
                        byte *minute,
                        byte *hour,
                        byte *dayOfWeek,
                        byte *dayOfMonth,
                        byte *month,
                        byte *year);
    void dispBrightnessCtrl();
    void displayTime();
    void displayTime_4digit7seg();
    

private:
    byte decToBcd(byte val);
    byte bcdToDec(byte val);

    bool _doneInitEEPROM;
    
    
};
#endif /* RTX_h */
