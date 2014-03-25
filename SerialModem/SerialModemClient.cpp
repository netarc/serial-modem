#include "SerialModem.h"
#include "SerialModemClient.h"
#include "platforms/platform.h"

using namespace Modem;

SerialModemClient::SerialModemClient() : _transmitMode(false),
                                         _connected(false) {

}

int SerialModemClient::connect(IPAddress ip, uint16_t port) {
  char address[18];
  sprintf(address, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
  return connect(address, port);
}

int SerialModemClient::connect(const char *address, uint16_t port) {
  _transmitMode = true;
  char *response = SerialModem.sendCommand(cgb_sprintf(PROGMEM_STR("AT+CHTTPACT=\"%s\",%d"), address, port), 30000, ESC_CR, "REQUEST\x0D");
  if (!response)
    return 0;

  _connected = !!strstr(response, "+CHTTPACT: REQUEST");
  return _connected;
}

void SerialModemClient::endTransmit() {
  if (!_transmitMode)
    return;

  _transmitMode = false;
  SerialModem.sendCommand("", 15000, ESC_CTRL_Z);

  /*
  return;
  char response[100];
  do {
    bool result = SerialModem.readLine(response, 100, 1000);
    if (result) {
      if (strstr(response, "+CHTTPACT") != NULL) {
        char *tkn = strtok(response, " ,");
        tkn = strtok(NULL, " ,");
        Serial.print("token: ");
        Serial.println(tkn);

        if (strcmp(tkn, "DATA") == 0) {
          char data[512];

          tkn = strtok(NULL, " ,");
          Serial.print("size token: ");
          Serial.println(tkn);
          uint32_t data_size = atoi(tkn);
          uint32_t bytes_read;
          uint32_t offset = 1;

          SerialModem._hardware_serial->setTimeout(15000);
          bytes_read = SerialModem._hardware_serial->readBytes(data, 1);
          if (data[0] == 10 || data[0] == 13)
            offset = 0;

          bytes_read = SerialModem._hardware_serial->readBytes(&data[offset], data_size);

          if (bytes_read == data_size) {
            Serial.println(data);
            Serial.println(">> download finished");
            return;
          } else {
            Serial.println(data);
            Serial.println(">> download timeout");
          }
        }
      }
    }
  } while(true);
  */
}

size_t SerialModemClient::write(uint8_t ch) {
  return SerialModem.writeBytes(&ch, 1);
}

size_t SerialModemClient::write(const uint8_t *bytes, size_t size) {
  return SerialModem.writeBytes(bytes, size);
}

int SerialModemClient::available() {
  endTransmit();
  return SerialModem._hardware_serial->available();
}

int SerialModemClient::read() {
  endTransmit();
  return SerialModem._hardware_serial->read();
}

int SerialModemClient::peek() {
  endTransmit();
  return SerialModem._hardware_serial->peek();
}

void SerialModemClient::flush() {
  return SerialModem._hardware_serial->flush();
}

int SerialModemClient::read(uint8_t *buffer, size_t size) {
  // endTransmit();
  // return SerialModem._hardware_serial->read(buffer, size);
  return 0;
}

void SerialModemClient::stop() {
  if (!_transmitMode)
    SerialModem.sendCommand("", 1000, ESC_CTRL_Z);

  _connected = false;
  _transmitMode = false;
}

uint8_t SerialModemClient::connected() {
  return _connected;
}