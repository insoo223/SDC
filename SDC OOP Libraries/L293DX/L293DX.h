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

//----------------------------------------------
//--- Pin assignments of L293D motor driving chip
//----------------------------------------------
#define EN1pin 10   // Arduino D10 pin
#define EN2pin 9  // Arduino D9 pin

// bottom 74HC595 pin
#define MLPpin 0
#define MLNpin 1
#define MRNpin 2
#define MRPpin 3

class L293DX
{
public:
    L293DX();
    void fwd(int duration);
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
    
    
private:
    uint8_t _enable1, _enable2;
    uint8_t _MLPpin, _MLNpin, _MRNpin, _MRPpin; // _input1, _input2, _input3, _input4;
};
#endif /* L293DX_h */
