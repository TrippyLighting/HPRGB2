/*

  HPRGB2.cpp - Arduino library for High Power RGB LED Shield Version 2.5
  http://ledshield.wordpress.com
  
*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "HPRGB2.h"
#include <avr/pgmspace.h>
/* _____PUBLIC FUNCTIONS_____________________________________________________ */
#define CIEL_8_12(a) (pgm_read_word_near(CIEL_8_12 + a)) // CIE Lightness lookup table functions
#define CIEL_10_12(a) (pgm_read_word_near(CIEL_10_12 +a))

/*
12 bits PWM to CIE Luminance conversion
L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
L* = 903.3(Y/Yn), Y/Yn <= 0.008856
*/
//lookup table for 256 CIE lab brightness corrected values with 12 bit resolution
prog_uint16_t CIEL_8_12[] PROGMEM = {
        0,2,4,5,7,9,11,12,14,16,18,20,21,23,25,27,28,30,32,34,36,37,39,41,43,45,47,49,52,54,56,59,61,64,66,69,72,75,77,80,83,87,90,93,97,100,103,107,111,115,118,122,126,
        131,135,139,144,148,153,157,162,167,172,177,182,187,193,198,204,209,215,221,227,233,239,246,252,259,265,272,279,286,293,300,308,315,323,330,338,346,354,362,371,
        379,388,396,405,414,423,432,442,451,461,471,480,490,501,511,521,532,543,554,565,576,587,599,610,622,634,646,658,670,683,696,708,721,734,748,761,775,789,802,817,
        831,845,860,875,890,905,920,935,951,967,983,999,1015,1032,1048,1065,1082,1099,1117,1134,1152,1170,1188,1206,1225,1243,1262,1281,1301,1320,1340,1359,1379,1400,1420,
        1441,1461,1482,1504,1525,1547,1568,1590,1613,1635,1658,1681,1704,1727,1750,1774,1798,1822,1846,1871,1896,1921,1946,1971,1997,2023,2049,2075,2101,2128,2155,2182,2210,
        2237,2265,2293,2322,2350,2379,2408,2437,2467,2497,2527,2557,2587,2618,2649,2680,2712,2743,2775,2807,2840,2872,2905,2938,2972,3006,3039,3074,3108,3143,3178,3213,3248,
        3284,3320,3356,3393,3430,3467,3504,3542,3579,3617,3656,3694,3733,3773,3812,3852,3892,3932,3973,4013,4055,4095};

