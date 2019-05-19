#include "defs.h"
#include "externs.h"
#include <EEPROM.h>

//-----------------------------
// check voltage with oscilloscope
void sensitivityCheckIR_ForOSC()
{
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
  trippleXwrite(IR_FRONT, HIGH);
  trippleXwrite(IR_LEFT, HIGH);
  trippleXwrite(IR_RIGHT, HIGH);
  delay(500);

  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  delay(500);
}//sensitivityCheckIR_ForOSC

//-----------------------------
bool done_readSensitivityFromEEPROM = false;
bool SerialDebug_readSensitivityFromEEPROM = false; //false true
void readSensitivityFromEEPROM()
{
  if (!done_readSensitivityFromEEPROM)
  {
    int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR) - 3;
    
    FIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_FRONT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    LIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_LEFT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    RIR30 = EEPROM.read(startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET);
    delay(EEPROM_READ_SAFEMARGIN);
    
    if (SerialDebug_readSensitivityFromEEPROM)
    {
       Serial.print("startAdrs = EEPROM: "); Serial.println(startAdrs);
       Serial.print("FIR30 = EEPROM: "); Serial.println(FIR30);
       Serial.print("LIR30 = EEPROM: "); Serial.println(LIR30);
       Serial.print("RIR30 = EEPROM: "); Serial.println(RIR30);
    }
    
    done_readSensitivityFromEEPROM = true;
  }
}//readSensitivityFromEEPROM

//-----------------------------
void testIRs_Motor()
{
  int uxDist;

  readSensitivityFromEEPROM();
  getPureIRsignal(false, false);
  setUnitAndRefFront();
  frontIR_based_Control();

  
  // *** Left IR based control
  if (LpureRcv > LIR_ABS_TURN)
  {
    turnRight(SHORT_TURN_ITERATION);
    prevMode = TURN_RIGHT_MODE;
  }

  // *** Right IR based control
  if (RpureRcv > RIR_ABS_TURN)
  {
    turnLeft(SHORT_TURN_ITERATION);
    prevMode = TURN_LEFT_MODE;
  }
  
}//testIRs_Motor

//-----------------------------------
bool debug2Serial = false; // false true
bool debugUnitDist = false; // false true

void testIRs_ADC()
{
  getPureIRsignal(false, false);
  //setIR_RefVal_Turn();  
  setIR_RefVal_Turn_Auto();
  setUnitAndRefFront();
  
  frontIR_based_Control();
}//testIRs_ADC

//--------------------- Front IR based control  
void frontIR_based_Control()
{
  // Being in far away from a front obstacle
  if (FpureRcv < ref_front)
  {
    if (IRmode == IR_SETmode)
      //Show FWD_MODE "0000" without DP, meaning "goForward()"
      //disp4digit7seg(FWD_MODE, 0);
      ;
    else
    {    
      goForward();
      /*
      checkSpeed();
      if ( abs(prevSpeed - travelSpeed) > 20 )
        disp4digit7seg_trippleX(travelSpeed, 0, 1);  
      */
    }
      
  }
  // Passing between turning point of "ref_front" 
  // and "goBackward()" criterion of "FIR_BW"
  else if (FpureRcv < FIR_BW)
  {
    decideTurnDirection();
  }
  // Crossing over "goBackward()" criterion of "FIR_BW"
  else
  {
    if (IRmode == IR_SETmode)
      // Show BWD_MODE "3000" without DP, meaning "goBackward()"
      //disp4digit7seg(BWD_MODE, 0);
      ;
    else
      goBackward();
    
    decideTurnDirection();
  }
}//frontIR_based_Control

//-----------------------------
void testUx_Motor()
{
  int uxDist;

  //IR info
  /*------
  readSensitivityFromEEPROM();
  getPureIRsignal(false, false);
  setUnitAndRefFront();
  --------*/
  
  //Ux based control
  frontUx_based_Control();

  //disp4digit7seg_trippleX(uxDist, 0, DISP_4digit7seg_ITERATION_NORMAL);
  /*
  if (uxDist < 15)
  {
    // *** Left IR based control
    if (LpureRcv > LIR_ABS_TURN)
    {
      turnRight(RECT_ITERATION);
    }
  
    // *** Right IR based control
    if (RpureRcv > RIR_ABS_TURN)
    {
      turnLeft(RECT_TL_ITERATION);
    }
  }    
  */
  // *** Left IR based control
  /*----------
  if (LpureRcv > LIR_ABS_TURN)
  {
    turnRight(SHORT_TURN_ITERATION);
  }

  // *** Right IR based control
  if (RpureRcv > RIR_ABS_TURN)
  {
    turnLeft(SHORT_TURN_ITERATION);
  }
  -------------*/
}//testUx_Motor


