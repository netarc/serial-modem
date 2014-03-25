#include "SerialModem.h"


  #ifdef PGM_P
// #error "wut"
    // #define PROGMEM_STR(str) progmem_read_str((PGM_P)F(str))
    // static inline char * progmem_read_str(PGM_P p) {
    //   return strcpy_P(g_gapBuffer->reserveRight(strlen_P(p)), p);
    // }
  #endif

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
    uint16_t bytesAvailable = g_gapBuffer->sizeAvailableForLeft();
    va_start(args, format);
    int res = vsnprintf(g_gapBuffer->left(), bytesAvailable, format, args);
    va_end(args);
    return res >= bytesAvailable ? NULL : g_gapBuffer->left();
  }


  #define CGB_MODE_GAP      1
  #define CGB_MODE_CIRCULAR 2

  CircularGapBuffer::CircularGapBuffer(char *ref, uint16_t size) : _data(ref),
                                                                   _size(size),
                                                                   _mode(0)
  {
  }

  CircularGapBuffer::~CircularGapBuffer() {
  }

  void CircularGapBuffer::beginGap() {
    if (_mode == CGB_MODE_GAP)
      return;
    _mode = CGB_MODE_GAP;
    _gapRight = 0;
  }

  uint16_t CircularGapBuffer::sizeAvailableForLeft() {
    beginGap();
    return _size - _gapRight;
  }

  char * CircularGapBuffer::reserveRight(uint16_t size) {
    beginGap();
    _gapRight = size + 1; // +1 for end string termination
    return _data + (_size - _gapRight);
  }


  void CircularGapBuffer::beginCircular() {
    if (_mode == CGB_MODE_CIRCULAR)
      return;
    _mode = CGB_MODE_CIRCULAR;
    _ptrStart = _ptrEnd = _data;
  }

  void CircularGapBuffer::appendCircular(char ch) {
    beginCircular();
    *_ptrEnd = ch;

    if (++_ptrEnd > _data + _size)
      _ptrEnd = _data;
    if (_ptrEnd == _ptrStart) {
      if (++_ptrStart > _data + _size)
        _ptrStart = _data;
    }
  }

  char * CircularGapBuffer::strstr(char *str) {
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
