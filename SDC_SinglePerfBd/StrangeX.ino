
void testStrangeX()
{
  //test 7seg 
    //seq1s();
    //seq10s();
    //seq100s();
    seq1000s();
  
}

void seq1s()
{
  //1s 0
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(0), _BV(0) | curMiddleX, curBottomX);
  delay(500);
  
  //1s 1
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(1), _BV(1) | curMiddleX, curBottomX);
  delay(500);
  
  //1s 2
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(2), _BV(8) | curMiddleX, curBottomX);
  delay(500);
  //1s 3
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(3), _BV(2) | curMiddleX, curBottomX); 
  delay(500);
  
  //1s 4  
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(4), _BV(0) | curMiddleX, curBottomX);
  delay(500);
  
  //1s 5
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(5), _BV(8) | curMiddleX, curBottomX);
  delay(500);
  
  //1s 6
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(6), _BV(0) | curMiddleX, curBottomX);
  delay(500);

  //1s 7
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(7), _BV(1) | curMiddleX, curBottomX);
  delay(500);

  //1s 8
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(8), _BV(8) | curMiddleX, curBottomX);
  delay(500);

  //1s 9
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(9), _BV(2) | curMiddleX, curBottomX);
  delay(500);

  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
 
}//seq1s

void seq10s()
{
              // 0, 1,  2, 3, 4, 5,  6, 7,  8, 9
  byte midX[10] = {159, 33, 5, 9, 2, 11, 4, 33, 95, 39};
  int s=0, e= 0xff;

  if (SingleReadCD4021BE(PGM_MODEpinCD4021) == LOW)
    for (byte i=s; i<e; i++)
    {
      control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
      control74HC595 (getTopByteX_Num(0), i, curBottomX);
      Serial.println(i);
      delay(1000);
    }
  else
    for (byte k=0; k<10; k++)
    {
      control74HC595 (getTopByteX_Num(k), midX[k] | curMiddleX, curBottomX);
      delay(500);
    }
 
}//seq10s

//NOT YET FOUND 0 & 8
void seq100s()
{
                // 0,   1,  2,  3,  4, 5,  6, 7, 8, 9
  byte midX[10] = {0,   37, 81, 37, 36,83, 10,37,0, 7  };
  int s=0, e= 0xff;

  if (SingleReadCD4021BE(PGM_MODEpinCD4021) == LOW)
    for (byte i=s; i<e; i++)
    {
      control74HC595 (getTopByteX_Num(0), i | curMiddleX, curBottomX);
      //delay(100);
      bool onesTR = SingleReadCD4021BE(3);
      bool tensTR = SingleReadCD4021BE(4);
      bool hunsTR = SingleReadCD4021BE(5);
      bool thosTR = SingleReadCD4021BE(6);
      if ( (onesTR == LOW) && (tensTR == LOW) && (hunsTR == HIGH) && (thosTR == LOW) )
      {
        Serial.println(i);
        //delay(1000);
      }
    }
  else
    for (byte k=0; k<10; k++)
    {
      control74HC595 (getTopByteX_Num(k), midX[k] | curMiddleX, curBottomX);
      if (k==0)
        //control74HC595 (getTopByteX_Num(k), ~_BV(midX[k]) | curMiddleX, curBottomX);
        ;
      delay(500);
    }
 
}//seq100s

void seq1000s()
{
                // 0 , 1G, 2,   3G,  4G,  5,  6G, 7G,   8,  9G
  byte midX[10] = {84, 4,  32,  85,  84,  6,  0,  149,  22, 23  };
  int s=0, e= 0xff;

  if (SingleReadCD4021BE(PGM_MODEpinCD4021) == LOW)
    for (byte i=s; i<e; i++)
    {
      control74HC595 (getTopByteX_Num(9), i | curMiddleX, curBottomX);
        Serial.println(i);
        delay(1000);
    }
  else
    for (byte k=0; k<10; k++)
    {
      control74HC595 (getTopByteX_Num(k), midX[k] | curMiddleX, curBottomX);
      delay(500);
    }

  //1000s 0
  /*
  control74HC595 (0x00, _BV(DIGIT_K_PIN) | curMiddleX, curBottomX);
  control74HC595 (getTopByteX_Num(0), _BV(8) | curMiddleX, curBottomX);
  delay(1000);
  */
}

