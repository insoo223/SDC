/*
 
 L293DX.cpp
 SDC
 
 Created by In soo Kim on 10/24/15.
 Release to Public domain
 
 Updated on 10/29/15
*/

#include "Arduino.h"
#include "L293DX.h"

/*----------------------------------------------------------
Function Name: 
	L293DX
Purpose: 
	The constructor of the class L293DX.
	Assign pins for Enable & Positive/Negative poles of motors
How to:
	1. Get "enable" & "motorPole" pins array as input arg. 
	2. Assign them to the corresponding privat vars.
Input Arguments: 
	byte* enable - Enable pins of L293D chip
	byte* motorPole - Positive/Negative or "A" pins of L293D chip

Output Arguments: 
	NA
Updated: 
	May 16, 2019 (Thu)
Created: 
	May 6, 2019 (Mon)
Advantage:
Limitation:
Ref:
	L293D datasheet https://app.box.com/s/50le9cx36soiljm42cxe2ibp9elxzv23
----------------------------------------------------------*/
L293DX :: L293DX(byte* enable, byte* motorPole)
{
	for (byte i=0; i<NUM_MOTOR_POLE; i++)
	{
		_motorPole[i] = motorPole[i];
		if (i < NUM_MOTOR)
		{
			_enable[i] = enable[i];
			pinMode(_enable[i],OUTPUT);
		}
	}

}//L293DX

//--------------------------------------------
//--- Go Forward & Backward, All stop
//--------------------------------------------

void L293DX :: fwd(trippleX* X, int duration)
{
    //uint8_t topX, midX, botX;
    //Byte pattern of parallel pins of each 74HC595 chip
	byte num74HC595[MAXCHIP74HC595]; //array element 0 for botX, 1 for midX, 2 for topX 
	//enum _me me;
	//enum _mp mp;
	byte chipID; 
	byte pinID;

    
    //trippleX X;
    
    //X->getCurrentX(&topX, &midX, &botX);
    X->getCurrentX(num74HC595);

    //LEFT motor is slightly weaker torque than right one,
    //  so that complement the power balance use analogWrite than digitalWrite
    //  by experiment as of Oct 30, 2015.
    //digitalWrite(_enable[0], HIGH);
    //digitalWrite(_enable[1], HIGH);
    //analogWrite(_enable[0], 230);
	pinID = _enable[0] & 0x0F;
    analogWrite(_enable[pinID], 255);
	pinID = _enable[1] & 0x0F;
	analogWrite(_enable[pinID], 0);
    //analogWrite(_enable[MLEpin], 230);
    //analogWrite(_enable[MREpin], 255);

	//botX |= _BV(_MLPpin) | _BV(_MRPpin);
	num74HC595[MAXCHIP74HC595-3] |= _BV(_motorPole[0] & 0x0F) | _BV(_motorPole[2] & 0x0F);
	/*
	chipID = 3;
	pinID = _mp.MLPpin
	X->setSingleChipSinglePin595(chipID, pinID, 1);
	*/

    //botX &= ~_BV(_MLNpin) & ~_BV(_MRNpin);
	num74HC595[MAXCHIP74HC595-3] &= ~_BV(_motorPole[1] & 0x0F) & ~_BV(_motorPole[3] & 0x0F);
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