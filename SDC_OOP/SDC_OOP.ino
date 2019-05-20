/*
  SDC_OOP.ino

  Created by In soo Kim on 10/24/15.
  Release to Public domain

  Updated on 10/29/15
*/

#include "defs.h"

#include <trippleX.h>
#include <Four7segX.h>
/*
#include <L293DX.h>
#include <RTX.h>
#include <DHT11X.h>
#include <CD4021X.h>
#include <IRX.h>
#include <ButtonMenu.h>
#include <UXX.h>
*/
/*******include
#include <EEPROM.h>

//#include <ICCX.h>
#include "Wire.h"
include*******/

trippleX X(6, 7, 8); //Arduino Pro Mini digital pins assigned to latch, clock, data pins of 74HC595

// Variables are depending on schematic
// Q-pin assignments for top X in trippleX,
//  beginning A to G & last one is for DP
// 74HC595pins assigned for 4digit 7seg LED: A-G, DP pins, and Digit pins
// The numbers are for segment pins: 0 for QA, 1 for QB ... 7 for QH pin of 74HC595 (May 6, 2019)
//                   A, B, C, D, E, F, G,DP            <--- segments of 74HC595
//                  QA,QC,QG,QE,QD,QB,QH,QF            <--- connected to Top 74HC595 or num74HC595[MAXDIG7SEG-1]
//1st nibble for chip num of group of 74HC595, 2nd for pin num of a 74HC595(as of May 13, 2019)
byte segPins[MAXSEG] = {0x10, 0x12, 0x16, 0x14, 0x13, 0x11, 0x17, 0x15}; 
Four7segX _7X(segPins); //(as of May 9, 2019)

//----------------------------------------------
//--- Pin assignments of transistors driving common cathode of four digits of 7 segment LEDs.
//--- Each of the defined number represents Q-pin of the middle of tripple 74HC595s
//--- i.e. MAXCHIP74HC595-2 should be an array index for the middle of tripple or multiple 74HC595s.
//----------------------------------------------
byte digitPins[_7SEG_NUM_DIGITS] = {0x20, 0x21, 0x22, 0x23}; //(as of May 13, 2019)
//                                  QA,QB,QC,QD <--- connected to Mid 74HC595 or num74HC595[MAXDIG7SEG-2]

byte group595[MAXCHIP595];

/*
byte enable[2] = {10, 9};
byte motorNode[4] = {0,1,3,2};
L293DX motor(enable,motorNode);
RTX rt;
DHT11X dht11;
CD4021X cd4021;
IRX ir;
BUTTONMENU menu;
UXX ux;
*/
/*
*/
void setup()
{
  _7X.setNumDigits(_7SEG_NUM_DIGITS); //set number of digits in a multipl-digit 7 segment LED
  _7X.setDigitPins(digitPins); //set pins of each digit of a multipl-digit 7 segment LED

  //The minimum continuous time[ms] for the state, HIGH or LOW, of each digit with unnoticeable flickering.
  //Four digits look like to be shown simultaneously for their fast round-robin displaying.
  //role replaced by the legacy gSingleDigitDelay
  _7X.setSingleDigitDelay(_7SEG_SINGLE_DIGIT_DELAY);
  
  //Set time to DS3231 RTC module
  //rt.setDS3231time(30,35,11,5,19,5,17);

  Serial.begin(9600);
  //rt.dispBrightnessCtrl();
  
  //X.ctrlAll_legacy(0,0,0);
  for (byte i=0; i<MAXCHIP595; i++)
    group595[i]=0;
  X.updateX(group595);
  X.ctrlAll();

}//setup

void loop()
{
  //Production-time code requires only the following single statement!
  //To test SDC_tandem328P for it has NO RTC
  //   so that, night time brightness cannot be controlled.
  //_7X.setNightMode(false);
  _7X.setNightMode(true);
  //menu.selectOpMode();

  //-------- Test------------------
  //test_Master();  
  //ir.saveRefPure_AllIR(true, true);
  //test_IRX_initEEPROM();
  //test_IRX_getPure_Single();
  //test_CD4021X();
  //test_DHT11X();
  //test_RTX();
  //test_L293DX();
  //test_trippleX_7segNum_UpsideDown();
  //test_trippleX_7segChar();
  test_trippleX_7segNum();
  //allSeg_trippleX();
}//loop
