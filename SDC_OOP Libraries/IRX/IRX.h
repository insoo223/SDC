/*
 IRX.h
 SDC
 
 Created by In soo Kim on 10/24/15.
 Release to Public domain
 */

#ifndef IRX_h
#define IRX_h

#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"
#include "CD4021X.h"
#include "L293DX.h"
#include <EEPROM.h>

#define IR_ONOFF_SAFEMARGIN 1 //[ms]

//----------------------------------------------
//--- MACRO
//----------------------------------------------

// Get the first byte from any size of data type
// A global variable is always takes 4 bytes in its size.
#define _BYTE(X) (X & 0xFF)

//----------------------------------------------
//--- Display time
//----------------------------------------------
#define DISP_4digit7seg_ITERATION_NORMAL    50 // Comfortable viewing time for 4digit 7segment LED
#define DISP_IR_SENSITIVITY                 50
#define DISP_INIT_EEPROM_STATUS             100 // status of initializing EEPROM
#define DISP_IR_SENS_ALL_DONE               200

//----------------------------------------------
//--- Delay time to check IR sensitivity automatically
//----------------------------------------------
#define SET_IR_SENSITIVITY_READY_ON_DURATION    1000 // [ms]
#define SET_IR_SENSITIVITY_READY_OFF_DURATION   2000 // [ms]
#define SET_IR_SENSITIVITY_COMPLETE_DURATION    300 // [ms]
#define SET_IR_SENSITIVITY_NEXT_DURATION        3000 // [ms]

#define EEPROM_WRITE_SAFEMARGIN                 10 //[ms]
#define EEPROM_READ_SAFEMARGIN                  10 //[ms]

//----------------------------------------------
//--- Pin assignments of tx IR to tripple 74HC595 output expansion chips
//----------------------------------------------
//--- middle 74HC595 pin assignments
// The fourth bit 1 means "middle"
#define IR_FRONT   0x14 //power pin to the front IR
#define IR_LEFT    0x15 //power pin to the left IR
#define IR_RIGHT   0x16 //power pin to the right IR

//----------------------------------------------
//--- Pin assignments for rx IR to the analog pin of Arduino
//----------------------------------------------
#define IR_Rcv_FRONTpin A0
#define IR_Rcv_LEFTpin A1
#define IR_Rcv_RIGHTpin A2

//----------------------------------------------
//--- Prefix to identify IR location when displaying on 7 seg LED
//----------------------------------------------
#define PREFIX_DISP_IR_FRONT 1
#define PREFIX_DISP_IR_LEFT  2
#define PREFIX_DISP_IR_RIGHT 3

//----------------------------------------------
//--- 4 char-wide strings & ordinary strings to be shown in debug mode
//----------------------------------------------
#define STR_FRNT                        "FRNT"
#define STR_LEFT                        "LEFT"
#define STR_RITE                        "RITE"
#define BLANK4CHAR                      "    "
#define STR_INIT                        "INIT"
#define STR_DONE                        "DONE"
#define STR_ALL_DONE                    "ALDN"

#define STR_IR_SENSITIVITY_DEBUG        "Cmb Amb Pure: "
#define STR_COMMA_SPC                   ", "

#define STR_EEPROM_ADRS                 "EEPROM adrs: "
#define STR_EEPROM_IR_SENS_READ         "EEPROM read: "
#define STR_IR_SENS_MEASURED            "IR measure: "

//----------------------------------------------
//--- EEPROM address
//----------------------------------------------
// location of the starting address of the current IR sensitivity reference value
// 0 means the 1st address of EEPROM contains the starting address of IR sensitivity measured beforehand
#define EEPROM_IR_SENSITIVITY_ADDR_PTR 0

#define EEPROM_IR_SENSITIVITY_FRONT_OFFSET   0
#define EEPROM_IR_SENSITIVITY_LEFT_OFFSET    1
#define EEPROM_IR_SENSITIVITY_RIGHT_OFFSET   2
#define EEPROM_IR_SENSITIVITY_MAX_ADDR       511 //adrs 0 to 511 is used for IR sensitivity storage

//----------------------------------------------
//--- ADC
//----------------------------------------------
#define MAX_ADC 1023

//----------------------------------------------
//--- IR standard value to make turning decision
//----------------------------------------------
#define FIRBackwardStd 600

//----------------------------------------------
//--- Duration or iteration time of motor operation
//----------------------------------------------
//#define FWD_DURATION    50 //[ms] //role replaced by byte _fwdDuration;
//#define BWD_DURATION    300 //[ms]

//#define RECT_TL_DURATION   300 //[ms] 300
//#define RECT_TR_DURATION   300 //[ms] 300
#define SHORT_TURN_DURATION 100 //[ms] 80
#define STOP_DURATION 300 //[ms]
#define STOP_AFT_TURN_DURATION 300 //[ms]

#define LIR_ABS_TURN    FIRBackwardStd
#define RIR_ABS_TURN    FIRBackwardStd

//----------------------------------------------
//--- Turn mode of motor operation
//----------------------------------------------
#define FWD_MODE 0
#define TURN_LEFT_MODE 1000
#define TURN_RIGHT_MODE 2000
#define BWD_MODE 3000

#define MAX_TURN_RIGHT_IN_A_ROW 3
#define MAX_TURN_LEFT_IN_A_ROW 3

//----------------------------------------------
//--- Class definition
//----------------------------------------------
class IRX
{
    
public:
    IRX();
    void runSDC(bool debugSerial, bool debug4digit7seg);
    void IRbased_SelfDriving(int FpureRcv, int LpureRcv, int RpureRcv, int diffLminusR);
    void decideTurnDirection(int diffLminusR);
    void smartTurnLeft();
    void smartTurnRight();
    void readOnceIRstd_EEPROM(bool debugSerial, bool debug4digit7seg);
    int getSideDiff(int LpureRcv, int RpureRcv);

    void saveRefPure_AllIR(bool SerialDebug, bool disp4digit7seg);
    byte prepareSaveRefPure(byte loc, byte EEPROMoffset, bool debugSerial, bool debug4digit7seg, char *str4charLocName, byte dispPrefix);
    byte saveRefPure_SingleIR(byte loc, bool SerialDebug, bool disp4digit7seg);

    void initEEPROM();

    void getPure_AllIRs(bool DebugSerial, bool Debug4digit7seg);
    int fireSingleIR(byte txCmd, byte rxPin, bool DebugSerial, bool Debug4digit7seg);
    int getPure_SingleIR(byte loc, bool DebugSerial, bool Debug4digit7seg);
    
private:
    uint8_t _txFcmd, _txLcmd, _txRcmd;
    uint8_t _rxFpin, _rxLpin, _rxRpin;

    bool _doneInitEEPROM;
    bool _doneReadOnceIRstdEEPROM;
    int _pureF, _pureL, _pureR;
    //standard value of each IR. They are used as turning decision point
    byte _FIRStd, _LIRStd, _RIRStd;
    int _prevTurnMode;
    
    // values decided by experimentals
    byte _fwdDuration;   //less than 255, usually smaller than 50 [ms]
    int _bwdDuration;    //more than 200,usually bigger than 300 [ms]
    int _rectTLduration; //more than 200,usually bigger than 300 [ms]
    int _rectTRduration; //more than 200,usually bigger than 300 [ms]
    

    byte _cntTURN_LEFT_MODE;
    byte _cntTURN_RIGHT_MODE;
    
    trippleX _X;
    Four7segX _7X;
    L293DX _motor;

};

#endif /* IRX_h */
