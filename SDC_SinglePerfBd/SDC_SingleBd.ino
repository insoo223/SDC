//**************************************************************//
//  Name    : _74HC595_7SegFourDigit_StripWired_OutExp.ino                                
//  Author  : Insoo Kim
//  Create  : 
//  Update  : Oct 5, 2015    
//  Notes   : Self-driving car on a single perforated board.
//          : Based on ver01 in _74HC595_7SegFourDigit_StripWired_OutExp folder
//          : 
//****************************************************************
#include "defs.h"
#include "externs.h"

//-----------------------------
void setup() 
{
  Serial.begin(9600);
  
  // ver 02 single piece of perf board
  latchPin = 6;
  clockPin = 7;
  dataPin = 8;
  
  latch4021 = 3;
  clock4021 = 4;
  data4021 = 5;
    
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(latch4021, OUTPUT);
  pinMode(clock4021, OUTPUT); 
  pinMode(data4021, INPUT);
  
  pinMode(MR_SPEEDpin, INPUT);
  pinMode(UxECHOpin, INPUT);
  
  //pinMode(PGM_MODEpin, INPUT);
  //pinMode(SET_IRpin, INPUT);
  setupL293D();
  //setupRT();
  //setupDHT11();
  
  //turn off motors, LEDs
  control74HC595(0, 0, 0);
  
  tirePerimeter = pi * tireD;
  curTime = millis();
  prevTime = curTime;  
  /*
  byte temp = 0;
  byte temp1 = 0;
  byte testObj = 0;
  byte res;
  testObj = 0b11110101;
  temp = (1<<PGM_START);
  temp1 = ~(1<<PGM_START);
  res = ~(testObj ^ ~(1<<PGM_START));
  Serial.print("(1<<PGM_START): 0b"); Serial.println(temp, BIN);
  Serial.print("~(1<<PGM_START): 0b");   Serial.println(temp1, BIN);
  Serial.print("~(testObj ^ ~(1<<PGM_START)): 0b");   Serial.println(res, BIN);
  */
  
  /*
  for (byte i=0; i<10; i++)
  {
    Serial.print(i); Serial.print(": "); Serial.println(getTopByteX_Num(i), BIN);
    //disp4digit7seg_trippleX(travelSpeed, 0, 1); 
  }
  
  for (byte c='A'; c<'Z'; c++)
  {
    Serial.print(c); Serial.print(": "); Serial.println(getTopByteX_ABC(c), BIN);
    dispABC_trippleX(c, 50);
    delay(500);
  }
  */
}//setup

