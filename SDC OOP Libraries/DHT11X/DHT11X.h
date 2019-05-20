/*
 DHT11X.h
 SDC
 
 Created by In soo Kim on 10/28/15.
 Release to Public domain
 
 Updated on 10/29/15
 */

#ifndef DHT11X_H
#define DHT11X_H

#include "Arduino.h"
#include "trippleX.h"
#include "Four7segX.h"

#define DISP_4digit7seg_ITERATION_TEMPHUMID       200

#define TIMEOUT 10000

//----------------------------------------------
//--- Pin assignments for DHT11 data 
//----------------------------------------------
#define DHTpin 11

class DHT11X
{
    
public:
    DHT11X();
    void displayTempHumidity(int delayDuration);
    double humidity;
    double temperature;

    
private:
    int read11(uint8_t pin);
    int read22(uint8_t pin);
    uint8_t _dhtPin;
    uint8_t bits[5];  // buffer to receive data
    int read(uint8_t pin);

};
#endif /* DHT11X_H */
