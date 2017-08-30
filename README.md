RGB LED clock
=============
Simple clock for ring with 60 RGB LED like Adafruit NeoPixel, my ring got from aliexpress has SK6812 GRBW type.

- https://www.adafruit.com/product/2874

This doens't support RTC and you have to set the time every time clock starts.


Library
-------
This requires FAB_LED library.
- https://github.com/sonyhome/FAB_LED

This includes Makefile for Arduino-Makefile.
- https://github.com/sudar/Arduino-Makefile




Serial console interafce
------------------------
You can set time and see some variables in serial console. For example, you can set the time to 16:57 with `T1657`.

```
? for help.
Shhmm   - set time
V       - show variables
T       - show time
```


TODO
----
- Fancy color scheme
