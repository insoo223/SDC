/*
 
 IRX.cpp
 SDC
 
 Created by In soo Kim on 10/24/15.
 Release to Public domain
 
 Updated on 10/29/15
 */

#include "Arduino.h"
#include "IRX.h"

//--------------------------------------------------
IRX :: IRX()
{
    _txFcmd = IR_FRONT;
    _txLcmd = IR_LEFT;
    _txRcmd = IR_RIGHT;
    
    _rxFpin = IR_Rcv_FRONTpin;
    _rxLpin = IR_Rcv_LEFTpin;
    _rxRpin = IR_Rcv_RIGHTpin;
    
    _doneInitEEPROM = false;
    _doneReadOnceIRstdEEPROM = false;
    
    _fwdDuration = 50;
    _bwdDuration = 300;
    _rectTLduration = 300;
    _rectTRduration = 300;
    
    
}//IRX


//--------------------------------------------
void IRX :: runSDC(bool debugSerial, bool debug4digit7seg)
{
    int sideDiff;
    
    //read reference value of each side IR to make turning decision
    readOnceIRstd_EEPROM(debugSerial, debug4digit7seg);
    //get the current IR value of each side
    getPure_AllIRs(debugSerial, debug4digit7seg); //true  false
    //get the difference between left and right side distance to any object
    sideDiff = getSideDiff(_pureL, _pureR);
    //drive the car based on the front side IR value
    IRbased_SelfDriving(_pureF, _pureL, _pureR, sideDiff);
}//runSDC

//--------------------------------------------
void IRX :: IRbased_SelfDriving(int FpureRcv, int LpureRcv, int RpureRcv, int diffLminusR)
{
    byte i;
    // *** Front IR based control
    // Being in far away from a front obstacle
    if (FpureRcv < _FIRStd)
    {
        _fwdDuration=50;
        _motor.fwd(_fwdDuration);
    }
    // Passing between turning point of "_FIRStd"
    // and "goBackward()" criterion of "FIRBackwardStd"
    else if (FpureRcv < FIRBackwardStd)
    {
        //scenario #1
        decideTurnDirection(diffLminusR);
        
        //scenario #2
        /*
        _fwdDuration=30;
        for (i=0; i<8; i++)
        {
            _motor.fwd(_fwdDuration);
            _motor.stop(10);
        }
        decideTurnDirection(diffLminusR);
         */
    }
    // Crossing over "Backward" criterion of "FIRBackwardStd"
    else
    {
        //scenario #1
        _bwdDuration=300;
        _motor.bwd(_bwdDuration);
        decideTurnDirection(diffLminusR);
        
        //scenario #2
        //_motor.stop(STOP_DURATION);
        /*
        _bwdDuration=300;
        _motor.bwd(_bwdDuration);
        _fwdDuration=50;
        decideTurnDirection(diffLminusR);
         */
    }
    
    // *** Left IR based control
    if (LpureRcv > LIR_ABS_TURN)
    {
        _motor.turnRight(SHORT_TURN_DURATION, STOP_AFT_TURN_DURATION);
        _prevTurnMode = TURN_RIGHT_MODE;
    }
    
    // *** Right IR based control
    if (RpureRcv > RIR_ABS_TURN)
    {
        _motor.turnLeft(SHORT_TURN_DURATION, STOP_AFT_TURN_DURATION);
        _prevTurnMode = TURN_LEFT_MODE;
    }
    
}//IRbased_SelfDriving

//--------------------------------------------
// Decide turning diection based on the weighted distance of left & right IR
void IRX :: decideTurnDirection(int diffLminusR)
{
    
    //Left side is closer to object than right side
    if (diffLminusR > 0)
    {
        smartTurnRight();
    }
    //when (Ldist =< Rdist), meaning the IR sensor is closer to Right side object than the left side one
    else
    {
        smartTurnLeft();
    }
    
}//decideTurnDirection

