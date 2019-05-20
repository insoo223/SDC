/*
 
 UXX.cpp
 SDC
 
 Created by In soo Kim on 10/30/15.
 Release to Public domain
 
 Updated on 10/30/15
 */

#include "Arduino.h"
#include "UXX.h"

//--------------------------------------------------
UXX :: UXX()
{
    _UXArray[CMD_BEGIN]=5;
    _UXArray[CMD_FWD]=10;
    _UXArray[CMD_LEFT]=15;
    _UXArray[CMD_RIGHT]=20;
    _UXArray[CMD_TNCLK]=25;
    _UXArray[CMD_TNANT]=30;
    _UXArray[CMD_STOP]=35;
    _UXArray[CMD_REV]=40;
    _UXArray[CMD_LOOPBEGIN]=45;
    _UXArray[CMD_LOOPEND]=50;
    _UXArray[CMD_END]=55;
    
    _ref_frontUx = 15;
    
    _fwdDuration = 50;
    _bwdDuration = 300;
    _rectTLduration = 300;
    _rectTRduration = 300;
}//UXX

//---------------------------------------------
//check BEGIN command
//---------------------------------------------
bool UXX :: chkCMD_BEGIN()
{
    byte cmdUX;
    Four7segX _7X;

    //get distance as command
    cmdUX = measureUx(false, false, false);
    
    if ((cmdUX > (_UXArray[CMD_BEGIN] - MARGIN_UX)) && (cmdUX < (_UXArray[CMD_BEGIN] + MARGIN_UX)) )
    {
        Serial.println("Found CMD_BEGIN"); //RAM shortage occured!
        Serial.println(cmdUX);
        _7X.disp4digits(cmdUX,0,50);
        //disp4char_trippleX(strCMD[CMD_BEGIN], 50);
        return true;
    }
    else
    {
        //Serial.println("NOT yet found CMD_BEGIN");
        //Serial.println(cmdUX);
        //disp4char_trippleX("READ", 50);
        return false;
    }
}//chkCMD_BEGIN()



//---------------------------------------------
void UXX :: ProgByUX()
{
    Four7segX _7X;
    
    if (!_foundCMD_BEGIN)
    {
        _foundCMD_BEGIN = chkCMD_BEGIN();
        if (_foundCMD_BEGIN)
            _7X.disp4digits(100, 0, 200);
        delay(200);
    }//if (!_foundCMD_BEGIN)
    /*************************
    else
    {
        if (!_foundCMD_END)
            _foundCMD_END = chkCMD_END();
        
        //if ((!_foundCMD_END) && (digitalRead(modeOPswitch) == LOW))
        if (!_foundCMD_END)
        {
            chkCMD();
        }//if (!_foundCMD_END)
        
        if (_foundCMD_END)
        {
            _7X.disp4digits(300, 0, 200);
            //doUsrLoop();
        }//if (_foundCMD_END == true)
    }//else (_foundCMD_BEGIN == true)
     *************************/
}//ProgByUX

//----------------------------------------------------
//--- Front Ultrasound sensor based control
void UXX :: UXbased_SelfDriving(int diffLRfromIRsensors)
{
    byte uxDist;
    L293DX motor;
    IRX ir;
    
    uxDist=measureUx(false, false,false);
    
    // Being in far away from a front obstacle
    if (uxDist > _ref_frontUx)
    {
        motor.fwd(_fwdDuration);
    }
    // Passing between turning point of "_ref_frontUx"
    // and "Backward" criterion of "_uxBW"
    else if (uxDist < _uxBW)
    {
        ir.decideTurnDirection(diffLRfromIRsensors);
    }
    // Crossing over "Backward" criterion of "_uxBW"
    else
    {
        motor.stop(STOP_DURATION);
        motor.bwd(_bwdDuration);
        ir.decideTurnDirection(diffLRfromIRsensors);
    }
}//UXbased_SelfDriving

//----------------------------------------------------
void UXX :: measureHeight (bool debugSerial, bool debug7seg)
{
    CD4021X cd4021;
    
    if (cd4021.readPGM_MODEslideSW() == HIGH)
        measureUx(true, true, true);
    else
        measureUx(true, true, false);
    
    
}//measureHeight

//----------------------------------------------------
byte UXX :: measureUx(bool debugSerial, bool debug7seg, bool dispUpsideDown)
{
    int duration, distance; // Duration used to calculate distance
    trippleX X;
    Four7segX _7X;
    
    if (debugSerial)
        Serial.println("measureUx: ");
    
    // The following trigPin/echoPin cycle is used to determine the
    // distance of the nearest object by bouncing soundwaves off of it.
    X.ctrlSingle(UxTRIGcmd, LOW);
    delayMicroseconds(2);
    
    X.ctrlSingle(UxTRIGcmd, HIGH);
    delayMicroseconds(10);
    
    X.ctrlSingle(UxTRIGcmd, LOW);
    
    duration = pulseIn(UxECHOpin, HIGH);
    
    //Calculate the distance (in cm) based on the speed of sound.
    distance = duration/58.2;
    if (debug7seg)
    {
        if (dispUpsideDown)
            _7X.disp4digits_UpsideDown(distance, 0, 50);
        else
            _7X.disp4digits(distance, 0, 50);
    }
    if (debugSerial)
    {
        Serial.print("distance: ");
        Serial.println(distance);
    }
    
    if (distance >= UX_MAX || distance <= UX_MIN)
    {
        distance = 40;
        // Send a negative number to computer and Turn LED ON
        //to indicate "out of range"
        if (debugSerial)
            Serial.println("-1");
    }
    
    return (distance);
}//measureUx


//----------------------------------------------------