//-----------------------------
int playMode = 1;
void loop() 
{
  //testStrangeX();  
  //loopL293D();
  /*--- test Bit Angle Modulation 
  //mainISR();
  --- test Bit Angle Modulation */
  
  //testTrippleX_Motor();
  //getPureIRsignal(true , false);  
  //delay(2000);
  
  /*--- test Self-Driving Car 
    //--- Set IR sensitivity and test motor control based on the sensitivy just adjusted
    //--- (End) Set IR sensitivity and test motor control based on the sensitivy just adjusted 
    if ( SingleReadCD4021BE(PGM_MODEpinCD4021) == HIGH)
    {
      IRmode = IR_RUNmode; //IR_RUNmode IR_SETmode
      testIRs_Motor();
      //testUx_Motor();
    }
    else
    {
      IRmode = IR_SETmode; //IR_RUNmode IR_SETmode
      testIRs_ADC();
    }
  -----------*/  

/* */
  disp4char_trippleX("LFWD", 50);
  delay(2000);
  disp4char_trippleX("RFWD", 50);
  delay(2000);
  disp4char_trippleX("LBWD", 50);
  delay(2000);
  disp4char_trippleX("RBWD", 50);
  delay(2000);
  //showDateTimeTemperatureHumidity();
  
  //setIR_RefVal_Turn_Auto();
  //intermittentGoForward();
  //testSensorBoardLED();
  //goForward();
  //delay(1000);
  //goBackward();
  //testTurnLeft();
  //OffGreenLED();
  //OffBothMotors();
  //delay(1000);
  //testTurnRight();
  //debugTrippleX_LED();
  
  /*---- test Ux
  loopUx();
  ------ (End) test Ux -----*/  
  
  /*--------
  if (SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
    playMode ^= 1;
  
  if (playMode == 0)
    showDateTimeTemperatureHumidity();
  else
  {
  }
  -----------*/  


  /*--- test DHT11 
  loopDHT11(); 
  --- (End) test DHT11 */
  
  //*--- test Real-Time module 
  //loopRT();
  //--- (End) test Real-Time module */

  //test4digit7seg();
  
  /*--- test IR sensors of front, left, and right
  getPureIRsignal(false, false);
  delay(1000);
  --- test IR sensors of front, left, and right */
  
  /*------ test read a single bit from CD4021BE 
  OffBothMotors();
  Serial.println( SingleReadCD4021BE(PGM_MODEpinCD4021) );
  delay(200);
  Serial.println( SingleReadCD4021BE(TactileButtonPinCD4021) );
  delay(200);
  Serial.println( SingleReadCD4021BE(MR_SPEEDpinCD4021) );
  delay(200);
  Serial.println("-------------------");
  -------- (End) test read a single bit from CD4021BE----------*/
  
  /*------ test read whole byte from CD4021BE 
  byte inputStatus; //char inputStatus;

  readCD4021BE(&inputStatus);
  Serial.println(inputStatus, BIN);

  //white space
  Serial.println("-------------------");
  
  //delay so all these print satements can keep up. 
  delay(1000);
  -------- (End) test read whole byte from CD4021BE----------*/
  
  /*------ test MR Speed
  goForward();
  checkSpeed();
  if ( abs(prevSpeed - travelSpeed) > 20 )
    disp4digit7seg_trippleX(travelSpeed, 0, 1);  
  //  disp4digit7seg(travelSpeed, 0);
  -------- (End) test MR Speed ----------*/
 
  /*------ IR Sensitivity check with oscilloscope
  //sensitivityCheckIR_ForOSC();
  
  //intermittentGoForward();
  //testDoubleStage74HC595_7segLED();
  //delay(1000);
  //testSingleStage74HC595_7segLED();
  ------ (End) IR Sensitivity check with oscilloscope */
  
  //Serial.println(SingleReadCD4021BE(SlideSwitch2ndPinCD4021)); //PGM_MODEpinCD4021 TactileButtonPinCD4021 SlideSwitch2ndPinCD4021
  //loopL293D();
  /*------ test 4 digits 7 segment LEDs through the Three 74HC595 module
  //disp4digit7seg_trippleX(1111, 0, 50);  
  //control74HC595 (getTopByteX_Num(1), _BV(DIGIT_U_PIN) | curMiddleX, curBottomX);
  //control74HC595 (0xff, 0xff, curBottomX);
  control74HC595 (0, 0xff, curBottomX);
  delay(20);
  control74HC595 (getTopByteX_Num(5), 37, curBottomX);
  -------- (End) test 4 digits 7 segment LEDs through the Three 74HC595 module ----------*/
  /*
  delay(500);
  disp4digit7seg_trippleX(2345, 0, 50);  
  delay(500);
  disp4digit7seg_trippleX(3456, 0, 50);  
  delay(500);
  */
  //disp4digit7seg_trippleX(5678, 0, 50);  
  
  /*
  control74HC595(0x44, 10, 0); //1
  delay(1000);
  control74HC595(0x00, 10, 0);
  delay(1000);
  control74HC595(0x44, 33, 0);  //10
  delay(1000);
  control74HC595(0x00, 33, 0);
  delay(1000);
  control74HC595(0x44, 37, 0);  //100
  delay(1000);
  control74HC595(0x00, 37, 0);
  delay(1000);
  
  control74HC595(0x44, 125, 0);
  */
/*
  for (int i=37; i<=0xff; i++)
  {
    control74HC595(0x44, i, 0);
    Serial.print("i: "); Serial.println(i);
    delay(2000);
  }
  */
  
  /*--- initialize the internal EEPROM of ATmega328p
  if ( SingleReadCD4021BE(TactileButtonPinCD4021) == LOW)
    initEEPROM();
  ------- (End) initialize the internal EEPROM of ATmega328p  ---------*/  
  
  
  //goForward();

  //testIRs_Motor();
  //readIR();
  //turnRight();
  //turnLeft(RECT_TL_ITERATION));
  //goBackward();
  //delay(1000);
  //testIRright();
  //testIRleft();
  
  /*--- Check speed
  //--- test done successfully on Aug 27, 2015 (revised code test Sep 07, 2015)
  goForward();
  checkSpeed();
  --- (End) Check speed */

  //--- Go backward setting both motors on and both relay switches on
  //--- test done successfully on Aug 27, 2015 (revised code test Sep 07, 2015)
  //goBackward();

  /*--- test Turn left & right
  //--- turn left about 90 degree and stop
  //--- test done successfully on Aug 27, 2015 (revised code test Sep 07, 2015)
  testTurnLeft();

  //--- turn right about 90 degree and stop
  //--- test done successfully on Aug 27, 2015 (revised code test Sep 07, 2015)
  testTurnRight();
  --- (End) test Turn left & right*/

  /*--- Go & stop while the other tire runs.
  //--- test done successfully on Aug 27, 2015
  testMR_FRIEND_MODE();
  testML_FRIEND_MODE();
  --- (End) Go & stop while the other tire runs. */

  //--- Go & stop while letting the other tire stop.
  //--- test done successfully on Aug 27, 2015
  //testMR_SOLE_MODE();
  //testML_SOLE_MODE();

  //--- test each motor
  //--- test done successfully on Sep 07, 2015
  //--- enhancement point: devise Off74HC595_Bottom instead of OffMotorLeft or OffMotorRight
  //testRM();
  //testLM();

  //--- test signal LED i.e. LED for PGM_START(Green), PGM_CMD(Yellow), and PGM_END(Red)
  //--- test done successfully on Sep 07, 2015
  //testLEDs();
  
  //turnOnLED0();
  //turnOnLED_Bottom();
  //turnOnLED_Middle();
  //turnOnLED_Top();
}//loop