//-----------------------------
void IRX :: smartTurnLeft()
{
    if (_prevTurnMode == TURN_LEFT_MODE)
        _cntTURN_LEFT_MODE++;
    if (_cntTURN_LEFT_MODE == MAX_TURN_LEFT_IN_A_ROW)
    {
        _motor.stop(STOP_DURATION);
        _motor.bwd(_bwdDuration);
        _motor.turnLeft(_rectTLduration, STOP_AFT_TURN_DURATION);
        _cntTURN_LEFT_MODE=0;
        _prevTurnMode= -1;
    }
    else
    {
        _motor.turnLeft(_rectTLduration, STOP_AFT_TURN_DURATION);
        _prevTurnMode = TURN_LEFT_MODE;
    }
    
}//smartTurnLeft

//-----------------------------
void IRX :: smartTurnRight()
{
    if (_prevTurnMode == TURN_RIGHT_MODE)
        _cntTURN_RIGHT_MODE++;
    if (_cntTURN_RIGHT_MODE == MAX_TURN_RIGHT_IN_A_ROW)
    {
        _motor.stop(STOP_DURATION);
        _motor.bwd(_bwdDuration);
        _motor.turnRight(_rectTRduration, STOP_AFT_TURN_DURATION);
        _cntTURN_RIGHT_MODE=0;
        _prevTurnMode= -1;
    }
    else
    {
        _motor.turnRight(_rectTRduration, STOP_AFT_TURN_DURATION);
        _prevTurnMode = TURN_RIGHT_MODE;
    }
    
}//smartTurnRight

//--------------------------------------------
void IRX :: readOnceIRstd_EEPROM(bool debugSerial, bool debug4digit7seg)
{
    
    if (!_doneReadOnceIRstdEEPROM)
    {
        int startAdrs;
        
        //get the last saved address pointer
        startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR) - 3;
        
        //get the reference value of each side IR
        _FIRStd = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_FRONT_OFFSET);
        delay(EEPROM_READ_SAFEMARGIN);
        _LIRStd = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_LEFT_OFFSET);
        delay(EEPROM_READ_SAFEMARGIN);
        _RIRStd = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET);
        delay(EEPROM_READ_SAFEMARGIN);
        
        if (debugSerial)
        {
            Serial.print(STR_EEPROM_ADRS); Serial.println(startAdrs);
            Serial.print(STR_FRNT); Serial.println(_FIRStd);
            Serial.print(STR_LEFT); Serial.println(_LIRStd);
            Serial.print(STR_RITE); Serial.println(_RIRStd);
        }
        
        if (debug4digit7seg)
        {
            _7X.disp4chars(STR_FRNT, DISP_IR_SENSITIVITY);
            _7X.disp4digits(_FIRStd, 0, DISP_IR_SENSITIVITY);
            
            _7X.disp4chars(STR_LEFT, DISP_IR_SENSITIVITY);
            _7X.disp4digits(_LIRStd, 0, DISP_IR_SENSITIVITY);
            
            _7X.disp4chars(STR_RITE, DISP_IR_SENSITIVITY);
            _7X.disp4digits(_RIRStd, 0, DISP_IR_SENSITIVITY);
        }
        _doneReadOnceIRstdEEPROM = true;
    }
}//readOnceIRstd_EEPROM

//--------------------------------------------
int IRX :: getSideDiff(int LpureRcv, int RpureRcv)
{
    int Lunit, Runit;
    int Ldist, Rdist;
    
    //refer "globals.h" for Lunit, Runit, Ldist, Rdist, and ref_front
    // ADC for unit distance within the range of 0cm to 30cm away from an obstacle.
    // It assumes linearity of IR sensitivity in the corresponding range.
    // You'd got more precise assumption after checking the signal sensitivity curvarture by experiment
    Lunit = 3000/(MAX_ADC - _LIRStd); // [0.1mm/ADC]
    Runit = 3000/(MAX_ADC - _RIRStd); // [0.1mm/ADC]
    
    //Ldist & Rdist will be used to decide turning direction at the distance of 30cm between the car & an obstacle
    // Ldist & Rdist will be increased if the car is closer to an obstacle
    // Weighted distance measured by the Left IR
    Ldist = (LpureRcv - _LIRStd) * Lunit;
    // Weighted distance measured by the Right IR
    Rdist = (RpureRcv - _RIRStd) * Runit;
    
    return (Ldist - Rdist);
    
}//getSideDiff

