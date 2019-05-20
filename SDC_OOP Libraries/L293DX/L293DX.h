/*
  L293DX.h
  SDC

  Created by In soo Kim on 10/24/15.
  Release to Public domain

 Updated on 10/29/15
*/

#ifndef L293DX_h
#define L293DX_h

#include "Arduino.h"
#include "trippleX.h"

#define NUM_MOTOR 2
#define NUM_MOTOR_POLE (NUM_MOTOR*2)

//----------------------------------------------
//--- Pin assignments of L293D motor driving chip
//----------------------------------------------
//#define EN1pin 10   // Arduino D10 pin
//#define EN2pin 9  // Arduino D9 pin

// bottom 74HC595 pin
/*
#define MLPpin 0
#define MLNpin 1
#define MRNpin 2
#define MRPpin 3
*/

class L293DX
{
public:
    L293DX(byte* enable, byte* motorPole);
    void fwd(trippleX* X, int duration);
	/*
    void bwd(int duration);
    void stop(int duration);
    void turnLeft(int turnDuration, int stopAfterTurnDuration);
    void turnRight(int turnDuration, int stopAfterTurnDuration);
    void turnClk(int turnDuration, int stopAfterTurnDuration);
    void turnAntClk(int turnDuration, int stopAfterTurnDuration);
    void LFwd(int duration);
    void LBwd(int duration);
    void LStop(int duration);
    void RFwd(int duration);
    void RBwd(int duration);
    void RStop(int duration);
    */
    
private:
    byte _enable[NUM_MOTOR];
    byte _motorPole[NUM_MOTOR_POLE]; //  byte MLPpin, byte MLNpin, byte MRPpin, byte MRNpin 
};
#endif /* L293DX_h */
