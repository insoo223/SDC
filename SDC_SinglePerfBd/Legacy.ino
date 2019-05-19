//----------------------------------------------
//--- Legacy IR code snippets
//----------------------------------------------
void setIR_RefVal_Turn()
{
  //disp4digit7seg(FpureRcv, 0);
  if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
  {
    if (REF_DIST == 30)
    {
      FIR30 = FpureRcv;
      FIR15 = FIR30 + ADCdiff_30_15cm;
    }
    else if (REF_DIST == 15)
    {
      FIR15 = FpureRcv;
      FIR30 = FIR15 - ADCdiff_30_15cm;
    }
    OnLED_SensorBoard(GREEN_LED);
    delay(300);
    OffLED_SensorBoard(GREEN_LED);    
    delay(300);
    //Serial.print("Updated FIR30: "); Serial.println(FIR30);
  }//if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)

  //disp4digit7seg(LpureRcv, 1);
  if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
  {
    if (REF_DIST == 30)
    {
      LIR30 = LpureRcv;
      LIR15 = LIR30 + ADCdiff_30_15cm;
    }
    else if (REF_DIST == 15)
    {
      LIR15 = LpureRcv;
      LIR30 = LIR15 - ADCdiff_30_15cm;
    }
    OnLED_SensorBoard(GREEN_LED);
    delay(300);
    OffLED_SensorBoard(GREEN_LED);    
    delay(300);
    //Serial.print("Updated LIR30: "); Serial.println(LIR30);
  }//if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)

  //disp4digit7seg(RpureRcv, 2);
  if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
  {
    if (REF_DIST == 30)
    {
      RIR30 = FpureRcv;
      RIR15 = RIR30 + ADCdiff_30_15cm;
    }
    else if (REF_DIST == 15)
    {
      RIR15 = FpureRcv;
      RIR30 = RIR15 - ADCdiff_30_15cm;
    }
    OnLED_SensorBoard(GREEN_LED);
    delay(300);
    OffLED_SensorBoard(GREEN_LED);    
    delay(300);
    //Serial.print("Updated RIR30: "); Serial.println(RIR30);
  }//if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
}//setIR_RefVal_Turn


//----------------------------------------------
//--- Legacy three 74HC595 chips control code snippets
//----------------------------------------------
//turn ON a corresponding bit of the top 74HC595 in the output expansion module
void On74HC595_Top(byte SoleORfriend, byte pos)
{
  // if SoleORfriend is FRIEND_MODE, SoleORfriend is curState74HC595
  // if SoleORfriend is SOLE_MODE, SoleORfriend is 0x00
  curState74HC595 = SoleORfriend | (1<<pos);
  control74HC595(curState74HC595, 0, 0);
}//On74HC595_Top

void TwoOn74HC595_Top(byte SoleORfriend, byte pos, byte pos2)
{
  curState74HC595 = SoleORfriend | (1<<pos) | (1<<pos2);
  control74HC595(curState74HC595, 0, 0);
}//TwoOn74HC595_Top

//-----------------------------------------------
//turn OFF a corresponding bit of the top 74HC595 in the output expansion module
void Off74HC595_Top(byte SoleORfriend, byte pos)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(pos);
  else
    curState74HC595=0x00;
  control74HC595(curState74HC595, 0, 0);
}//Off74HC595_Top

//-----------------------------------------------
//turn ON a corresponding bit of the middle 74HC595 in the output expansion module
void On74HC595_Middle(byte SoleORfriend, byte pos)
{
  curState74HC595 = SoleORfriend | (1<<pos);
  control74HC595(0, curState74HC595, 0);
}//On74HC595_Middle

void ThreeOn74HC595_Middle(byte SoleORfriend, byte pos, byte pos2, byte pos3)
{
  curState74HC595 = SoleORfriend | (1<<pos) | (1<<pos2) | (1<<pos3);
  control74HC595(0, curState74HC595, 0);
}//ThreeOn74HC595_Middle

