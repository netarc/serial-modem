#ifndef _SERIAL_MODEM_CLIENT__H_
#define _SERIAL_MODEM_CLIENT__H_

#include <Client.h>

class SerialModemClient : public Client {
public:
  SerialModemClient();

  virtual int connect(IPAddress address, uint16_t port);
  virtual int connect(const char *address, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t*, size_t);
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();
  virtual int read(uint8_t*, size_t);
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool() {
    return _connected;
  }

private:
  void endTransmit();

  bool _connected;
};

#endif // _SERIAL_MODEM_CLIENT__H_