//--------------------------------------------
//--- Save IR sensitivity to EEPROM
//--------------------------------------------

// Set IR sensitivity of front, left, and right IRs in a row.
void IRX :: saveRefPure_AllIR(bool debugSerial, bool debug4digit7seg)
{
    int startAdrs;
    
    startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);
    
    if (debug4digit7seg)
    {
        //finished tuning sensitivity
        _7X.disp4digits(startAdrs, 0, DISP_IR_SENSITIVITY);
    }
    if (debugSerial)
    {
        Serial.print(STR_EEPROM_ADRS);
        Serial.println(startAdrs);
    }
    
    // save IR sensitivity to EEPROM and to class private vars: _FIRStd, _LIRStd, _RIRStd
    _FIRStd = saveRefPure_SingleIR(IR_FRONT, debugSerial, debug4digit7seg);
    _LIRStd = saveRefPure_SingleIR(IR_LEFT, debugSerial, debug4digit7seg);
    _RIRStd = saveRefPure_SingleIR(IR_RIGHT, debugSerial, debug4digit7seg);
    
    //**********************
    //--- write to EEPROM, the updated startAdrs
    //**********************
    if ( (startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1) <= EEPROM_IR_SENSITIVITY_MAX_ADDR )
        EEPROM.write(EEPROM_IR_SENSITIVITY_ADDR_PTR, startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1);
    else
        EEPROM.write(EEPROM_IR_SENSITIVITY_ADDR_PTR, 1);
    
    if (debug4digit7seg)
    {
        //finished tuning sensitivity
        _7X.disp4chars(STR_ALL_DONE, DISP_IR_SENS_ALL_DONE);
    }
    delay(SET_IR_SENSITIVITY_NEXT_DURATION);
}//saveRefPure_AllIR

//--------------------------------------------
// char *str4charLocName //string to display IR sensor location
byte IRX :: prepareSaveRefPure(byte loc, byte EEPROMoffset, bool debugSerial, bool debug4digit7seg, char *str4charLocName, byte dispPrefix)
{
    int pureRcv; // the value of pureRcv should be same or less than 255 for EEPROM is byte-wide.
    int startAdrs; //start address of EEPROM for IR sensitivity data set
    
    //notice which IR sensor is under saving
    if (debug4digit7seg)
    {
        //disp IR location
        _7X.disp4chars(str4charLocName, DISP_IR_SENSITIVITY);
        delay(SET_IR_SENSITIVITY_READY_ON_DURATION);
        //disp blank
        _7X.disp4chars(BLANK4CHAR, DISP_IR_SENSITIVITY);
        delay(SET_IR_SENSITIVITY_READY_OFF_DURATION);
    }
    
    //get IR pure signal to save
    pureRcv = getPure_SingleIR(loc, debugSerial, debug4digit7seg);
    
    //If the signal is overflowed to be held in byte size storage, then trim it
    if (pureRcv > 255)
        pureRcv = 255;
    //**********************
    //--- write to EEPROM, the calibrated sensitivity of the Front IR
    //**********************
    startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);
    
    EEPROM.write(startAdrs + EEPROMoffset, _BYTE(pureRcv));
    
    delay(EEPROM_WRITE_SAFEMARGIN);
    
    //notice saving is done
    if (debug4digit7seg)
    {
        //disp DONE
        _7X.disp4chars(STR_DONE, DISP_IR_SENSITIVITY);
        //disp IR location
        _7X.disp4chars(str4charLocName, DISP_IR_SENSITIVITY);
        delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
        //disp blank
        _7X.disp4chars(BLANK4CHAR, DISP_IR_SENSITIVITY);
        delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
        
        //disp EEPROM address
        _7X.disp4digits(startAdrs+EEPROMoffset, 0, DISP_IR_SENSITIVITY);
        delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
        
        //disp EEPROM value; dispPrefix notifies sensor location by number, i.e. 1000
        _7X.disp4digits(dispPrefix*1000 + EEPROM.read(startAdrs + EEPROMoffset), 0, DISP_4digit7seg_ITERATION_NORMAL);
    }
    
    
    if (debugSerial)
    {
        //disp IR location
        Serial.println(str4charLocName);
        //disp EEPROM address
        Serial.print(STR_EEPROM_ADRS);
        Serial.println(startAdrs + EEPROMoffset);
        //disp EEPROM value
        Serial.print(STR_EEPROM_IR_SENS_READ);
        Serial.println(EEPROM.read(startAdrs + EEPROMoffset));
        //disp mesurement value
        Serial.print(STR_IR_SENS_MEASURED);
        Serial.println(pureRcv);
    }
    
    return (_BYTE(pureRcv));
    
}//prepareSaveRefPure