//lookup table for 1024 CIE lab brightness corrected values with 12 bit resolution
prog_uint16_t CIEL_10_12[] PROGMEM = {
	0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 19, 20, 20, 21,
	21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 39, 40,
	40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 54, 54, 55, 55, 56, 56, 57, 58, 58, 59, 59, 60, 61, 61, 62, 63, 63, 64,
	65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 78, 79, 80, 81, 81, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96,
	97, 98, 98, 99, 100, 101, 102, 103, 104, 105, 106, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
	130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 143, 144, 145, 146, 147, 148, 149, 150, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162, 163, 165, 166, 167,
	168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 183, 185, 186, 187, 189, 190, 191, 193, 194, 195, 197, 198, 200, 201, 202, 204, 205, 206, 208, 209, 211, 212,
	214, 215, 217, 218, 219, 221, 222, 224, 225, 227, 228, 230, 231, 233, 234, 236, 238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255, 257, 258, 260, 262, 263, 265,
	267, 268, 270, 272, 273, 275, 277, 278, 280, 282, 284, 285, 287, 289, 291, 292, 294, 296, 298, 300, 301, 303, 305, 307, 309, 311, 313, 314, 316, 318, 320, 322, 324, 326,
	328, 330, 331, 333, 335, 337, 339, 341, 343, 345, 347, 349, 351, 353, 355, 357, 359, 361, 363, 365, 368, 370, 372, 374, 376, 378, 380, 382, 384, 387, 389, 391, 393, 395,
	397, 400, 402, 404, 406, 408, 411, 413, 415, 417, 420, 422, 424, 426, 429, 431, 433, 436, 438, 440, 443, 445, 447, 450, 452, 454, 457, 459, 462, 464, 467, 469, 471, 474, 
	476, 479, 481, 484, 486, 489, 491, 494, 496, 499, 501, 504, 507, 509, 512, 514, 517, 519, 522, 525, 527, 530, 533, 535, 538, 541, 543, 546, 549, 551, 554, 557, 560, 562, 
	565, 568, 571, 573, 576, 579, 582, 585, 588, 590, 593, 596, 599, 602, 605, 608, 610, 613, 616, 619, 622, 625, 628, 631, 634, 637, 640, 643, 646, 649, 652, 655, 658, 661, 
	664, 667, 671, 674, 677, 680, 683, 686, 689, 692, 696, 699, 702, 705, 708, 712, 715, 718, 721, 725, 728, 731, 734, 738, 741, 744, 748, 751, 754, 758, 761, 764, 768, 771, 
	775, 778, 781, 785, 788, 792, 795, 799, 802, 806, 809, 813, 816, 820, 823, 827, 830, 834, 837, 841, 845, 848, 852, 856, 859, 863, 867, 870, 874, 878, 881, 885, 889, 892, 
	896, 900, 904, 908, 911, 915, 919, 923, 927, 930, 934, 938, 942, 946, 950, 954, 958, 962, 966, 970, 973, 977, 981, 985, 989, 993, 997, 1002, 1006, 1010, 1014, 1018, 1022, 
	1026, 1030, 1034, 1038, 1043, 1047, 1051, 1055, 1059, 1063, 1068, 1072, 1076, 1080, 1085, 1089, 1093, 1097, 1102, 1106, 1110, 1115, 1119, 1123, 1128, 1132, 1137, 1141, 1145, 
	1150, 1154, 1159, 1163, 1168, 1172, 1177, 1181, 1186, 1190, 1195, 1199, 1204, 1208, 1213, 1218, 1222, 1227, 1232, 1236, 1241, 1246, 1250, 1255, 1260, 1264, 1269, 1274, 1279, 
	1283, 1288, 1293, 1298, 1303, 1307, 1312, 1317, 1322, 1327, 1332, 1337, 1341, 1346, 1351, 1356, 1361, 1366, 1371, 1376, 1381, 1386, 1391, 1396, 1401, 1406, 1411, 1417, 1422, 
	1427, 1432, 1437, 1442, 1447, 1453, 1458, 1463, 1468, 1473, 1479, 1484, 1489, 1494, 1500, 1505, 1510, 1516, 1521, 1526, 1532, 1537, 1542, 1548, 1553, 1559, 1564, 1570, 1575, 
	1580, 1586, 1591, 1597, 1603, 1608, 1614, 1619, 1625, 1630, 1636, 1642, 1647, 1653, 1658, 1664, 1670, 1676, 1681, 1687, 1693, 1698, 1704, 1710, 1716, 1722, 1727, 1733, 1739, 
	1745, 1751, 1757, 1762, 1768, 1774, 1780, 1786, 1792, 1798, 1804, 1810, 1816, 1822, 1828, 1834, 1840, 1846, 1852, 1859, 1865, 1871, 1877, 1883, 1889, 1895, 1902, 1908, 1914, 
	1920, 1927, 1933, 1939, 1945, 1952, 1958, 1964, 1971, 1977, 1983, 1990, 1996, 2003, 2009, 2015, 2022, 2028, 2035, 2041, 2048, 2054, 2061, 2067, 2074, 2081, 2087, 2094, 2100, 
	2107, 2114, 2120, 2127, 2134, 2140, 2147, 2154, 2161, 2167, 2174, 2181, 2188, 2195, 2201, 2208, 2215, 2222, 2229, 2236, 2243, 2250, 2257, 2263, 2270, 2277, 2284, 2291, 2299, 
	2306, 2313, 2320, 2327, 2334, 2341, 2348, 2355, 2362, 2370, 2377, 2384, 2391, 2398, 2406, 2413, 2420, 2428, 2435, 2442, 2449, 2457, 2464, 2472, 2479, 2486, 2494, 2501, 2509, 
	2516, 2524, 2531, 2539, 2546, 2554, 2561, 2569, 2576, 2584, 2592, 2599, 2607, 2615, 2622, 2630, 2638, 2645, 2653, 2661, 2669, 2676, 2684, 2692, 2700, 2708, 2716, 2723, 2731, 
	2739, 2747, 2755, 2763, 2771, 2779, 2787, 2795, 2803, 2811, 2819, 2827, 2835, 2843, 2851, 2860, 2868, 2876, 2884, 2892, 2900, 2909, 2917, 2925, 2933, 2942, 2950, 2958, 2967, 
	2975, 2983, 2992, 3000, 3009, 3017, 3026, 3034, 3042, 3051, 3059, 3068, 3077, 3085, 3094, 3102, 3111, 3119, 3128, 3137, 3145, 3154, 3163, 3171, 3180, 3189, 3198, 3207, 3215, 
	3224, 3233, 3242, 3251, 3260, 3268, 3277, 3286, 3295, 3304, 3313, 3322, 3331, 3340, 3349, 3358, 3367, 3376, 3385, 3395, 3404, 3413, 3422, 3431, 3440, 3450, 3459, 3468, 3477, 
	3487, 3496, 3505, 3515, 3524, 3533, 3543, 3552, 3562, 3571, 3580, 3590, 3599, 3609, 3618, 3628, 3637, 3647, 3657, 3666, 3676, 3685, 3695, 3705, 3714, 3724, 3734, 3744, 3753, 
	3763, 3773, 3783, 3793, 3802, 3812, 3822, 3832, 3842, 3852, 3862, 3872, 3882, 3892, 3902, 3912, 3922, 3932, 3942, 3952, 3962, 3972, 3982, 3992, 4003, 4013, 4023, 4033, 4044, 
	4054, 4064, 4074, 4085, 4095};

