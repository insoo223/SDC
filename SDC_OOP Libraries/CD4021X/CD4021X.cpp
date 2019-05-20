/*
 CD4021X.cpp
 SDC
 
 Created by In soo Kim on 10/25/15.
 Release to Public domain
 Updated on 10/29/15
*/

#include "Arduino.h"
#include "CD4021X.h"

//--------------------------------------------------
CD4021X :: CD4021X()
{
    _l4021 = LATCH4021pin;
    _c4021 = CLOCK4021pin;
    _d4021 = DATA4021pin;

    pinMode (_l4021, OUTPUT);
    pinMode (_c4021, OUTPUT);
    pinMode (_d4021, INPUT);
    
}//CD4021X

//--------------------------------------------
uint8_t CD4021X :: readPGM_MODEslideSW()
{
    return (SingleReadCD4021BE(PGM_MODEpinCD4021));
}//readPGM_MODEslideSW

//--------------------------------------------
uint8_t CD4021X :: readTactileButton()
{
    return (SingleReadCD4021BE(TactileButtonPinCD4021));
}//readTactileButton

//--------------------------------------------
uint8_t CD4021X :: read2ndSlideSW()
{
    return (SingleReadCD4021BE(SlideSwitch2ndPinCD4021));
}//read2ndSlideSW

//--------------------------------------------
uint8_t CD4021X :: SingleReadCD4021BE(uint8_t pinPosition)
{
    uint8_t inputStatus=0, bitVal=0;
    
    //Pulse the latch pin:
    //set it to 1 to collect parallel data
    digitalWrite(_l4021,1);
    //set it to 1 to collect parallel data, wait
    delayMicroseconds(20);
    //set it to 0 to transmit data serially
    digitalWrite(_l4021,0);
    
    //while the shift register is in serial mode
    //collect each shift register into a byte
    //the register attached to the chip comes in first
    inputStatus = shiftIn(_d4021, _c4021);
    bitVal = ((inputStatus & (1 << pinPosition)) >> pinPosition) & 0x01;
    return (bitVal);
    
}//SingleReadCD4021BE

//--------------------------------------------
void CD4021X :: readCD4021BE(uint8_t* inputStatus)
{
    //Define variables to hold the data
    //for shift register.
    //starting with a non-zero numbers can help
    //troubleshoot
    //byte switchVar1 = 0x88; // 10001000  //0x48;  //01001000
    //char inputStatus = 0x88; // 10001000;
    
    //Pulse the latch pin:
    //set it to 1 to collect parallel data
    digitalWrite(_l4021,1);
    //set it to 1 to collect parallel data, wait
    delayMicroseconds(20);
    //set it to 0 to transmit data serially
    digitalWrite(_l4021,0);
    
    //while the shift register is in serial mode
    //collect each shift register into a byte
    //the register attached to the chip comes in first
    *inputStatus = shiftIn(_d4021, _c4021);
    
}//readCD4021BE

//---------------------------------------------------------
////// ----------------------------------------shiftIn function
///// just needs the location of the data pin and the clock pin
///// it returns a byte with each bit in the byte corresponding
///// to a pin on the shift register. leftBit 7 = Pin 7 / Bit 0= Pin 0
uint8_t CD4021X :: shiftIn(int myDataPin, int myClockPin)
{
    int i;
    int temp = 0;
    int pinState;
    uint8_t myDataIn = 0;
    
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, INPUT);
    //we will be holding the clock pin high 8 times (0,..,7) at the
    //end of each time through the for loop
    
    //at the begining of each loop when we set the clock low, it will
    //be doing the necessary low to high drop to cause the shift
    //register's DataPin to change state based on the value
    //of the next bit in its serial information flow.
    //The register transmits the information about the pins from pin 7 to pin 0
    //so that is why our function counts down
    for (i=7; i>=0; i--)
    {
        digitalWrite(myClockPin, 0);
        delayMicroseconds(0.2);
        temp = digitalRead(myDataPin);
        if (temp) {
            pinState = 1;
            //set the bit to 0 no matter what
            myDataIn = myDataIn | (1 << i);
        }
        else {
            //turn it off -- only necessary for debuging
            //print statement since myDataIn starts as 0
            pinState = 0;
        }
        
        //Debuging print statements
        //Serial.print(pinState);
        //Serial.print("     ");
        //Serial.println (dataIn, BIN);
        
        digitalWrite(myClockPin, 1);
        
    }
    //debuging print statements whitespace
    //Serial.println();
    //Serial.println(myDataIn, BIN);
    return myDataIn;
}//shiftIn