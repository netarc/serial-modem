#ifndef SERIAL_MODEM_DRIVERS_SIM5218_H
#define SERIAL_MODEM_DRIVERS_SIM5218_H

// SIM5218 Serial Command Manual
// http://www.cooking-hacks.com/skin/frontend/default/cooking/pdf/SIM5218_AT_command_manual.pdf

using namespace Modem;

class DriverSIM5218 : public BaseDriver {
public:
  DriverSIM5218() {}

  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Software
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool setAPN(const char *apn) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CGSOCKCONT=1,\"IP\",\"%s\""), apn)) == Modem::SUCCESS;
  }

  virtual bool gpsEnable() {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CGPS=1,2"))) == Modem::SUCCESS;
  }

  virtual bool gpsSetServer(char *server, uint16_t port) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CGPSURL=\"%s:%d\""), server, port)) == Modem::SUCCESS;
  }

  virtual bool connectData() {
    if (_connectedData)
      return true;

    _connectedData = true;
    return _connectedData;
  }

  virtual bool openSocketTCP(const char *address, uint16_t port) {
    if (!_connectedData || _connectedSocket)
      return false;

    if (SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+NETOPEN=\"TCP\",%d"), port), 30000) != Modem::SUCCESS)
      return false;
    if (SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+TCPCONNECT=\"%s\",%d"), address, port), 30000) != Modem::SUCCESS) {
      SerialModem.sendBasicCommand(PROGMEM_STR("AT+NETCLOSE"), 30000) == Modem::SUCCESS;
      return false;
    }
    return _connectedSocket = true;
  }

  virtual bool closeSocket() {
    if (!_connectedSocket)
      return false;
    _connectedSocket = SerialModem.sendBasicCommand(PROGMEM_STR("AT+NETCLOSE"), 30000) == Modem::SUCCESS;
  }

  virtual int writeSocket(const uint8_t *bytes, size_t size) {
    if (!_connectedData || !_connectedSocket)
      return 0;


    SerialModem.writeCommand(cgb_sprintf(PROGMEM_STR("AT+TCPWRITE=%d"), size));
    SerialModem._hardware_serial->write(ESC_CR);
    if (!SerialModem.getResponse(30000, ">"))
      return 0;
    SerialModem.writeBytes(bytes, size);
    return SerialModem.parseBasicResponse(SerialModem.getResponse(30000)) == Modem::SUCCESS ? size : 0;
  }

  virtual bool connectVoice() {
    return false;
  }
};

#define DRIVER_SIM5218 (new DriverSIM5218())

#endif // SERIAL_MODEM_DRIVERS_SIM5218_H