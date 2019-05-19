#include "defs.h"
#include "externs.h"

//*********************************************
//--- Maneuvarability control
//----------------------------------------------
void intermittentGoForward()
{
  goForward();
  delay(FWD_DURATION);
  OffBothMotors();
  delay(FWD_DURATION/3);
}//intermittentGoForward

//----------------------------------------------
void goForward()
{
  TwoOn74HC595_Bottom(SOLE_MODE, ML_ONOFF, MR_ONOFF);
  // forward moving is a default occasion, so that usually do not need duration
  // Arduino "loop" speed is quick enough not to pause the forward moving
  //delay(FWD_DURATION);
}//goForward

//take more time than using TwoOn74HC595_Bottom, so that discard in real-time situation
void goForward_LongTimeConsume()
{
  trippleXwrite(ML_ONOFF, HIGH);
  trippleXwrite(MR_ONOFF, HIGH);

  trippleXwrite(ML_FWDREV, LOW);
  trippleXwrite(MR_FWDREV, LOW);
  // forward moving is a default occasion, so that usually do not need duration
  // Arduino "loop" speed is quick enough not to pause the forward moving
  //delay(FWD_DURATION);
}//goForward_LongTimeConsume

//----------------------------------------------
void goBackward()
{
  trippleXwrite(ML_ONOFF, HIGH);
  trippleXwrite(ML_FWDREV, HIGH);

  trippleXwrite(MR_ONOFF, HIGH);
  trippleXwrite(MR_FWDREV, HIGH);

  // backward moving is an interim occasion, so that give some duration
  delay(BWD_DURATION);
}//goBackward

//----------------------------------------------
// fast turn by moving both motors in opposite direction
void turnLeft(int duration)
{
  for (int i=0; i<duration; i++) 
  {
    trippleXwrite(MR_ONOFF, HIGH);
    trippleXwrite(ML_ONOFF, HIGH);

    trippleXwrite(ML_FWDREV, HIGH);
    trippleXwrite(MR_FWDREV, LOW);
  }
  OffBothMotors();
  delay(STOP_DURATION_AFTER_TURN);
}//turnLeft

//----------------------------------------------
// fast turn by moving both motors in opposite direction
void turnRight(int duration)
{
  for (int i=0; i<duration; i++)
  {
    trippleXwrite(ML_ONOFF, HIGH);
    trippleXwrite(MR_ONOFF, HIGH);

    trippleXwrite(ML_FWDREV, LOW);
    trippleXwrite(MR_FWDREV, HIGH);  
  }
  OffBothMotors();
  delay(STOP_DURATION_AFTER_TURN);
}//turnRight

//----------------------------------------------
void OffBothMotors()
{
  trippleXwrite(ML_ONOFF, LOW);
  trippleXwrite(MR_ONOFF, LOW);
  //In controlling on & off of motors, delay is not good for fine control
  //delay(STOP_DURATION);
}//OffBothMotors


