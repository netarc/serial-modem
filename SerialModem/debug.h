#ifndef _SERIAL_MODEM_DEBUG__H_
#define _SERIAL_MODEM_DEBUG__H_

#ifdef DEBUG
  #define DLog(format, ...) _hw_printf((format), ##__VA_ARGS__)
#else
  #define DLog(format, ...)
#endif

#endif // _SERIAL_MODEM_DEBUG__H_