//-----------------------------------------------
//turn OFF a corresponding bit of the middle 74HC595 in the output expansion module
void Off74HC595_Middle(byte SoleORfriend, byte pos)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(pos);
  else
    curState74HC595=0x00;
  control74HC595(0, curState74HC595, 0);
}//Off74HC595_Middle

//-----------------------------------------------
//turn ON a corresponding bit of the bottom 74HC595 in the output expansion module
void On74HC595_Bottom(byte SoleORfriend, byte pos)
{
  curState74HC595 = SoleORfriend | (1<<pos);
  control74HC595(0, 0, curState74HC595);
}//On74HC595_Bottom

void TwoOn74HC595_Bottom(byte SoleORfriend, byte pos, byte pos2)
{
  curState74HC595 = SoleORfriend | (1<<pos) | (1<<pos2);
  control74HC595(0, 0, curState74HC595);
}//TwoOn74HC595_Bottom

void TwoOnOff74HC595_Bottom(byte SoleORfriend, byte pos, byte pos2)
{
  curState74HC595 = SoleORfriend | (1<<pos);
  curState74HC595 &= ~(1<<pos2);
  control74HC595(0, 0, curState74HC595);
}//TwoOn74HC595_Bottom

void ThreeOn74HC595_Bottom(byte SoleORfriend, byte pos, byte pos2, byte pos3)
{
  curState74HC595 = SoleORfriend | (1<<pos) | (1<<pos2) | (1<<pos3);
  control74HC595(0, 0, curState74HC595);
}//ThreeOn74HC595_Bottom
//-----------------------------------------------
//turn OFF a corresponding bit of the bottom 74HC595 in the output expansion module
void Off74HC595_Bottom(byte SoleORfriend, byte pos)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(pos);
  else
    curState74HC595=0x00;
  control74HC595(0, 0, curState74HC595);
}//Off74HC595_Bottom

//-----------------------------------------------
// turn on or off only for a target bit untouching other bits
void friendlyOFF(byte pos)
{
  byte bitizeOne, bitizeZero;
  bitizeOne = (1 << pos); //0010 0000 in case of pos is 5
  bitizeZero = ~bitizeOne; // 1101 1111
  curState74HC595 = ~(curState74HC595 ^ bitizeZero);
}//friendlyOFF


//----------------------------------------------
//--- Legacy Motor code snippets ---------------
//----------------------------------------------
void goForward_Legacy()
{
  TwoOn74HC595_Bottom(SOLE_MODE, MR_ONOFF, ML_ONOFF);
  
  //TwoOn74HC595_Bottom(FRIEND_MODE, MR_ONOFF, ML_ONOFF);
  //delay(FWD_DURATION);
  //intermittentGoForward();
  //statusBottom74HC595 = 
}//goForward_Legacy

//----------------------------------------------
void goBackward_Legacy()
{
  TwoOn74HC595_Bottom(FRIEND_MODE, MR_ONOFF, ML_ONOFF);
  TwoOn74HC595_Bottom(FRIEND_MODE, MR_FWDREV, ML_FWDREV);
  delay(BWD_DURATION);
}//goBackward_Legacy

//----------------------------------------------
void turnLeft_Legacy()
{
  //for (int i=0; i<RECT_ITERATION; i++) 
  for (int i=0; i<RECT_TL_ITERATION; i++) 
    //On74HC595_Bottom(SOLE_MODE, MR_ONOFF);
    TwoOnOff74HC595_Bottom(SOLE_MODE, MR_ONOFF, ML_ONOFF);

  OffBothMotors();
}//turnLeft_Legacy