/*
Set address for MCP4728 and pca9685
*/

HPRGB::HPRGB(uint8_t mcp4728ID, uint8_t pca9685ID)
{
  _mcp4728ID = mcp4728ID;
  _mcp4728_address = (MCP4728_BASE_ADDR | _mcp4728ID);
  _pca9685ID = pca9685ID;
  _pca9685_address = (PCA9685_BASE_ADDR | _pca9685ID);

//  Serial.println("HPRGB initialized");
}

/*
Wire.begin for I2C.
Get current settings from MCP4728. Set proper settings (voltage reference, gain, powerdown mode in case)
*/

void HPRGB::begin()
{

#if defined(__MK20DX128__) || defined(__MK20DX256__) //Teensy 3.0 or Teensy 3.1
	Wire.begin(I2C_MASTER, 0, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_1000);
#else
	Wire.begin();
#endif

	getStatus(); // read the values from mcp4728
	for (uint8_t ch=0;ch<=3 ;ch++ )
	{
		_intVref[ch] = 1; // set all channels to use internal voltage reference (2.048V)
		_gain[ch] = 0; // Gain = x1
		_powerDown[ch] = 0; // Regular running mode
	}
	mcp4728FastWrite(); // write initial values to input registers.
	pca9685Wake(); // wake PCA9685.
}

/*
Write current values and settings to MCP4728 EEPROM
*/

uint8_t HPRGB::eepromWrite()
{
  return mcp4728SeqWrite();
}

/*
Return current value for a channel (value / 4098)
*/

uint16_t HPRGB::getValue(uint8_t channel)
{
  return _values[channel];
}
/*
Return maximum current setting of the channel (mA)
*/

