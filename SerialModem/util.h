#ifndef _SERIAL_MODEM_VARS__H_
#define _SERIAL_MODEM_VARS__H_

#ifndef MAX
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#endif

// namespace Modem {
  static const int SUCCESS = 1;
  static const int NO_RESPONSE = 0;
  static const int ERROR = -1;
  static const int FAILURE = -2;


  const char ESC_ETX    = 0x03;  // Ends socket connection
  const char ESC_DLE    = 0x10;  // Escapes ETX and DLE within Payload
  const char ESC_CR     = 0x0D;
  const char ESC_NL     = 0x0A;
  const char ESC_CTRL_Z = 0x1A;

  // Utility method that does a sprintf and stores the result in a shared buffer
  char *cgb_sprintf(const char *format, ...);

  /**
   * PMemString is a string wrapper used for PROGMEM strings to bring them into the SRAM heap temporarily
   */
  class __PMemStringRef;
  class PMemString {
  public:
    PMemString(const char *str);
    explicit PMemString(const __PMemStringRef *ptr);
    ~PMemString(void);

    operator const char*() { return _buffer; }
    operator char*() { return _buffer; }

  protected:
    char *_buffer;
  };



  /**
   * CircularBuffer is used when we are reading data from the modem and parsing for responses
   */
  class CircularBuffer {
  public:
    CircularBuffer(char *ref, uint16_t size);
    ~CircularBuffer();

    void appendCircular(char ch);
    char * substring(char *substr, char escape=0);
    char * realignLeft();
    char * resetLeft();

  // private:
    char *_data;
    char *_dataEnd;
    char *_ptrStart;
    char *_ptrEnd;
  };
// }

extern CircularBuffer *g_circularBuffer;

#endif
