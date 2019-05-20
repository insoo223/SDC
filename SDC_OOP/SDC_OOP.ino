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

// 74HC595pins assigned for 4digit 7seg LED: A-G, DP pins, and Digit pins
// The first eight numbers are for Segment pins: 0 for QA, 1 for QB ... 7 for QH pin of 74HC595 (May 6, 2019)
// The last four numbers are Digit pins: Ones, Tens, Hundreds, and Kilos(Thousands)
// i.e.       A, B, C, D, E, F, G,DP,D4,D3,D2,D1 <--- connected to 4digit-7segment LED
//           QA,QC,QG,QE,QD,QB,QH,QF,            <--- connected to Top 74HC595 or num74HC595[MAXDIG7SEG-1]
//                                   QA,QB,QC,QD <--- connected to Mid 74HC595 or num74HC595[MAXDIG7SEG-2]
//Four7segX _7X(0, 2, 6, 4, 3, 1, 7, 5, 0, 1, 2, 3); //depends on the HW schematic (as of May 8, 2019)
Four7segX _7X(0, 2, 6, 4, 3, 1, 7, 5); //(as of May 9, 2019)
//           QA,QC,QG,QE,QD,QB,QH,QF            <--- connected to Top 74HC595 or num74HC595[MAXDIG7SEG-1]
byte pinDigits[_7SEG_NUM_DIGITS] = {0, 1, 2, 3}; //(as of May 9, 2019)
//                                  QA,QB,QC,QD <--- connected to Mid 74HC595 or num74HC595[MAXDIG7SEG-2]

byte pos74HC595[MAXDIG7SEG];
/*
L293DX motor;
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
  _7X.setNumDigits(_7SEG_NUM_DIGITS);
  _7X.setPinDigits(pinDigits);
  _7X.setSingleDigitDelay(_7SEG_SINGLE_DIGIT_DELAY);
  
  //Set time to DS3231 RTC module
  //rt.setDS3231time(30,35,11,5,19,5,17);

  Serial.begin(9600);
  //rt.dispBrightnessCtrl();
  
  //X.ctrlAll_legacy(0,0,0);
  for (byte i=0; i<MAXDIG7SEG; i++)
    pos74HC595[i]=0;
  X.ctrlAll(pos74HC595);

}//setup

void loop()
{
  //Production-time code requires only the following single statement!
  //To test SDC_tandem328P for it has NO RTC
  //   so that, night time brightness cannot be controlled.
  _7X.setNightMode(false);
  //_7X.setNightMode(true);
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
}//loop
