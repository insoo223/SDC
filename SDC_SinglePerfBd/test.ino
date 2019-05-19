

//-----------------------------------------
// Test 4digit7seg module sit on the sensor board and run by Three 74HC595 module 
//-----------------------------------------
void test4digit7seg()
{
  bool serialDebug = false; //true false
  
  for (int i=0; i<10000; i++)
  {
    switch (i % 3)
    {
      case 0:
        trippleXwrite(PGM_START, HIGH);
        trippleXwrite(PGM_CMD, LOW);
        trippleXwrite(PGM_END, LOW);
        break;
      case 1:
        trippleXwrite(PGM_START, LOW);
        trippleXwrite(PGM_CMD, HIGH);
        trippleXwrite(PGM_END, LOW);
        break;
      case 2:
        trippleXwrite(PGM_START, LOW);
        trippleXwrite(PGM_CMD, LOW);
        trippleXwrite(PGM_END, HIGH);
        break;
    }//switch
    if (serialDebug)
    {
      Serial.println(curTopX & 0xFF, BIN);
      Serial.println(curMiddleX & 0xFF, BIN);
      Serial.println(curBottomX & 0xFF, BIN);
      Serial.println("--------------------------");
    }
    disp4digit7seg_trippleX(i, 0, 50);    
    if (serialDebug)
    {
      Serial.println(i % 3);
      Serial.println("--------------------------");
      Serial.println(curTopX & 0xFF, BIN);
      Serial.println(curMiddleX & 0xFF, BIN);
      Serial.println(curBottomX & 0xFF, BIN);
      Serial.println("--------------------------");
    }
  }//for  
  
}//test4digit7seg

//-------------------------------------------------- 
void showSameNum() 
{
  
  for (int i=0; i<4; i++)
  {
    for(int k=0; k<50; k++)
    {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //control 1s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x02); //control 10s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x04); //control 100s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x08); //control 1000s common cathod transistor
      if (i == 9)
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i] | OnesDigit[DP]); //display digit
      else
        shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
    }
  }
}//showSameNum

//-------------------------------------------------- 
void showDiffNum() 
{
  
  for (int i=0; i<10; i++)
  {
    for(int k=0; k<50; k++)
    {
      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //control 1s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+3]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x02); //control 10s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+2]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x04); //control 100s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i+1]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 0x08); //control 1000s common cathod transistor
      shiftOut(dataPin, clockPin, MSBFIRST, OnesDigit[i]); //display digit
      digitalWrite(latchPin, HIGH);
      delay(5);
    }
  }
}//showDiffNum

//-------------------------------------------------- 
void debugTrippleX_LED()
{
  trippleXwrite(PGM_START, HIGH);
  Serial.println("trippleXwrite(PGM_START, HIGH)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);

  trippleXwrite(PGM_CMD, HIGH);
  Serial.println("trippleXwrite(PGM_CMD, HIGH)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);

  trippleXwrite(PGM_END, HIGH);
  Serial.println("trippleXwrite(PGM_END, HIGH)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);

  trippleXwrite(PGM_START, LOW);
  Serial.println("trippleXwrite(PGM_START, LOW)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);

  trippleXwrite(PGM_CMD, LOW);
  Serial.println("trippleXwrite(PGM_CMD, LOW)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);
 
  trippleXwrite(PGM_END, LOW);
  Serial.println("trippleXwrite(PGM_END, LOW)");
  Serial.println(curBottomX & 0xFF, BIN);
  Serial.println(curMiddleX & 0xFF, BIN);
  Serial.println(curTopX & 0xFF, BIN);
  Serial.println("--------------------------");
  delay(500);
}//debugTrippleX_LED

//-----------------------------------------
// Test LEDs on the sensor board
//-----------------------------------------
void testSensorBoardLED()
{
  OnLED_SensorBoard(GREEN_LED);
  delay(500);
  OnLED_SensorBoard(YELLOW_LED);
  delay(500);
  OnLED_SensorBoard(RED_LED);
  delay(500);

  OffLED_SensorBoard(GREEN_LED);
  delay(500);
  OffLED_SensorBoard(YELLOW_LED);
  delay(500);
  OffLED_SensorBoard(RED_LED);
  delay(500);
  for (int i=0; i<2; i++)
  {
    OnLED_SensorBoard(GREEN_LED);
    OnLED_SensorBoard(YELLOW_LED);
    OnLED_SensorBoard(RED_LED);
    delay(300);
  
    OffLED_SensorBoard(GREEN_LED);
    OffLED_SensorBoard(YELLOW_LED);
    OffLED_SensorBoard(RED_LED);
    delay(300);
  }
}//testLED_SensorBoardLED

//-----------------------------------------
// Test Motor
//-----------------------------------------
void testTrippleX_Motor()
{
  //each motor fwd
  disp4char_trippleX("LFWD", 50);
  trippleXwrite(ML_ONOFF, HIGH);
  trippleXwrite(ML_FWDREV, LOW);
  delay(1000);
  trippleXwrite(ML_ONOFF, LOW);
  delay(1000);
  disp4char_trippleX("RFWD", 50);
  trippleXwrite(MR_ONOFF, HIGH);
  trippleXwrite(MR_FWDREV, LOW);
  delay(1000);
  trippleXwrite(MR_ONOFF, LOW);
  delay(1000);

  //each motor bwd
  disp4char_trippleX("LBWD", 50);
  trippleXwrite(ML_ONOFF, HIGH);
  trippleXwrite(ML_FWDREV, HIGH);
  delay(1000);
  trippleXwrite(ML_ONOFF, LOW);
  trippleXwrite(ML_FWDREV, LOW);
  delay(1000);
  disp4char_trippleX("RBWD", 50);
  trippleXwrite(MR_ONOFF, HIGH);
  trippleXwrite(MR_FWDREV, HIGH);
  delay(1000);
  trippleXwrite(MR_ONOFF, LOW);
  trippleXwrite(MR_FWDREV, LOW);
  delay(1000);
  
  disp4char_trippleX("GFWD", 50);
  goForward();
  delay(1000);
  
  OffBothMotors();
  delay(1000);

  disp4char_trippleX("GBWD", 50);
  goBackward();
  delay(1000);

  OffBothMotors();
  delay(1000);
  
  disp4char_trippleX("TLFT", 50);
  testTurnLeft();
  disp4char_trippleX("TRTE", 50);
  testTurnRight();
}//testTrippleX_Motor

//-----------------------------------------
void testTurnLeft()
{
  while (!doneTL)
  {
    turnLeft(RECT_TL_ITERATION);
    OffBothMotors();
    delay(500);
    turnLeft(SHORT_TURN_ITERATION);
    OffBothMotors();
    delay(STOP_DURATION);
    
    doneTL=true;
  }
}//testTurnLeft

//----------------------------------------------
void testTurnRight()
{
  while (!doneTR)
  {
    turnRight(RECT_ITERATION);
    OffBothMotors();
    delay(STOP_DURATION);
    doneTR=true;
  }
}//testTurnRight

//----------------------------------------------




