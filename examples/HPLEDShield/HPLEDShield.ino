#include <Wire.h>
#include <HPRGB2.h>


HPRGB ledShield; // default mcp4728 id(0) and default PCA9685 id(0)

void setup()
{
  ledShield.begin();
  ledShield.setCurrent(350,350,350); // set maximum current for channel 1-3 (mA)
  ledShield.setFreq(600);// operation frequency of the LED driver (KHz)
  ledShield.eepromWrite();// write current settings to EEPROM
  delay(100); // wait for EEPROM writing
}


void loop()
{
  ledShield.goToRGB(255,255,255); delay (3000); //go to RGB color (white) now
  ledShield.goToRGB12(4095,4095,4095); delay (3000); //go to 12bit RGB (white) now
  ledShield.goToRGB(255,0,0); delay (3000); //go to RGB color (red)
  ledShield.goToRGB(0,255,0); delay (3000);//go to RGB color (green)
  ledShield.goToRGB(0,0,255); delay (3000); //go to RGB color (blue)
  ledShield.goToHSB(0,0,255); delay (3000); //go to HSV color (Hue, Saturation, brightness)
}