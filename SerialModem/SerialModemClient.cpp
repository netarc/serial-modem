#include "SerialModem.h"
#include "SerialModemClient.h"
#include "platforms/platform.h"


SerialModemClient::SerialModemClient() : _connected(false) {
}

int SerialModemClient::connect(IPAddress ip, uint16_t port) {
  char address[18];
  sprintf(address, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
  return connect(address, port);
}

int SerialModemClient::connect(const char *address, uint16_t port) {
  if (!SerialModem.driver()->connectData())
    return 0;

  _connected = SerialModem.driver()->openSocketTCP(address, port);
  return _connected;
}

size_t SerialModemClient::write(uint8_t ch) {
  return SerialModem.driver()->writeSocket(&ch, 1);
}

size_t SerialModemClient::write(const uint8_t *bytes, size_t size) {
  return SerialModem.driver()->writeSocket(bytes, size);
}

int SerialModemClient::available() {
  SerialModem.driver()->onSocketRead();
  return SerialModem._hardware_serial->available();
}

int SerialModemClient::read() {
  SerialModem.driver()->onSocketRead();
  return SerialModem._hardware_serial->read();
}

int SerialModemClient::peek() {
  SerialModem.driver()->onSocketRead();
  return SerialModem._hardware_serial->peek();
}

void SerialModemClient::flush() {
  SerialModem.driver()->onSocketRead();
  return SerialModem._hardware_serial->flush();
}

int SerialModemClient::read(uint8_t *buffer, size_t size) {
  // return SerialModem._hardware_serial->read(buffer, size);
  return 0;
}

void SerialModemClient::stop() {
  SerialModem.driver()->closeSocket();
}

uint8_t SerialModemClient::connected() {
  return _connected;
}