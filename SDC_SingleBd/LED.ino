
//------------------------------------------
void OnLED_SensorBoard(byte color)
{
  trippleXwrite(color, HIGH);
  delay(LED_ON_DURATION);
}//OnGreenLED

//------------------------------------------
void OffLED_SensorBoard(byte color)
{
  trippleXwrite(color, LOW);
  delay(LED_OFF_DURATION);
}//OffGreenLED


