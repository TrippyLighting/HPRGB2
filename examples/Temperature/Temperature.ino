#include <Wire.h>
#include <HPRGB2.h>
HPRGB ledShield;

void setup() 
{
  ledShield.begin();
  Serial.begin(9600);
}

void loop() 
{
  float intTemp = ledShield.getIntTemp(); // Get local temperature in Celcius degree
  float extTemp = ledShield.getExtTemp(); // Get remote temperature in Celcius degree
  Serial.print("Local Temp = ");
  Serial.print(intTemp,DEC);
  Serial.print(" C  ");
  Serial.print("Remote Temp = ");
  Serial.print(extTemp,DEC);
  Serial.println(" C");
  
  float intTempF = ledShield.getIntTempF();// Get local temperature in Fahrenheit degree
  float extTempF = ledShield.getExtTempF();// Get remote temperature in Fahrenheit degree
  Serial.print("Local Temp = ");
  Serial.print(intTempF,DEC);
  Serial.print(" F  ");
  Serial.print("Remote Temp = ");
  Serial.print(extTempF,DEC);
  Serial.println(" F");
  
  delay(1000);
}