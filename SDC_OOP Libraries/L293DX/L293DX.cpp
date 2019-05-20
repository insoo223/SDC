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
L293DX :: L293DX(byte* enable, byte* motorPole)
{
	for (byte i=0; i<4; i++)
	{
		_motorPole[i] = motorPole[i];
		if (i<2)
		{
			_enable[i] = enable[i];
			pinMode(_enable[i],OUTPUT);
		}
	}

	/*
    _enable[0] = enable[0];
    _enable[1] = enable[1];
	_motorPole[0] = motorNode[0];
	_motorPole[1] = motorNode[1];
	_motorPole[2] = motorNode[2];
	_motorPole[3] = motorNode[3];
    _MLPpin = MLPpin;
    _MLNpin = MLNpin;
    _MRNpin = MRNpin;
    _MRPpin = MRPpin;
    
    pinMode(_enable[0],OUTPUT);
    pinMode(_enable[1],OUTPUT);
	*/
}//L293DX

//--------------------------------------------
//--- Go Forward & Backward, All stop
//--------------------------------------------

void L293DX :: fwd(trippleX* X, int duration)
{
    //uint8_t topX, midX, botX;
    //Byte pattern of parallel pins of each 74HC595 chip
	byte num74HC595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 

    
    //trippleX X;
    
    //X->getCurrentX(&topX, &midX, &botX);
    X->getCurrentX(num74HC595);

    //LEFT motor is slightly weaker torque than right one,
    //  so that complement the power balance use analogWrite than digitalWrite
    //  by experiment as of Oct 30, 2015.
    //digitalWrite(_enable[0], HIGH);
    //digitalWrite(_enable[1], HIGH);
    analogWrite(_enable[1], 255);
    analogWrite(_enable[0], 230);
    //botX |= _BV(_MLPpin) | _BV(_MRPpin);
	num74HC595[MAXCHIP74HC595-3] |= _BV(_motorPole[0]) | _BV(_motorPole[2]);
    //botX &= ~_BV(_MLNpin) & ~_BV(_MRNpin);
	num74HC595[MAXCHIP74HC595-3] &= ~_BV(_motorPole[1]) & ~_BV(_motorPole[3]);
    X->ctrlAll_legacy(num74HC595[MAXCHIP74HC595-1], num74HC595[MAXCHIP74HC595-2], num74HC595[MAXCHIP74HC595-3]);
    delay(duration);

}//fwd

/***************************
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

***************************/