uint16_t HPRGB::getCurrent(uint8_t channel) {
  uint16_t currentSet;
  uint16_t ctrlV = _values[channel];
  if (ctrlV >= 2000) {
    currentSet = 100/senseR;
  }
  else {
    currentSet = ctrlV/senseR/20;
  }
  return (currentSet);
}
/*
Set maximum current setting of the channels (mA)
*/
void HPRGB::setCurrent(uint16_t redCurrent, uint16_t greenCurrent, uint16_t blueCurrent) {
  uint16_t values[3] = {blueCurrent, greenCurrent, redCurrent};
  for (uint8_t channel=0; channel <= 2; channel++) {
    _values[channel+1] = values[channel] * senseR * 20;
  }
  mcp4728FastWrite();
}
/*
Return operating frequency setting (KHz)
Due to the non-linear curve of freq, +-1% inaccuracy
*/

uint16_t HPRGB::getFreq() {
  uint16_t currentFreq;
  uint16_t fAdjV = _values[0];
  if (fAdjV >= 2000) {
    currentFreq = 2100;
  }
  else {
    currentFreq = ((-0.000173694) * fAdjV + 1.36185 ) * fAdjV + 62.5883; // second order polynomial fitting to the curve (freq vs DAC steps)
  }
  return currentFreq;
}
/*
set operating frequency (KHz)
Due to the non-linear curve of freq, +-1% inaccuracy
*/
void HPRGB::setFreq(uint16_t freq) {
  _values[0] =  0.0001787 * freq * freq + 0.56895 * freq + 8.0598; // second order polynomial fitting to the curve (freq vs DAC steps)
  mcp4728FastWrite();
}

/*
 set operating frequency (KHz) and maximum current setting for each channel (mA)
 Due to the non-linear curve of freq, +-1% inaccuracy
 */
void HPRGB::setFreqAndCurrent(uint16_t freq, uint16_t redCurrent, uint16_t greenCurrent, uint16_t blueCurrent) {
    uint16_t values[3] = {blueCurrent, greenCurrent, redCurrent};
    _values[0] =  0.0001787 * freq * freq + 0.56895 * freq + 8.0598; // second order polynominal fitting to the curve (freq vs DAC steps)
    for (uint8_t channel=0; channel <= 2; channel++) {
      _values[channel+1] = values[channel] * senseR * 20;
    }
    mcp4728FastWrite();
}

/*
PCA9685 Color mixing commands
*/

/*
Default 8 bit input and 12 bit sRGB CIE lab corrected PWM output
*/
void HPRGB::goToRGB(uint8_t red, uint8_t green, uint8_t blue)
{
  pca9685PWM(CIEL_8_12(blue),CIEL_8_12(green),CIEL_8_12(red));
}

/*
10 bit input and 12 bit sRGB CIE lab corrected PWM output
*/
void HPRGB::goToRGBHI(uint16_t red, uint16_t green, uint16_t blue)
{
	pca9685PWM(CIEL_10_12(blue),CIEL_10_12(green),CIEL_10_12(red));
}


/*
Direct 12bit PWM
*/
void HPRGB::goToRGB12(uint16_t red, uint16_t green, uint16_t blue)
{
  pca9685PWM(blue,green,red);
}
/*
HSV 0..255
*/
void HPRGB::goToHSB(uint8_t hue, uint8_t saturation, uint8_t value)
{
  uint8_t r, g, b;
  HSBtoRGB8(hue, saturation, value, &r, &g, &b);
  goToRGB(r, g, b);
}

void HPRGB::goToHSB10(uint16_t hue, uint8_t saturation, uint8_t value)
{
  uint8_t r, g, b;
  HSBtoRGB10(hue, saturation, value, &r, &g, &b);
  goToRGB(r, g, b);
}

/*
Set PWM Frequency 40-1000Hz, Default 200Hz
*/
void HPRGB::setPWMFrequency(uint16_t freq)
{
  uint8_t prescale = (6104/freq) - 1; // FIXME preScale =round(25000000Hz/(4096*freq))-1
  pca9685PWMPreScale(prescale);
}

