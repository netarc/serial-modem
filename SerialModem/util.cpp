#include "SerialModem.h"

namespace Modem {
  __PROGMEM_STR::__PROGMEM_STR(PROGMEM_PTR ptr) {
    #ifdef PGM_P
      _buffer = (char *)malloc(strlen_P(ptr) + 1);
      strcpy_P(_buffer, ptr);
    #else
      #error "unhandled PROGMEM_PTR"
    #endif
  }

  __PROGMEM_STR::~__PROGMEM_STR() {
    if (_buffer)
      free(_buffer);
  }


  char *cgb_sprintf(const char *format, ...) {
    va_list args;
    char *buffer = g_circularBuffer->resetLeft();
    va_start(args, format);
    int res = vsnprintf(buffer, SERIAL_MODEM_SHARED_BUFFER, format, args);
    va_end(args);
    return res >= SERIAL_MODEM_SHARED_BUFFER ? NULL : buffer;
  }



  CircularBuffer::CircularBuffer(char *ref, uint16_t size) : _data(ref),
                                                             _size(size)
  {
    _ptrStart = _ptrEnd = _data;
  }

  CircularBuffer::~CircularBuffer() {
  }

  char * CircularBuffer::resetLeft() {
    _ptrStart = _ptrEnd = _data;
    return _ptrStart;
  }

  void CircularBuffer::appendCircular(char ch) {
    *_ptrEnd = ch;

    if (++_ptrEnd > _data + _size)
      _ptrEnd = _data;
    if (_ptrEnd == _ptrStart) {
      if (++_ptrStart > _data + _size)
        _ptrStart = _data;
    }
  }

  char * CircularBuffer::strstr(char *str) {
    char c;
    size_t len;

    c = *str++;
    if (!c)
        return (char *) _ptrStart; // Trivial empty string case

    len = strlen(str);
    do {
        char sc;

        do {
            sc = *_ptrStart++;
            if (!sc)
                return (char *) 0;
        } while (sc != c);
    } while (strncmp(_ptrStart, str, len) != 0);

    return (char *) (_ptrStart - 1);


    return NULL;
  }

}