//*********************************************
//--- RIGHT motor control
//----------------------------------------------
void testRM_Legacy()
{
  for (int i=0; i<OP_DURATION; i++)
    On74HC595_Bottom(SOLE_MODE, MR_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    //Off74HC595_Bottom(FRIEND_MODE, MR_ONOFF);
    //Off74HC595_Bottom(SOLE_MODE, MR_ONOFF);
    OffMotorRight_Legacy(SOLE_MODE);

  for (int i=0; i<OP_DURATION; i++)
    TwoOn74HC595_Bottom(SOLE_MODE, MR_FWDREV, MR_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    //Off74HC595_Bottom(FRIEND_MODE, MR_ONOFF);
    //Off74HC595_Bottom(SOLE_MODE, MR_ONOFF);
    OffMotorRight_Legacy(SOLE_MODE);
}//testRM

void OnMotorRight_REV_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (1<<MR_ONOFF) | (1<<MR_FWDREV));
  //Serial.print("OnMotorRight_REV: "); Serial.println(prevState | (1<<MR_ONOFF) | (1<<MR_FWDREV));
}//OnMotorRight_REV

//----------------------------------------------
void OnMotorRight_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (1<<MR_ONOFF));
}//OnMotorRight

//----------------------------------------------
void OffMotorRight_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (0<<MR_ONOFF));
}//OffMotorRight

//*********************************************
//--- LEFT motor control
//----------------------------------------------
void testLM_Legacy()
{
  for (int i=0; i<OP_DURATION; i++)
    On74HC595_Bottom(SOLE_MODE, ML_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    //Off74HC595_Bottom(FRIEND_MODE, MR_ONOFF);
    //Off74HC595_Bottom(SOLE_MODE, ML_ONOFF);
    //OffMotorRight(SOLE_MODE);
    OffMotorLeft_Legacy(SOLE_MODE);

  for (int i=0; i<OP_DURATION; i++)
    TwoOn74HC595_Bottom(SOLE_MODE, ML_FWDREV, ML_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    //Off74HC595_Bottom(FRIEND_MODE, MR_ONOFF);
    //Off74HC595_Bottom(SOLE_MODE, ML_ONOFF);
    //OffMotorRight(SOLE_MODE);
    OffMotorLeft_Legacy(SOLE_MODE);
}//testLM

void OnMotorLeft_REV_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (1<<ML_ONOFF) | (1<<ML_FWDREV));
}//OnMotorLeft_REV

//----------------------------------------------
void OnMotorLeft_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (1<<ML_ONOFF));
}//OnMotorLeft

//----------------------------------------------
void OffMotorLeft_Legacy(byte prevState)
{
  control74HC595(0, 0, prevState | (0<<ML_ONOFF));
}//OffMotorLeft


