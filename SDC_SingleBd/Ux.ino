/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */


#define DISPTIME 5  //display time [second]

byte maximumRange = 250; // Maximum range needed (default 200)
byte minimumRange = 0; // Minimum range needed
int duration, distance; // Duration used to calculate distance


boolean showTimeOut = false;
byte showCnt=0;
//------------------------------------------------
void loopUx() 
{

  measureUx(false, true);
/*---------------------
  //switch pressed
  if (digitalRead(swPin) == LOW)
  {
    measureUx(false, true);
    showTimeOut = false;
    showCnt=0;
  }//if (digitalRead(swPin) == LOW)
  //switch released
  else
  {
    //if within show time, display the measured distance\
    if (!showTimeOut)
    {
      digitalWrite(latchPin, HIGH);
      showCnt++;
      //delay(500);
    }//if (!showTimeOut)
     
    if (showCnt > DISPTIME*2)
    {
      dispBlank();
      digitalWrite(latchPin, LOW);
      showTimeOut = true;
      //showCnt=0;
    }//if (showCnt > DISPTIME*2)
    delay(100);
  }//else
---------------------*/

}//loop

//------------------------------------------------
//bool measureUx_SerialDebug = false; //true false

int measureUx(bool DebugSerial, bool Debug7seg)
{
  if (DebugSerial)
    Serial.println("measureUx: ");
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 //digitalWrite(trigPin, LOW); 
 //Off74HC595_Middle(FRIEND_MODE, Ux_TRIG);
 trippleXwrite(Ux_TRIG, LOW);
 delayMicroseconds(2); 

 //digitalWrite(trigPin, HIGH);
 //On74HC595_Middle(FRIEND_MODE, Ux_TRIG);
 trippleXwrite(Ux_TRIG, HIGH);
 delayMicroseconds(10); 
 
 //digitalWrite(trigPin, LOW); 
 //Off74HC595_Middle(FRIEND_MODE, Ux_TRIG);
 trippleXwrite(Ux_TRIG, LOW);
 
 //duration = pulseIn_CD4021(UxECHOpinCD4021, HIGH);
 duration = pulseIn(UxECHOpin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 if (Debug7seg)
	 disp4digit7seg_trippleX(distance, 0, 50);
  if (DebugSerial)
  {
     Serial.print("distance: ");
     Serial.println(distance);
  }

 if (distance >= maximumRange || distance <= minimumRange)
 {
   distance = 40;
 // Send a negative number to computer and Turn LED ON 
 //to indicate "out of range" 
  if (DebugSerial)
     Serial.println("-1");
 //digitalWrite(LEDPin, HIGH); 
 }
 
 return (distance);
}//measureUx  

unsigned long pulseIn_CD4021(uint8_t pin, uint8_t state) 
{

    unsigned long pulseWidth = 0;
    unsigned long loopCount = 0;
    unsigned long loopMax = 5000000;

    // While the pin is *not* in the target state we make sure the timeout hasn't been reached.
    //while ((digitalRead(pin)) != state) 
    while ((SingleReadCD4021BE(pin)) != state) 
    {
      //Serial.print( "SingleReadCD4021BE(pin)" );
      //Serial.println( (SingleReadCD4021BE(pin)) );
      
        if (loopCount++ == loopMax) 
        {
            return 0;
        }
    }

    // When the pin *is* in the target state we bump the counter while still keeping track of the timeout.
    //while ((digitalRead(pin)) == state) 
    while ((SingleReadCD4021BE(pin)) == state) 
    {
        if (loopCount++ == loopMax) 
        {
            return 0;
        }
        pulseWidth++;
    }

    // Return the pulse time in microsecond!
    return pulseWidth * 2.36; // Calculated the pulseWidth++ loop to be about 2.36uS in length.
}//pulseIn_CD4021



