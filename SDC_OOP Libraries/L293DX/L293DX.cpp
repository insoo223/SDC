/*
 
 L293DX.cpp
 SDC
 
 Created by In soo Kim on 10/24/15.
 Release to Public domain
 
 Updated on 10/29/15
*/

#include "Arduino.h"
#include "L293DX.h"

//--------------------------------------------------
L293DX :: L293DX()
{
    
    _enable1 = EN1pin;
    _enable2 = EN2pin;
    
    _MLPpin = MLPpin;
    _MLNpin = MLNpin;
    _MRNpin = MRNpin;
    _MRPpin = MRPpin;
    
    pinMode(_enable1,OUTPUT);
    pinMode(_enable2,OUTPUT);
}//L293DX

//--------------------------------------------
//--- Go Forward & Backward, All stop
//--------------------------------------------

void L293DX :: fwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, HIGH);
    botX |= _BV(_MLPpin) | _BV(_MRPpin);
    botX &= ~_BV(_MLNpin) & ~_BV(_MRNpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}//fwd

//--------------------------------------------------
void L293DX :: bwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, HIGH);
    botX |= _BV(_MLNpin) | _BV(_MRNpin);
    botX &= ~_BV(_MLPpin) & ~_BV(_MRPpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}//bwd

//--------------------------------------------------
void L293DX :: stop(int duration)
{
    digitalWrite(_enable1, LOW);
    digitalWrite(_enable2, LOW);
    delay(duration);
}//stop

//--------------------------------------------
//--- Turn Left & Right
//--------------------------------------------

void L293DX :: turnLeft(int turnDuration, int stopAfterTurnDuration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, HIGH);
    //disp4char_trippleX("LEFT", 10);
    botX |= _BV(_MLNpin) | _BV(_MRPpin);
    botX &= ~_BV(_MLPpin) & ~_BV(_MRNpin);
    X.ctrlAll(topX, midX, botX);
    delay(turnDuration);
    stop(stopAfterTurnDuration);
}//turnLeft

//--------------------------------------------------
void L293DX :: turnRight(int turnDuration, int stopAfterTurnDuration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, HIGH);
    //disp4char_trippleX("RITE", 10);
    botX |= _BV(_MLPpin) | _BV(_MRNpin);
    botX &= ~_BV(_MLNpin) & ~_BV(_MRPpin);
    X.ctrlAll(topX, midX, botX);
    delay(turnDuration);
    stop(stopAfterTurnDuration);
}//turnRight


//--------------------------------------------
//--- Turn Clock-wise & Anti Clock-wise
//--------------------------------------------

void L293DX :: turnClk(int turnDuration, int stopAfterTurnDuration)
{
    turnRight(turnDuration, stopAfterTurnDuration);
}//turnClk

//--------------------------------------------------
void L293DX :: turnAntClk(int turnDuration, int stopAfterTurnDuration)
{
    turnLeft(turnDuration, stopAfterTurnDuration);
}//turnAntClk


//--------------------------------------------
//--- LEFT MOTOR
//--------------------------------------------

void L293DX :: LFwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, LOW);
    botX |= _BV(_MLPpin);
    botX &= ~_BV(_MLNpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}

//--------------------------------------------------
void L293DX :: LBwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, LOW);
    botX |= _BV(_MLNpin);
    botX &= ~_BV(_MLPpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}

//--------------------------------------------------
void L293DX :: LStop(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable1, LOW);
    //digitalWrite(_enable2, HIGH);
    delay(duration);
}

//--------------------------------------------
//--- RIGHT MOTOR
//--------------------------------------------

void L293DX :: RFwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable2, HIGH);
    digitalWrite(_enable1, LOW);
    botX |= _BV(_MRPpin);
    botX &= ~_BV(_MRNpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}

//--------------------------------------------------
void L293DX :: RBwd(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    digitalWrite(_enable2, HIGH);
    digitalWrite(_enable1, LOW);
    botX |= _BV(_MRNpin);
    botX &= ~_BV(_MRPpin);
    X.ctrlAll(topX, midX, botX);
    delay(duration);
}

//--------------------------------------------------
void L293DX :: RStop(int duration)
{
    uint8_t topX, midX, botX;
    
    trippleX X;
    
    X.getCurrentX(&topX, &midX, &botX);
    
    //digitalWrite(_enable1, HIGH);
    digitalWrite(_enable2, LOW);
    delay(duration);
}