//--- Front Ultrasound sensor based control  
void frontUx_based_Control()
{
  int uxDist;
  
  uxDist=measureUx(false, false);
  
  // Being in far away from a front obstacle
  if (uxDist > ref_frontUx)
  {
    if (IRmode == IR_SETmode)
      //Show FWD_MODE "0000" without DP, meaning "goForward()"
      //disp4digit7seg(FWD_MODE, 0);
      ;
    else
      goForward();
  }
  // Passing between turning point of "ref_front" 
  // and "goBackward()" criterion of "FIR_BW"
  else if (uxDist < Ux_BW)
  {
    decideTurnDirection();
  }
  // Crossing over "goBackward()" criterion of "FIR_BW"
  else
  {
    if (IRmode == IR_SETmode)
      // Show BWD_MODE "3000" without DP, meaning "goBackward()"
      //disp4digit7seg(BWD_MODE, 0);
      ;
    else
    {
      OffBothMotors();
      delay(500);
      goBackward();
    }
    
    //decideTurnDirection();
    smartTurnLeft();
  }
}//frontUx_based_Control

//-----------------------------
void smartTurnLeft()
{
  if (prevMode == TURN_LEFT_MODE)
    cntTURN_LEFT_MODE++;
  if (cntTURN_LEFT_MODE == MAX_TURN_LEFT_IN_A_ROW)
  {
    OffBothMotors();
    delay(500);
    goBackward();
    turnLeft(RECT_TL_ITERATION);
    cntTURN_LEFT_MODE=0;
    prevMode= -1;
  }
  else
  {
    turnLeft(RECT_TL_ITERATION);
    prevMode = TURN_LEFT_MODE;
  }

}//smartTurnLeft

//-----------------------------
void smartTurnRight()
{
  if (prevMode == TURN_RIGHT_MODE)
    cntTURN_RIGHT_MODE++;
  if (cntTURN_RIGHT_MODE == MAX_TURN_RIGHT_IN_A_ROW)
  {
    OffBothMotors();
    delay(STOP_DURATION);
    goBackward();
    turnRight(RECT_ITERATION);
    cntTURN_RIGHT_MODE=0;
    prevMode= -1;
  }
  else
  {
    turnRight(RECT_ITERATION);
    prevMode = TURN_RIGHT_MODE;
  }

}//smartTurnRight

//-----------------------------
// Decide turning diection based on the weighted distance of left & right IR
void decideTurnDirection()
{
  // When the IR sensor is closer to Left side object than the right side one
  if (Ldist > Rdist) 
  {
    if (IRmode == IR_SETmode)
      // Show TURN_RIGHT_MODE "2000" without DP, meaning "turnRight(RECT_ITERATION)"
      //disp4digit7seg(TURN_RIGHT_MODE, 0);
      ;
    else
      smartTurnRight();
  }
  //when (Ldist =< Rdist), meaning the IR sensor is closer to Right side object than the left side one
  else 
  {
    if (IRmode == IR_SETmode)
      // Show TURN_LEFT_MODE "1000" without DP, meaning "turnRight(RECT_ITERATION)"
      //disp4digit7seg(TURN_LEFT_MODE, 0);
      ;
    else
      smartTurnLeft();
  }
  
  if (IRmode == IR_SETmode)
  {
    if (debug2Serial)
    {
      if (Ldist > Rdist)   
        Serial.println("TURN_RIGHT_MODE: Ldist & Rdist = ");
      else
        Serial.println("TURN_LEFT_MODE: Ldist & Rdist = ");
      Serial.print(Ldist);
      Serial.print(", ");
      Serial.println(Rdist);
    }
    
    if (debugUnitDist)
    {
      // FYI, Show weighted distance of Left & Right side IRs
      //disp4digit7seg(Ldist, 1);
      //disp4digit7seg(Rdist, 2);
    }
  }
}//decideTurnDirection

//-----------------------------
void setUnitAndRefFront()
{
  //refer "globals.h" for Lunit, Runit, Ldist, Rdist, and ref_front
  if (REF_DIST == 30)
  {
    // ADC for unit distance within the range of 0cm to 30cm away from an obstacle.
    // It assumes linearity of IR sensitivity in the corresponding range. 
    // You'd got more precise assumption after checking the signal sensitivity curvarture by experiment
    Lunit = 3000/(MAX_ADC - LIR30); // [0.1mm/ADC]
    Runit = 3000/(MAX_ADC - RIR30); // [0.1mm/ADC]

    //Ldist & Rdist will be used to decide turning direction at the distance of 30cm between the car & an obstacle
    // Ldist & Rdist will be increased if the car is closer to an obstacle
    // Weighted distance measured by the Left IR
    Ldist = (LpureRcv - LIR30) * Lunit;
    // Weighted distance measured by the Right IR
    Rdist = (RpureRcv - RIR30) * Runit;
  }//if (REF_DIST == 30)
  

  if (REF_DIST == 30)
    ref_front = FIR30;
    
  //Serial.println(ref_front);  

}//setUnitAndRefFront