//--------------------------------------------
byte IRX :: saveRefPure_SingleIR(byte loc, bool debugSerial, bool debug4digit7seg)
{
    byte val;
    
    switch (loc)
    {
        case IR_FRONT:
            val = prepareSaveRefPure(IR_FRONT, EEPROM_IR_SENSITIVITY_FRONT_OFFSET, true, true, STR_FRNT, PREFIX_DISP_IR_FRONT);
            break;
            
        case IR_LEFT:
            val = prepareSaveRefPure(IR_LEFT, EEPROM_IR_SENSITIVITY_LEFT_OFFSET, true, true, STR_LEFT, PREFIX_DISP_IR_LEFT);
            break;
            
        case IR_RIGHT:
            val = prepareSaveRefPure(IR_RIGHT, EEPROM_IR_SENSITIVITY_RIGHT_OFFSET, true, true, STR_RITE, PREFIX_DISP_IR_RIGHT);
            break;
    }//switch (loc)
    
    return (val);
}//saveRefPure_SingleIR


//--------------------------------------------
//--- EEPROM initialization
//--------------------------------------------
void IRX :: initEEPROM()
{
    if (_doneInitEEPROM)
    {
        _7X.disp4chars("ARDY", DISP_INIT_EEPROM_STATUS);
        _7X.disp4chars(STR_DONE, DISP_INIT_EEPROM_STATUS);
    }
    while (!_doneInitEEPROM)
    {
        _7X.disp4chars(STR_INIT, DISP_INIT_EEPROM_STATUS);
        _7X.disp4chars("EEPR", DISP_INIT_EEPROM_STATUS);
        
        for (int i=0; i<1023; i++)
        {
            _7X.disp4digits(i,0,1);
            if (i==0)
                EEPROM.write(i,1);
            else
                EEPROM.write(i,0);
            delay(1);
            
        }
        
        _doneInitEEPROM = true;
        
        _7X.disp4chars(STR_DONE, DISP_INIT_EEPROM_STATUS);
        _7X.disp4chars(STR_INIT, DISP_INIT_EEPROM_STATUS);
    }// while (!doneInitEEPROM)
    
}//initEEPROM

