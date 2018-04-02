# Iron Timer
This is a simple Arduino project that was written to automatically turn off a
commercial iron (with no auto-off functionality) after a certain amount of time.

It shows the amount of time remaining on a seven-segment display. The decimal points on the display
are alternated each time the main loop runs to serve as a heartbeat to indicate the program is
still running properly.

There is a timer-based ISR that is used to drive the display. The reset
button triggers another ISR that resets the time left back to the maximum value.

Components used:
* Metro Mini ([Adafruit Link](https://www.adafruit.com/product/2590)) (this should work with all Arduinos including the Uno - The Arduino Mega and some others may need minor tweaks to the timer-based ISR)
* [IoT Power Relay](https://dlidirect.com/products/iot-power-relay) ([Adafruit Link](https://www.adafruit.com/product/2935))

While it is possible to build the AC components yourself, I highly recommend buying a pre-made
device such as the the [IoT Power Relay](https://dlidirect.com/products/iot-power-relay).

Improvements to be made:
* Make reset switch less sensitive - sometimes power fluctuations cause it to trigger.
  I have somewhat fixed this by using a better power supply and adding a capacitor
  in parallel, but I think thewre is likely a simpler more fool-proof way.

###### I have no affiliation with Adafruit and these links are not referral links. I like all of the libraries and educational material they release and like to support them where I can.