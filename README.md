# AVR-Time
This is a basic timer library for the AVR

This code was written with the intention of being used with an external crystal oscillator. 
My current setup is with a 16MHz external xtl attached to an atmega328p.

The fuse settings for my setup can be put in place using 
avrdude -c usbtiny -p m328p -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
To revert these settings use
avrdude -c usbtiny -p m328p -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

Further settings can be calculated using 
http://www.engbedded.com/fusecalc/
Explainations for the different setting fuses can be found at
http://www.ladyada.net/learn/avr/fuses.html