//-----------------------------
bool doneInitEEPROM = false;
void initEEPROM()
{
    while (!doneInitEEPROM)
    {
      On74HC595_Bottom(SOLE_MODE, PGM_START);
      delay(500);
      Off74HC595_Bottom(SOLE_MODE, PGM_START);
      delay(500);
      
      for (int i=0; i<1023; i++)
      {
        if (i==0)
          EEPROM.write(i,1);
        else
          EEPROM.write(i,0);
        delay(1);
      }

      On74HC595_Bottom(SOLE_MODE, PGM_END);
      delay(500);
      Off74HC595_Bottom(SOLE_MODE, PGM_END);
      delay(500);
    }// while (!doneInitEEPROM)
}//initEEPROM

//-----------------------------
bool setIR_RefVal_Turn_Auto_SerialDebug = false; //false true
bool setIR_RefVal_Turn_Auto_disp4digit7seg = true; //false true

//-----------------------------
// the value of ref30 should be same or within 255.
void setOneIR_Sensitivity(byte loc)
{
  int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);
  int led, *ref30, *ref15, expRcv, EEPROMoffset;
  int dispPrefix;
  char target[50];
  
  switch (loc)
  {
    case IR_FRONT:
      led = GREEN_LED;
      ref30 = &FIR30;
      ref15 = &FIR15;
      expRcv = FpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_FRONT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_FRONT;
      strcpy(target, "Front IR sensitivity");
      break;

    case IR_LEFT:
      led = YELLOW_LED;
      ref30 = &LIR30;
      ref15 = &LIR15;
      expRcv = LpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_LEFT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_LEFT;
      strcpy(target, "Left IR sensitivity");
      break;

    case IR_RIGHT:
      led = RED_LED;
      ref30 = &RIR30;
      ref15 = &RIR15;
      expRcv = RpureRcv;
      EEPROMoffset = EEPROM_IR_SENSITIVITY_RIGHT_OFFSET;
      dispPrefix = PREFIX_DISP_IR_RIGHT;
      strcpy(target, "Right IR sensitivity");
      break;
  }//switch (loc)
      
  // set the Front IR sensitivity
  ////disp4digit7seg(FpureRcv, 0);
  OnLED_SensorBoard(led);
  delay(SET_IR_SENSITIVITY_READY_ON_DURATION);
  OffLED_SensorBoard(led);
  delay(SET_IR_SENSITIVITY_READY_OFF_DURATION);
  
  //if reference distance is 30cm (default), 
  //  measurement distance should be at 30cm away from an object
  //Currently, no other REF_DIST except 30.
  if (REF_DIST == 30)
  {
    //FpureRcv is a global variable attained thru "getPureIRsignal()"
    // implying the function "getPureIRsignal()" should be called beforhand.
    *ref30 = expRcv;
    
    //ADCdiff_30_15cm is a rule of thumb compensation, 
    //  need to experiment & measure further
    *ref15 = *ref30 + ADCdiff_30_15cm;
  }
  OnLED_SensorBoard(led);
  delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
  OffLED_SensorBoard(led);    
  delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);

  //--- write to EEPROM, the calibrated sensitivity of the Front IR
  EEPROM.write(startAdrs + EEPROMoffset, *ref30); 
  if (setIR_RefVal_Turn_Auto_SerialDebug)
  {
    Serial.println("-----"); 
    Serial.println(target); 
    Serial.print("startAdrs + EEPROMoffset: "); Serial.println(startAdrs + EEPROMoffset);
    Serial.print("IR sensitivity in EEPROM: "); Serial.println(EEPROM.read(startAdrs + EEPROMoffset));
    Serial.print("Updated IR sensitivity: "); Serial.println(*ref30);
  }

  if (setIR_RefVal_Turn_Auto_disp4digit7seg)
    disp4digit7seg_trippleX(dispPrefix + EEPROM.read(startAdrs + EEPROMoffset), 0, DISP_4digit7seg_ITERATION_NORMAL);

}//setOneIR_Sensitivity

