HPRGB2
======

Arduino Library for High Power RGB LED shield ledshield.wordpress.com

This supercedes the HPRGB_Shield_V2_Library-v1.2.1.zip library available on Google Code.
https://code.google.com/p/neuroelec/downloads/detail?name=HPRGB_Shield_V2_Library-v1.2.1.zip&can=2&q=


Changes fom V1.2.1
1.  Adds higher resolutoin goToRGBHI routine. This uses 1024 CIE lab brightness corrected 12-bit values
    for slow, smooth dimming at low brighness values. The original goToRGB function uses only 256 12-bit values.
2.  Adds a higher resolution goToHSB10 routing which uses 768 vs. 255 values for Hue resulting in much smother
    color fading at slow fading speeds compared to the goToHSB routine.
