//#include <DHT_SimpleLib.h>
#include <dht.h>
dht DHT;
 
void setupDHT11(){
  pinMode(dht_dpin, INPUT);
}//setup

void displayTemp_4digit7seg()
{
  DHT.read11(dht_dpin);
  disp4digit7seg_trippleX(DHT.temperature * 100 + DHT.humidity, 0, DISP_4digit7seg_ITERATION_TEMPHUMID); 
}

void loopDHT11(){
  //SingleReadCD4021BE(DHT11_DATApinCD4021);
  //This is the "heart" of the program.
  DHT.read11(dht_dpin);
  
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("% ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature);
  disp4digit7seg_trippleX(DHT.temperature * 100 + DHT.humidity, 0, 50); 
  Serial.println("C ");
  delay(3000);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
}// loop()