//Acquire TMP421 internal temperature in Degrees Celsius
float HPRGB::getIntTemp()
{
  uint8_t hiTemp, lowTemp;
  Wire.beginTransmission(TMP421_ADDR);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(TMP421_ADDR, 2); // request 2 byte from address 1001000
  while(Wire.available())
  {
  hiTemp = Wire.read(); // Read the first octet
  lowTemp = Wire.read(); // Read the second octet
  }
  return (hiTemp + (lowTemp >> 4)*0.0625);
}

//Acquire TMP421 internal temperature in Degrees Fahrenheit
float HPRGB::getIntTempF()
{
  return (getIntTemp()*9/5 + 32);
}

//Acquire TMP421 remote temperature in Degrees Celsius
float HPRGB::getExtTemp()
{
  uint8_t hiTemp, lowTemp;
  Wire.beginTransmission(TMP421_ADDR);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.requestFrom(TMP421_ADDR, 2); // request 2 byte from address 1001000
  while(Wire.available())
  {
  hiTemp = Wire.read(); // Read the first octet
  lowTemp = Wire.read(); // Read the second octet
  }
  return (hiTemp + (lowTemp >> 4)*0.0625);
}

//Acquire TMP421 remote temperature in Degrees Fahrenheit
float HPRGB::getExtTempF()
{
  return (getExtTemp()*9/5 + 32);
}


/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/*
FastWrite : send only input register values to DAC
No command byte.
*/

uint8_t HPRGB::mcp4728FastWrite() {
  Wire.beginTransmission(_mcp4728_address);
  for (uint8_t channel=0; channel <= 3; channel++) {
    Wire.write(highByte(_values[channel]));
    Wire.write(lowByte(_values[channel]));
  }
  return Wire.endTransmission();
}
/*
MultiWrite : send all register values to DAC
update voltage reference, power down, gain, values on registers
*/
uint8_t HPRGB::mcp4728MultiWrite() {
  Wire.beginTransmission(_mcp4728_address);
  for (uint8_t channel=0; channel <= 3; channel++) {
    Wire.write(MULTIWRITE | (channel << 1)); 
    Wire.write(_intVref[channel] << 7 | _powerDown[channel] << 5 | _gain[channel] << 4 | highByte(_values[channel]));
    Wire.write(lowByte(_values[channel]));
  }
  return Wire.endTransmission();
}
/*
SingleWrite : send single register values to DAC EEPROM
update voltage reference, power down, gain, values on registers
*/
uint8_t HPRGB::mcp4728SingleWrite(uint8_t channel) {
  Wire.beginTransmission(_mcp4728_address);
  Wire.write(SINGLEWRITE | (channel << 1)); 
  Wire.write(_intVref[channel] << 7 | _powerDown[channel] << 5 | _gain[channel] << 4 | highByte(_values[channel]));
  Wire.write(lowByte(_values[channel]));
  return Wire.endTransmission();
}
/*
SequentialWrite : send four register values to DAC EEPROM
update voltage reference, power down, gain, values on registers
*/
uint8_t HPRGB::mcp4728SeqWrite() {
  Wire.beginTransmission(_mcp4728_address);
  Wire.write(SEQWRITE); 
  for (uint8_t channel=0; channel <= 3; channel++) {
    Wire.write(_intVref[channel] << 7 | _powerDown[channel] << 5 | _gain[channel] << 4 | highByte(_values[channel]));
    Wire.write(lowByte(_values[channel]));
  }
  return Wire.endTransmission();
}
/*
Get current values and settings from MCP4728 for initialization
*/
void HPRGB::getStatus()
{
  Wire.requestFrom(int(_mcp4728_address), 24);
  while(Wire.available())
  { 
    int deviceID = Wire.read();
    int hiByte = Wire.read();
    int loByte = Wire.read();

    int isEEPROM = (deviceID & 0B00001000) >> 3;
    int channel = (deviceID & 0B00110000) >> 4;
    if (isEEPROM == 1) {
      _intVrefEp[channel] = (hiByte & 0B10000000) >> 7;
      _gainEp[channel] = (hiByte & 0B00010000) >> 4;
      _powerDownEp[channel] = (hiByte & 0B01100000) >> 5;
      _valuesEp[channel] = ((hiByte & 0B00001111)<<8)+loByte;
    }
    else {
      _intVref[channel] = (hiByte & 0B10000000) >> 7;
      _gain[channel] = (hiByte & 0B00010000) >> 4;
      _powerDown[channel] = (hiByte & 0B01100000) >> 5;
      _values[channel] = ((hiByte & 0B00001111)<<8)+loByte;
    }
  }
}
/*
Wake PCA9685 oscillator and enable auto increment
*/
void HPRGB::pca9685Wake()
{
  Wire.beginTransmission(_pca9685_address);
  Wire.write((byte)0x00);
  Wire.write(0B00100001);
  Wire.endTransmission();
}
/*
PCA9685 PWM frequency pre-scale
*/
void HPRGB::pca9685PWMPreScale(uint8_t prescale)
{
  Wire.beginTransmission(_pca9685_address);
  Wire.write(0xfe);
  Wire.write(prescale);
  Wire.endTransmission();
}

