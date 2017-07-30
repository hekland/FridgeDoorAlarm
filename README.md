# README #

### What is this repository for? ###

**Low-power circuit-break detector using the ATtiny85, to ensure the fridge and freezer doors are closed**

* Version 1.0 (2017-07-30): Initial release

This program will, every eight seconds, check the two circuits 
to see whether they're open or closed. This is typically the fridge
and the freezer door. If the fridge door is open for more than three minutes
the piezo-buzzer will beep three times followed by four seconds pause, and continue until it's closed. 
For the freezer door, nine beeps will be produced.
In addition, an LED will blink briefly every 60 seconds to show it's alive.

The ATtiny85 is using a 1MHz internal clock, and will run for 3.8 years on 
a 220mAh CR2032 coin cell battery. If the LED is omitted, over 5 years runtime
is expected.

### How do I get set up? ###

**What you need**

* [ATtiny support for Arduino IDE](https://github.com/damellis/attiny)
* [Low-power Library for Arduino with ATtiny85-support](https://github.com/sej7278/LowPower)
* An Arduino Uno for programming the ATtiny85.
* Some hardware as described in the Fritzing file.

**How to program the ATtiny85**

* [Setup your Uno as programmer](http://highlowtech.org/?p=1706)
* [Burn bootloader on the ATtiny85 using the 1MHz internal clock, and install sketch](http://highlowtech.org/?p=1695)

### Contribution guidelines ###


### Who do I talk to? ###

Written by: Fredrik Hekland