//-----------------------------------------------
void showDateTimeTemperatureHumidity()
{
  displayTime_4digit7seg();
  displayTemp_4digit7seg();
  delay(1000);
}//showDateTimeTemperatureHumidity

//-----------------------------------------------
// bit-oriented control for 74HC595
// Control three 74HC595 output eXpansion chips
// created: Thu Sep24,2015
// input args
//    ctrlID: refer "defs.h", 74HC595 pin assignments
//    state: HIGH to set, LOW to clear a corresponding pin of a 74HC595
void trippleXwrite(char ctrlID, char state)
{
  char bottomByte, middleByte, topByte;
  char sectorX, dataX;
  
  // The first bit  in the upper nibble, bit 4, of ctrlID represents 
  // which of three 74HC595s, called "sectorX", holds the corresponding pin
  // sectorX 0 for bottom, 1 for middle, and 2 for top sector of three 74HC595s
  sectorX = ctrlID >> SECTOR_BIT_LOCATION_74HC595;
  
  // The bits in the lower nibble, bit3 to bit0, of ctrlID represent
  // the corresponding pin of the 74HC595 chip of ctrlID  
  dataX = ctrlID & DATA_BITS_MASK_74HC595;
  
  switch (sectorX)
  {
    case 0: //bottom 74HC595
      if  (state == HIGH)
        bottomByte = _BYTE(curBottomX) | _BV(dataX); 
      else
        bottomByte = _BYTE(curBottomX) & ~_BV(dataX);
      middleByte = _BYTE(curMiddleX);
      topByte = _BYTE(curTopX);
      //update the current bottom byte of 74HC595
      curBottomX = bottomByte;
      break;
      
    case 1: //middle 74HC595
      bottomByte = _BYTE(curBottomX);
      if  (state == HIGH)
        middleByte = _BYTE(curMiddleX) | _BV(dataX);
      else
        middleByte = _BYTE(curMiddleX) & ~_BV(dataX);
      topByte = _BYTE(curTopX);
      //update the current middle byte of 74HC595
      curMiddleX = middleByte;
      break;
      
    case 2: //top 74HC595
      bottomByte = _BYTE(curBottomX);
      middleByte = _BYTE(curMiddleX);
      if  (state == HIGH)
        topByte = _BYTE(curTopX) | _BV(dataX);
      else
        topByte = _BYTE(curTopX) & ~_BV(dataX);
      //update the current top byte of 74HC595
      curTopX = topByte;
      break;
  }//switch(sectorX)
            
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bottomByte); //Bottom 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, middleByte); //Middle 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, topByte); //Top 74HC595
  digitalWrite(latchPin, HIGH);
}//trippleXwrite

//-----------------------------------------------
// byte-oriented control for 74HC595
void control74HC595(byte topBYTE, byte middleBYTE, byte bottomBYTE)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, bottomBYTE); //Bottom 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, middleBYTE); //Middle 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, topBYTE); //Top 74HC595
  digitalWrite(latchPin, HIGH);

}//control74HC595