//--------------------------------------------
//--- Pure IR signal
//--------------------------------------------
void IRX :: getPure_AllIRs(bool debugSerial, bool debug4digit7seg)
{
    int FcombinedRcv, FambientRcv, FpureRcv;
    int LcombinedRcv, LambientRcv, LpureRcv;
    int RcombinedRcv, RambientRcv, RpureRcv;
    
    //--- Tunr ON ir sensor(s)
    _X.ctrlSingle(_txFcmd, HIGH);
    _X.ctrlSingle(_txLcmd, HIGH);
    _X.ctrlSingle(_txRcmd, HIGH);
    
    // Give some break to raise its full power of IR beam, 1 ms is enough.
    delay(IR_ONOFF_SAFEMARGIN);
    
    // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
    // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
    FcombinedRcv=analogRead(_rxFpin);
    LcombinedRcv=analogRead(_rxLpin);
    RcombinedRcv=analogRead(_rxRpin);
    
    //--- Turn OFF IR sensors
    _X.ctrlSingle(_txFcmd, LOW);
    _X.ctrlSingle(_txLcmd, LOW);
    _X.ctrlSingle(_txRcmd, LOW);
    
    // Give some break to cool down their IR beam emission, 1 ms is enough.
    delay(IR_ONOFF_SAFEMARGIN);
    
    // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
    // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
    FambientRcv=analogRead(_rxFpin);
    LambientRcv=analogRead(_rxLpin);
    RambientRcv=analogRead(_rxRpin);
    
    // Now, we can get the pure signal strengh of our IR LEDs!
    FpureRcv=FcombinedRcv-FambientRcv;
    LpureRcv=LcombinedRcv-LambientRcv;
    RpureRcv=RcombinedRcv-RambientRcv;
    
    _pureF = FpureRcv;
    _pureL = LpureRcv;
    _pureR = RpureRcv;
    
    if (debugSerial)
    {
        Serial.print(STR_FRNT);
        Serial.print(STR_IR_SENSITIVITY_DEBUG);
        Serial.print(FcombinedRcv); Serial.print(STR_COMMA_SPC); Serial.print(FambientRcv); Serial.print(STR_COMMA_SPC); Serial.println(FpureRcv);
        Serial.print(STR_LEFT);
        Serial.print(STR_IR_SENSITIVITY_DEBUG);
        Serial.print(LcombinedRcv); Serial.print(STR_COMMA_SPC); Serial.print(LambientRcv); Serial.print(STR_COMMA_SPC); Serial.println(LpureRcv);
        Serial.print(STR_RITE);
        Serial.print(STR_IR_SENSITIVITY_DEBUG);
        Serial.print(RcombinedRcv); Serial.print(STR_COMMA_SPC); Serial.print(RambientRcv); Serial.print(STR_COMMA_SPC); Serial.println(RpureRcv);
        Serial.println();
    }
    
    if (debug4digit7seg)
    {
        _7X.disp4digits(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL);
        _7X.disp4digits(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL);
        _7X.disp4digits(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL);
    }
}//getPureIRs

//--------------------------------------------------
int IRX :: fireSingleIR(byte txCmd, byte rxPin, bool debugSerial, bool debug4digit7seg)
{
    int combinedRcv, ambientRcv, pureRcv;
    
    //--- Turn On ir sensor
    _X.ctrlSingle(txCmd, HIGH);
    
    // Give some break to raise its full power of IR beam, 1 ms is enough.
    delay(IR_ONOFF_SAFEMARGIN);
    
    // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
    // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
    combinedRcv = analogRead(rxPin);
    
    //--- Turn OFF IR sensors
    // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
    //Off74HC595_Middle(SOLE_MODE, txCmd);
    _X.ctrlSingle(txCmd, LOW);
    
    // Give some break to cool down their IR beam emission, 1 ms is enough.
    delay(IR_ONOFF_SAFEMARGIN);
    
    // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
    // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
    ambientRcv=analogRead(rxPin);
    
    // Now, we can get the pure signal strengh of our IR LEDs!
    pureRcv=combinedRcv-ambientRcv;
    
    if (debugSerial)
    {
        Serial.print(STR_IR_SENSITIVITY_DEBUG);
        Serial.print(combinedRcv); Serial.print(STR_COMMA_SPC);
        Serial.print(ambientRcv); Serial.print(STR_COMMA_SPC);
        Serial.println(pureRcv);
    }
    
    if (debug4digit7seg)
        _7X.disp4digits(pureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL);
    
    return (pureRcv);
    
}//fireSingleIR

//--------------------------------------------------
int IRX :: getPure_SingleIR(byte loc, bool debugSerial, bool debug4digit7seg)
{
    int val;
    
    switch (loc)
    {
        case IR_FRONT:
            val = fireSingleIR(_txFcmd, _rxFpin, debugSerial, debug4digit7seg);
            break;
            
        case IR_LEFT:
            val = fireSingleIR(_txLcmd, _rxLpin, debugSerial, debug4digit7seg);
            break;
            
        case IR_RIGHT:
            val = fireSingleIR(_txRcmd, _rxRpin, debugSerial, debug4digit7seg);
            break;
    }//switch (loc)
    
    return (val);
    
}//getPure_SingleIR
