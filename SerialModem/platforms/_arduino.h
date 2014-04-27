#ifndef _SERIAL_MODEM_PLATFORM_ARDUINO__H_
#define _SERIAL_MODEM_PLATFORM_ARDUINO__H_


// Core library for code-sense
#if defined(MAPLE_IDE) // [untested] Maple specific
#include "WProgram.h"
#define ARDUINO_PLATFORM
#elif defined(MPIDE) // [untested] chipKIT specific
#include "WProgram.h"
#define ARDUINO_PLATFORM
#elif defined(DIGISPARK) // [untested] Digispark specific
#include "Arduino.h"
#define ARDUINO_PLATFORM
#elif defined(ENERGIA) // [untested] LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // [untested] Teensy specific
#include "WProgram.h"
#define ARDUINO_PLATFORM
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#define ARDUINO_PLATFORM
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#define ARDUINO_PLATFORM
#endif

#ifdef ARDUINO_PLATFORM
  #define plt_setPinMode(...) pinMode(__VA_ARGS__)
  #define plt_digitalWrite(...) digitalWrite(__VA_ARGS__)
  #define plt_millis() millis()
  #define plt_delay(ms) delay(ms)

  #define SMSerialInterfaceClass HardwareSerial *

  // Make use of PROGMEM strings
  #ifdef PGM_P
    #define PROGMEM_RAW(str) ((PGM_P)F(str))
    #define PROGMEM_PTR PGM_P
    #define PROGMEM_STR(str) Modem::__PROGMEM_STR(PSTR(str))
    #define LSTRING(str) Modem::__PROGMEM_STR(str)
    #define LSTRING_PTR Modem::__PROGMEM_STR
  #endif

  static int _hw_printf(const char *format, ...) {
    char buf[128];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, 128-1, format, ap);
    for(char *p = &buf[0]; *p; p++) {
      if(*p == '\n')
        Serial.write('\r');
      Serial.write(*p);
    }
    va_end(ap);
    return strlen(buf);
  }
#endif

#endif // _SERIAL_MODEM_PLATFORM_ARDUINO__H_