/*
Send PWM only STOP to channel, PWM start at 0. Non-incremental. Slow in loop
*/
void HPRGB::pca9685PWMSingle(uint8_t channel, uint16_t value)
{
  channel = (channel * 4) + 8;
  Wire.beginTransmission(_pca9685_address);
  Wire.write(channel); 
  Wire.write(lowByte(value));
  Wire.endTransmission();
  Wire.beginTransmission(_pca9685_address);
  Wire.write(channel + 1);
  Wire.write(highByte(value));
  Wire.endTransmission();
}
/*
Send PWM only OFF to channel, PWM start at 0. auto-incremental.
40% faster than single write and channel changes together.
*/
void HPRGB::pca9685PWM(uint16_t value0,uint16_t value1,uint16_t value2)
{
  Wire.beginTransmission(_pca9685_address);
  Wire.write((byte)0x06);				// start from channel 0 ON
  Wire.write((byte)0x00);				// set all ON time to 0
  Wire.write((byte)0x00);
  Wire.write(lowByte(value0));	// set OFF according to value
  Wire.write(highByte(value0));
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write(lowByte(value1));
  Wire.write(highByte(value1));  
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.write(lowByte(value2));
  Wire.write(highByte(value2));    
  Wire.endTransmission();
}

uint16_t HPRGB::pca9685GetPWM(uint8_t channel) 
{
  channel = (channel * 4) + 6;
  Wire.beginTransmission(_pca9685_address);
  Wire.write(channel);
  Wire.endTransmission();
  Wire.requestFrom(_pca9685_address, (size_t) 4);
  while(Wire.available()){
    uint8_t onLow = Wire.read();
    uint8_t onHi = Wire.read();
    uint8_t offLow = Wire.read();
    uint8_t offHi = Wire.read();
    return (word((offHi & 0B00001111), offLow) - word((onHi & 0B00001111), onLow));
  }
}
/*
HSV to RGB adopted from cyzRGB and adafruit
http://code.google.com/p/codalyze/source/browse/cyz_rgb/trunk/cyz/color.c
https://github.com/adafruit/RGB-matrix-Panel/blob/master/examples/plasma/plasma.pde
*/
void HPRGB::HSBtoRGB8(uint8_t h, uint8_t s, uint8_t v, uint8_t* r, uint8_t* g, uint8_t* b)
{

        if ( s == 0 )
        {
                *b = *g = *r = v;
        }
        else
        {       
                
                uint8_t i = (h*6)/256;
                uint16_t f = (h*6) % 256;

                uint16_t p = (v * (255 - s)) / 256;
                uint16_t q = (v * (255 - (s * f)/256)) / 256;
                uint16_t t = (v * (255 - (s * (255 - f))/256)) / 256;

                if      ( i == 0 ) { *r = v ; *g = t ; *b = p; } // 0   deg (r)   to 60  deg (r+g)
                else if ( i == 1 ) { *r = q ; *g = v ; *b = p; } // 60  deg (r+g) to 120 deg (g)
                else if ( i == 2 ) { *r = p ; *g = v ; *b = t; } // 120 deg (g)   to 180 deg (g+b)
                else if ( i == 3 ) { *r = p ; *g = q ; *b = v; } // 180 deg (g+b) to 240 deg (b)
                else if ( i == 4 ) { *r = t ; *g = p ; *b = v; } // 240 deg (b)   to 300 deg (b+r)
                else               { *r = v ; *g = p ; *b = q; } // 300 deg (b+r) to 0   deg (r)
        }

}

