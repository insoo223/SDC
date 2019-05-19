void setupL293D()
{
  pinMode(EN1pin, OUTPUT); // PWM
  pinMode(EN2pin, OUTPUT); // PWM
}

void LFwd()
{
  digitalWrite(EN1pin, HIGH);
  control74HC595 (0x44, 10, 6);
}

void LBwd()
{
  digitalWrite(EN1pin, HIGH);
  control74HC595 (0x44, 10, 8);
}

void LFwdAndBwd()
{
  LFwd();
  delay(2000);
  digitalWrite(EN1pin, LOW);
  delay(2000);
  control74HC595 (0, 0, 0); //16
  LBwd();
  delay(2000);
}

void LFwdAndStop()
{
  LFwd();
  delay(2000);
  control74HC595 (0x44, 10, 17);
  delay(2000);
}

void LBwdAndStop()
{
  LFwd();
  delay(2000);
  control74HC595 (0x44, 10, 13);
  delay(2000);
}

void LNoMove()
{
  LFwd();
  delay(2000);
  control74HC595 (0x44, 10, 12);
  delay(2000);
}

void RFwd()
{
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 8);
}

void RFwdAndStop()
{
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 8);
  delay(2000);
  control74HC595 (0x44, 10, 0);
  delay(2000);
}

void RBwd()
{
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 6);
}

//void turnLeft()
//after rewiring MR 
void goBwd()
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 14); //12 15 31
}

//void turnRight()
//after rewiring MR 
void goFwd()
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 1); //5
}

//after rewiring MR 
void turnRight()
{
  goFwd();
  digitalWrite(EN2pin, LOW);
  delay(50);
}  

//after rewiring MR 
void turnLeft()
{
  goFwd();
  digitalWrite(EN1pin, LOW);
  delay(50);
}  

void LstopRBwd()
{
  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0xff, 0xff, 4);
}
void loopL293D()
{

  digitalWrite(EN1pin, HIGH);
  digitalWrite(EN2pin, HIGH);
  control74HC595 (0x44, 10, 14); //12 15 31
  
  //analogWrite(EN1pin, 250);
  //digitalWrite(EN1pin, HIGH);
  //digitalWrite(EN2pin, HIGH);
  //digitalWrite(EN2pin, HIGH);
  //digitalWrite(MLPpin, HIGH);
  //digitalWrite(MLNpin, LOW);

  //Left Fwd
  //control74HC595 (0x44, 10, 6); //Good!
  // control74HC595 (getTopByteX_Num(1), _BV(1) | curMiddleX, 0x02); //NG
  //control74HC595 (getTopByteX_Num(1), _BV(1) | curMiddleX, 6); //NG
  

  //digitalWrite(EN1pin, LOW);
  //delay(2000);
  //digitalWrite(EN1pin, HIGH);

  //Left Bwd
  //control74HC595 (0x44, 10, 8); //Good!
  //delay(2000);
  //control74HC595 (0x44, 10, 0);
  //delay(2000);
  /*
  for (int i=0; i<0xff; i++)
  {
    control74HC595 (i, 10, i);
    Serial.println(i);
    delay(3000);
  }
  */

  //digitalWrite(MRPpin, HIGH);
  //digitalWrite(MRNpin, LOW);

}
  
