/*
 
 RTX.cpp
 SDC
 
 Created by In soo Kim on 10/25/15.
 Release to Public domain
 
 Updated on 10/29/15
*/

#include "Arduino.h"
#include "RTX.h"

//--------------------------------------------------
RTX :: RTX()
{
    Wire.begin();
    //Serial.begin(9600);
    // set the initial time here:
    // DS3231 seconds, minutes, hours, day, date, month, year
    //setDS3231time(30,58,14,5,15,10,15);
    
}//RTX

//----------------------------------------------------
// Convert normal decimal numbers to binary coded decimal
byte RTX :: decToBcd(byte val)
{
    return( (val/10*16) + (val%10) );
}

//----------------------------------------------------
// Convert binary coded decimal to normal decimal numbers
byte RTX :: bcdToDec(byte val)
{
    return( (val/16*10) + (val%16) );
}


//----------------------------------------------------
void RTX :: setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                          dayOfMonth, byte month, byte year)
{
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

//----------------------------------------------------
void RTX :: readDS3231time(byte *second,
                           byte *minute,
                           byte *hour,
                           byte *dayOfWeek,
                           byte *dayOfMonth,
                           byte *month,
                           byte *year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    if ((*hour >= 17) || (*hour <= 7))
    {
        gNightMode = true;
        gSingleDigitDelay = 0;
    }
    else
    {
        gNightMode = false; //false true
        gSingleDigitDelay = SINGLE_DIGIT_DELAY;
    }
    
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}//readDS3231time

//----------------------------------------------------
void RTX :: dispBrightnessCtrl()
{
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // when reading current time, 7 seg LED display brightness is set according to the time
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,  &year);
}//dispBrightnessCtrl
//----------------------------------------------------
void RTX :: displayTime()
{
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,  &year);
    // send it to the serial monitor
    Serial.println("Current time is...");
    Serial.print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10)
    {
        Serial.print("0");
    }
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second<10)
    {
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Day of week: ");
    switch(dayOfWeek){
        case 1:
            Serial.println("Sunday");
            break;
        case 2:
            Serial.println("Monday");
            break;
        case 3:
            Serial.println("Tuesday");
            break;
        case 4:
            Serial.println("Wednesday");
            break;
        case 5:
            Serial.println("Thursday");
            break;
        case 6:
            Serial.println("Friday");
            break;
        case 7:
            Serial.println("Saturday");
            break;
    }
}

//----------------------------------------------------
void RTX :: displayTime_4digit7seg()
{
    Four7segX _7X;
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    int fourDigits;
    byte chinese_dayOfWeek;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    fourDigits = month*100 + dayOfMonth;
    _7X.disp4digits(fourDigits, 0, DISP_4digit7seg_ITERATION_DATE);
    
    chinese_dayOfWeek = dayOfWeek-1;
    if (chinese_dayOfWeek == 0)
        chinese_dayOfWeek = 7;
    _7X.disp4digits(chinese_dayOfWeek, 0, DISP_4digit7seg_ITERATION_DAY);
    
    fourDigits = hour*100 + minute;
    _7X.disp4digits(fourDigits, 0, DISP_4digit7seg_ITERATION_TIME);
    
}//displayTime_4digit7seg

//----------------------------------------------------