// original code from http://www.johngineer.com/blog/?p=1022
//optimizations to routine on: http://www.adafruit.com/blog/2012/03/14/constant-brightness-hsb-to-rgb-algorithm/

void HPRGB::HSBtoRGB10(uint16_t index, uint8_t sat, uint8_t bright, uint8_t* r, uint8_t* g, uint8_t *b  )
{
	uint16_t r_temp, g_temp, b_temp;
	uint8_t index_mod;
	uint8_t inverse_sat = (sat ^ 255);

	index = index % 768;
	index_mod = index % 256;

	if (index < 256)
	{
		r_temp = index_mod ^ 255;
		g_temp = index_mod;
		b_temp = 0;
	}

	else if (index < 512)
	{
		r_temp = 0;
		g_temp = index_mod ^ 255;
		b_temp = index_mod;
	}

	else if ( index < 768)
	{
		r_temp = index_mod;
		g_temp = 0;
		b_temp = index_mod ^ 255;
	}

	else
	{
		g_temp = 0;
		b_temp = 0;
	}

	r_temp = ((r_temp * sat) / 255) + inverse_sat;
	g_temp = ((g_temp * sat) / 255) + inverse_sat;
	b_temp = ((b_temp * sat) / 255) + inverse_sat;

	r_temp = (r_temp * bright) / 255;
	g_temp = (g_temp * bright) / 255;
	b_temp = (b_temp * bright) / 255;

	*r = (uint8_t)r_temp;
	*g = (uint8_t)g_temp;
	*b = (uint8_t)b_temp;
}

void HPRGB::HSBtoRGB10_2(uint16_t index, uint8_t sat, uint8_t bright, uint8_t* r, uint8_t* g, uint8_t *b  ) {
//	uint8_t temp[5], n = (index >> 8) % 3;
	uint8_t temp[5], n = (index >> 8);
	// %3 not needed if input is constrained, but may be useful for color cycling and/or if modulo constant is fast
	uint8_t x = ((((index & 255) * sat) >> 8) * bright) >> 8;
	// shifts may be added for added speed and precision at the end if fast 32 bit calculation is available
	uint8_t s = (( 256 - sat ) * bright) >> 8;
	temp[0] = temp[3] = s;
	temp[1] = temp[4] = x + s;
	temp[2] = bright - x;
	*r = (uint8_t)temp[n + 2];
	*g = (uint8_t)temp[n + 1];
	*r = (uint8_t)temp[n ];
}


void HPRGB::HSBtoRGB10_1(uint16_t index, uint8_t sat, uint8_t bright, uint8_t* r, uint8_t* g, uint8_t *b  ) {
//	uint8_t temp[5], n = (index >> 8) % 3;
	uint8_t temp[5], n = (index >> 8);
	temp[0] = temp[3] = (uint8_t)(( (sat ^ 255) * bright) / 255);
	temp[1] = temp[4] = (uint8_t)((((( (index & 255) * sat) / 255) + (sat ^ 255)) * bright) / 255);
	temp[2] = (uint8_t)(((((((index & 255) ^ 255) * sat) / 255) + (sat ^ 255)) * bright) / 255);
	*r = temp[n + 2];
	*g = temp[n + 1];
	*r = temp[n ];
	}
