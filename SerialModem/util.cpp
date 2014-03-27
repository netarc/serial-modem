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



  CircularBuffer::CircularBuffer(char *ref, uint16_t size) : _data(ref)
  {
    _ptrStart = _ptrEnd = _data;
    _dataEnd = _data + size;
  }

  CircularBuffer::~CircularBuffer() {
  }

  char * CircularBuffer::resetLeft() {
    _ptrStart = _ptrEnd = _data;
    return _ptrStart;
  }

  // Re-order our buffer so our ptrStart starts at data start so we can have a string that doesn't wrap the end
  char * CircularBuffer::realignLeft() {
    *_ptrEnd = 0;
    if (_ptrStart == _data)
      return _ptrStart;

    char tmp;
    char *mark = _data;
    // stage 1 - with our mark starting at the front walk our mark and the ptrStart swapping their values until ptrStart wraps to the front
    while (_ptrStart != _data) {
      tmp = *mark;
      *mark++ = *_ptrStart++;

      if (_ptrStart > _dataEnd)
        _ptrStart = _data;
    }
    // stage 2 - walk the mark to the end swapping values until mark reaches the end
    while (mark < _dataEnd) {
      tmp = *mark;
      *mark++ = *_dataEnd;
      *_dataEnd = tmp;
    }
    _ptrEnd = _dataEnd;
    return _ptrStart;
  }

  void CircularBuffer::appendCircular(char ch) {
    *_ptrEnd = ch;

    if (++_ptrEnd > _dataEnd)
      _ptrEnd = _data;
    if (_ptrEnd == _ptrStart) {
      if (++_ptrStart > _dataEnd)
        _ptrStart = _data;
    }
  }

  char * CircularBuffer::substring(char *substr, char escape) {
    // mark is our first character of the substr
    char mark = *substr++;
    if (!mark)
      return NULL;

    char *source = _ptrStart;
    char *a,*b;
    // level 1 - scan for the mark in our source
    while (source != _ptrEnd) {
      // circular buffer bounds check
      if (source > _dataEnd) {
        source = _data;
        if (source == _ptrEnd)
          break;
      }

      if (*source++ != mark)
        continue;

      // level 2 - starting after the 1st chars, continue checking the match
      a = source;
      b = substr;
      while (1) {
        if (*b == 0 && escape == 0)
          return source-1;

        // circular buffer bounds check
        if (a > _dataEnd)
          a = _data;
        if (a == _ptrEnd)
          break;

        // we have reached the end of our substr so check our escape character if provided
        if (*b == 0) {
          if (*a == escape)
            return source-1;
          else
            break;
        }

        if (*a++ != *b++)
          break;
      }
    }
    return NULL;
  }
}
