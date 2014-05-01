#ifndef _SERIAL_MODEM_PLATFORM__H_
#define _SERIAL_MODEM_PLATFORM__H_

//  Default hardware specific method pointers to null space to declare prototypes for compile time
#define hw_setPinMode(...)
#define hw_digitalWrite(...)
#define hw_HIGH 0x1
#define hw_LOW  0x0
#define hw_INPUT 0x0
#define hw_OUTPUT 0x1

//  Default platform specific method pointers to null space to declare prototypes for compile time
#define plt_millis() 0
#define plt_delay(ms)

#define SMSerialInterfaceClass void *

// PROGMEM strings support
#define PROGMEM
#define PMEM_STR(str) str
#define PMEM_STR_REF(string_literal) string_literal
#define PMEM_STR_REF_ARRAY(pmem_char_array) (&pmem_char_array[0])

#include "platforms/_arduino.h"

#endif // _SERIAL_MODEM_PLATFORM__H_