//-----------------------------
// Set IR sensitivity of front, left, and right IRs in a row.
void setIR_RefVal_Turn_Auto()
{
  int startAdrs = EEPROM.read(EEPROM_IR_SENSITIVITY_ADDR_PTR);

  if (setIR_RefVal_Turn_Auto_SerialDebug)
  {
    Serial.print("startAdrs: ");
    Serial.println(startAdrs);
  }

  if (setIR_RefVal_Turn_Auto_disp4digit7seg)
    disp4digit7seg_trippleX(startAdrs, 0, DISP_4digit7seg_ITERATION_NORMAL);
    
  // set IR sensitivity
  setOneIR_Sensitivity(IR_FRONT);
  setOneIR_Sensitivity(IR_LEFT);
  setOneIR_Sensitivity(IR_RIGHT);

  //--- write to EEPROM, the updated startAdrs
  if ( (startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1) <= EEPROM_IR_SENSITIVITY_MAX_ADDR )
    EEPROM.write(0, startAdrs+EEPROM_IR_SENSITIVITY_RIGHT_OFFSET+1);
  else
    EEPROM.write(0, 1);

  //flash all three LEDs meaning to set sensitivity is finished
  for (int i=0; i<2; i++)
  {
    trippleXwrite(PGM_START, HIGH);
    trippleXwrite(PGM_CMD, HIGH);
    trippleXwrite(PGM_END, HIGH);
    delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
    trippleXwrite(PGM_START, LOW);
    trippleXwrite(PGM_CMD, LOW);
    trippleXwrite(PGM_END, LOW);
    delay(SET_IR_SENSITIVITY_COMPLETE_DURATION);
  }
  
  delay(SET_IR_SENSITIVITY_NEXT_DURATION);
}//setIR_RefVal_Turn_Auto

//------------------------------
//bool getPureIRsignal_SerialDebug = false; // false true
//bool getPureIRsignal_4digit7seg = false; // false true
void getPureIRsignal(bool DebugSerial, bool Debug4digit7seg)
{
  //--- Tunr ON ir sensor(s)
  //ThreeOn74HC595_Middle(FRIEND_MODE, IR_FRONT, IR_LEFT, IR_RIGHT);
  trippleXwrite(IR_FRONT, HIGH);
  trippleXwrite(IR_LEFT, HIGH);
  trippleXwrite(IR_RIGHT, HIGH);

  // Give some break to raise its full power of IR beam, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);

  // After full powering up the IR LEDs,, read IR beam's intensity of IR LEDs of three sides.
  // Their signals are composite ones added IR LED and IR light from the ambient light source, such as the Sun, florecent bulb etc.
  FcombinedRcv=analogRead(IR_Rcv_FRONTpin);
  LcombinedRcv=analogRead(IR_Rcv_LEFTpin);
  RcombinedRcv=analogRead(IR_Rcv_RIGHTpin);

  //--- Turn OFF IR sensors
  // SOLE_MODE turns off itself and all the other bits, so that effectively off all IRs.
  //Off74HC595_Middle(SOLE_MODE, IR_FRONT);  
  trippleXwrite(IR_FRONT, LOW);
  trippleXwrite(IR_LEFT, LOW);
  trippleXwrite(IR_RIGHT, LOW);
  
  // Give some break to cool down their IR beam emission, 1 ms is enough.
  delay(IR_ONOFF_SAFEMARGIN);
  
  // After cooling down the IR LEDs, read IR beam's intensity of IR LEDs of three sides.
  // Their signals ONLY contain infra red light from the ambient light source, such as the Sun, florecent bulb etc.
  FambientRcv=analogRead(IR_Rcv_FRONTpin);  
  LambientRcv=analogRead(IR_Rcv_LEFTpin);
  RambientRcv=analogRead(IR_Rcv_RIGHTpin);

  // Now, we can get the pure signal strengh of our IR LEDs!
  FpureRcv=FcombinedRcv-FambientRcv;
  LpureRcv=LcombinedRcv-LambientRcv;
  RpureRcv=RcombinedRcv-RambientRcv;

  if (DebugSerial)
  {
    Serial.print("F Cmb Amb Pure: "); Serial.print(FcombinedRcv); Serial.print(", "); Serial.print(FambientRcv); Serial.print(", "); Serial.println(FpureRcv);
    Serial.print("L Cmb Amb Pure: "); Serial.print(LcombinedRcv); Serial.print(", "); Serial.print(LambientRcv); Serial.print(", "); Serial.println(LpureRcv);
    Serial.print("R Cmb Amb Pure: "); Serial.print(RcombinedRcv); Serial.print(", "); Serial.print(RambientRcv); Serial.print(", "); Serial.println(RpureRcv);
    Serial.println("----------------------------------");
  }
  
  if (Debug4digit7seg)
  {
    disp4digit7seg_trippleX(1000+FpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(2000+LpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
    disp4digit7seg_trippleX(3000+RpureRcv, 0, DISP_4digit7seg_ITERATION_NORMAL); 
  }    
}//getPureIRsignal


