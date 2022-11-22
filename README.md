# DDS-for-Icom-IC-2N
Digital Display System for old legendary Portable radio Icom IC-2N (IC-2AT, IC2AE)

This is Barebone but functional Digital Display and Control system for portable radio Icom IC-2N (IC-2AT, IC2AE)
The Icom IC-2N portable radios still remain popular in use in Indonesia.
Although this mechanic radio, it's proven robust as daily use.
Many problem the mechanical rotary thumbwheel drive is defective, but internal part of radio is still functional.
Arduino is pupular and easy to use for creating new display and control for Portable radio Icom IC-2N (IC-2AT, IC2AE).
For display, we use cheap LCD 16X2 hd44780, one rotary encoder, and for key buttons configured as keypad.
The Display shown 144.00MHz, VFO/MEM indicator, Simplex-Duplex mode, save Memory function
other feature, saving duplex memory, and last frequency autosave.


Depedencies:
Wire.h library
LiquidCrystal_i2c.h library
Encoder.h library by PJRC.com
EEPROM.h library

Usable and tested with Arduino uno, also works with Nano, and Promini with little modifications.

Displayed S-meter still dummy display, i already given the working basic, make efforts by yourself!
