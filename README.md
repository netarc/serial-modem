Introduction
==================

SerialModem is a c++ support library for WCDMA/HSPA modems over a serial interface. Currently supported modems are:
* Cooking Hacks SIM5218E Shield (INTERFACE_SIM5218)
* MultiTech MTSMC-H5-IP Shield  (INTERFACE_MTSMC_H5)

SerialModem was initially designed for [Arduino](http://www.arduino.cc/) based devices using the [Arduino 3g GPRS/GSM/GPS Shield](http://www.cooking-hacks.com/documentation/tutorials/arduino-3g-gprs-gsm-gps); but has since evolved to support additional modems.

**NOTE**: Unless stated otherwise, the following instructions are specific to [Arduino Uno](http://arduino.cc/en/Main/arduinoBoardUno) board. If you are using other boards, the exact steps may vary.

How to Install the library
==========================

1. Clone the latest source or download the latest ZIP.
2. Open the Arduino IDE, click `Sketch->Import Library...->Add Library...`, then navigate to `SerialModem/SerialModem`
3. Now you can find SerialModem examples under `File->Examples->SerialModem`
3. Enjoy coding!

Using the SerialModem library
=========================

You will need to include the libaries main header to gives access to the SerialModem object using the following code:

```cpp
#include "SerialModem.h"
```

To setup the modem you will need to specify what modem you are using via the ```setHardwareInterface``` method as well as set the correct Network APN using the ```setAPN`` method. This is typically done in the microcontroller startup phase using the following code:

```cpp
void setup() {
  ...
  // Specify the Modem you are using
  SerialModem.setHardwareInterface(INTERFACE_SIM5218);
  SerialModem.begin(&Serial, 115200);
  while (!SerialModem.ready()) {
    Serial.println("waiting for modem ready");
    delay(2000);
  }

  while (SerialModem.getNetworkStatus() < NETWORK_STATUS_ROAMING) {
    Serial.println("waiting for network registration");
    delay(2000);
  }

  // Specify the APN which the SIM can connect to
  while (!SerialModem.setAPN("m2m.com.attz")) {
    Serial.println("setting APN");
    delay(2000);
  }
  ...
}
```

LICENSE
=======

This library is released under the MIT license. See [`LICENSE`](LICENSE) for the terms.