//*********************************************
// Test routines
//----------------------------------------------
void testMR_SOLE_MODE()
{
  for (int i=0; i<OP_DURATION; i++)
    OnMotorRight_Legacy(SOLE_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorRight_Legacy(SOLE_MODE);
    
  //testOffBothMotors();
  for (int i=0; i<OP_DURATION; i++)
    OnMotorRight_REV_Legacy(SOLE_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorRight_Legacy(SOLE_MODE);
}//testMR_SOLE_MODE

//----------------------------------------------
void testMR_FRIEND_MODE()
{
  curState74HC595= (1<<ML_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    OnMotorRight_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorRight_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OnMotorRight_REV_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorRight_Legacy(FRIEND_MODE);
    
}//testMR_FRIEND_MODE

//----------------------------------------------
void testML_SOLE_MODE()
{
  for (int i=0; i<OP_DURATION; i++)
    OnMotorLeft_Legacy(SOLE_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorLeft_Legacy(SOLE_MODE);
    
  //testOffBothMotors();
  for (int i=0; i<OP_DURATION; i++)
    OnMotorLeft_REV_Legacy(SOLE_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorLeft_Legacy(SOLE_MODE);
    
}//testML_SOLE_MODE

//----------------------------------------------
void testML_FRIEND_MODE()
{
  curState74HC595= (1<<MR_ONOFF);
  for (int i=0; i<OP_DURATION; i++)
    OnMotorLeft_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorLeft_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OnMotorLeft_REV_Legacy(FRIEND_MODE);
    
  for (int i=0; i<OP_DURATION; i++)
    OffMotorLeft_Legacy(FRIEND_MODE);
    
}//testML_FRIEND_MODE


//----------------------------------------------
//--- Legacy LED code snippets ---------------
//----------------------------------------------
//--- Green LED
void turnONgreenLED(byte SoleORfriend)
{
  curState74HC595 = SoleORfriend | (1<<PGM_START);
  control74HC595(0, 0, curState74HC595);
}

void turnOFFgreenLED(byte SoleORfriend)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(PGM_START);
  else
    curState74HC595=0;
  control74HC595(0, 0, curState74HC595);
}

//--- Yellow LED
void turnONyellowLED(byte SoleORfriend)
{
  curState74HC595 = SoleORfriend | (1<<PGM_CMD);
  control74HC595(0, 0, curState74HC595);
}

void turnOFFyellowLED(byte SoleORfriend)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(PGM_CMD);
  else
    curState74HC595=0;
  control74HC595(0, 0, curState74HC595);
}

//--- Red LED
void turnONredLED(byte SoleORfriend)
{
  curState74HC595 = SoleORfriend | (1<<PGM_END);
  control74HC595(0, 0, curState74HC595);
}

void turnOFFredLED(byte SoleORfriend)
{
  if (SoleORfriend == FRIEND_MODE)
    friendlyOFF(PGM_END);
  else
    curState74HC595=0;
  control74HC595(0, 0, curState74HC595);
}

void testLEDs_Legacy()
{
  //turn on all the LEDs
  On74HC595_Bottom(FRIEND_MODE, PGM_START);
  On74HC595_Bottom(FRIEND_MODE, PGM_CMD);
  On74HC595_Bottom(FRIEND_MODE, PGM_END);
  delay(1000);
  //off a single green LED while leaving others on
  Off74HC595_Bottom(FRIEND_MODE, PGM_START);
  delay(500);
  //on a single green LED while leaving others on
  On74HC595_Bottom(FRIEND_MODE, PGM_START);
  delay(500);
  //off a single yellow LED while leaving others on
  Off74HC595_Bottom(FRIEND_MODE, PGM_CMD);
  delay(500);
  //on a single yellow LED while leaving others on
  On74HC595_Bottom(FRIEND_MODE, PGM_CMD);
  delay(500);
  //off a single red LED while leaving others on
  Off74HC595_Bottom(FRIEND_MODE, PGM_END);
  delay(500);
  //on a single red LED while leaving others on
  On74HC595_Bottom(FRIEND_MODE, PGM_END);
  delay(500);
  //off all the LEDs using SOLE_MODE
  Off74HC595_Bottom(SOLE_MODE, PGM_START);
  delay(1000);
}

//------------------------------------------
void turnOnLED_Top()
{
    for (int i=0; i<8; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      digitalWrite(latchPin, HIGH);
      delay(500);
    }

    for (int i=0; i<256; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, i); //Q0
      digitalWrite(latchPin, HIGH);
      delay(300);
    }
    
}//turnOnLED_Top

void turnOnLED_Middle()
{
    for (int i=0; i<8; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      digitalWrite(latchPin, HIGH);
      delay(500);
    }

    for (int i=0; i<256; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, i); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      digitalWrite(latchPin, HIGH);
      delay(300);
    }
}//turnOnLED_Middle

void turnOnLED_Bottom()
{
    for (int i=0; i<8; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      digitalWrite(latchPin, HIGH);
      delay(300);
    }

    for (int i=0; i<256; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, i); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      digitalWrite(latchPin, HIGH);
      delay(300);
    }
}//turnOnLED_Bottom

void turnOnLED0()
{
    for (int i=0; i<8; i++)
    {
      digitalWrite(latchPin, LOW);
      //shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      //shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      //shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      shiftOut(dataPin, clockPin, MSBFIRST, LEDtest[i]); //Q0
      digitalWrite(latchPin, HIGH);
      delay(500);
    }
}//turnOnLED0

