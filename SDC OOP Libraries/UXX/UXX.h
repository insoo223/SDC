/*
 UXX.h
 SDC
 
 Created by In soo Kim on 10/30/15.
 Release to Public domain
 
 Updated on 10/30/15
 */

#ifndef UXX_H
#define UXX_H

#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"
#include "L293DX.h"
#include "IRX.h"
#include "CD4021X.h"


//----------------------------------------------
//--- Pin assignments for Ultrasound Echo pins
//--- Trigger pin is attached to Q7 pin of the middle 74HC595
//----------------------------------------------
#define UxECHOpin     12

//--- middle 74HC595 pin assignments
#define UxTRIGcmd    0x17 //trigger pin to the Ultrasound transducer

#define UX_MAX 250 // Maximum range needed (default 200)
#define UX_MIN 0   // Minimum range needed
//#define Ux_BW  3
//----------------------------------------------
//--- EEPROM address map
//----------------------------------------------
//EEPROM address 512 to 1023 will be used.
//  0 to 511 is used for IR sensitivity storage
//  so that, EP_Cmd_BeginBlock should not be zero, only from 1 to 3 are valid.

#define EP_Cmd_BaseAdrs    512
//EEPROM is a byte-wide storage, so that bigger than 256 places are accessible by block & offset
#define EP_Cmd_BeginBlock  0
#define EP_Cmd_BeginOffset 1
#define EP_Cmd_EndBlock    2
#define EP_Cmd_EndOffset   3
#define EP_Cmd_NumAdrs     4
#define EP_Cmd_1stAdrs     5
#define EEPROMmaxAdrs      1023

#define MARGIN_UX   2   // [cm]
#define maxCmdIdx  10  // # of elements in the command set
#define maxUsrCmd  30  // max # of user command entry

#define STOP_DURATION 300 //[ms]

class UXX
{
    
public:
    UXX();
    bool chkCMD_BEGIN();
    void ProgByUX();
    
    void UXbased_SelfDriving(int diffLRfromIRsensors);
    void measureHeight (bool debugSerial, bool debug7seg);
    byte measureUx(bool debugSerial, bool debug7seg, bool dispUpsideDown);
    
    
private:
    //distances in centimeter[cm]
    byte _UXArray[maxCmdIdx+1];
    enum CMD {
        CMD_BEGIN, CMD_FWD, CMD_LEFT, CMD_RIGHT,
        CMD_TNCLK, CMD_TNANT, CMD_STOP, CMD_REV,
        CMD_LOOPBEGIN, CMD_LOOPEND, CMD_END};
    char _strCMD[maxCmdIdx+1][5];
    byte usrCmdArray[maxUsrCmd], usrCmdIdx;
    
    byte _ref_frontUx; //reference distance of Ux sensor to make turning decision

    // values decided by experimentals
    byte _fwdDuration;   //less than 255, usually smaller than 50 [ms]
    int _bwdDuration;    //more than 200,usually bigger than 300 [ms]
    int _rectTLduration; //more than 200,usually bigger than 300 [ms]
    int _rectTRduration; //more than 200,usually bigger than 300 [ms]
    byte _uxBW;
    
    boolean _foundCMD_BEGIN, _foundCMD_END;
    boolean DONE_writeCmd2EEPROM;
    boolean DONE_readCMD;

};
#endif /* UXX_H */
