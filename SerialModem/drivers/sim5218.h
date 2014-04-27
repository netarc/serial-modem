#ifndef SERIAL_MODEM_DRIVERS_SIM5218_H
#define SERIAL_MODEM_DRIVERS_SIM5218_H

// SIM5218 Serial Command Manual
// http://www.cooking-hacks.com/skin/frontend/default/cooking/pdf/SIM5218_AT_command_manual.pdf

#define SERIAL_MODEM_SIM5218_SOCKET_BUFFER 128

// static char PROGMEM setAPN[]  = {"AT+CGSOCKCONT"};

class DriverSIM5218 : public BaseDriver {
public:
  DriverSIM5218() {
    _sendBufferWrite = 0;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Software
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool setAPN(const char *apn) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CGSOCKCONT=1,\"IP\",\"%s\""), apn)) == Modem::SUCCESS;
  }

  virtual bool gpsEnable() {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CGPS=1,2"))) == Modem::SUCCESS;
  }

  virtual bool gpsSetServer(char *server, uint16_t port) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CGPSURL=\"%s:%d\""), server, port)) == Modem::SUCCESS;
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

    if (SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+NETOPEN=\"TCP\",%d"), port), 30000) != Modem::SUCCESS)
      return false;
    if (SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+TCPCONNECT=\"%s\",%d"), address, port), 30000) != Modem::SUCCESS) {
      SerialModem.sendBasicCommand(PMEM_STR("AT+NETCLOSE"), 30000) == Modem::SUCCESS;
      return false;
    }
    return _connectedSocket = true;
  }

  virtual bool closeSocket() {
    if (!_connectedSocket)
      return false;
    writeBufferToSocket(true);
    _connectedSocket = SerialModem.sendBasicCommand(PMEM_STR("AT+NETCLOSE"), 30000) != Modem::SUCCESS;
  }

  virtual int writeSocket(const uint8_t *bytes, size_t size) {
    if (!_connectedData || !_connectedSocket)
      return 0;

    size_t written=0;
    while (written++ < size) {
      _sendBuffer[_sendBufferWrite++] = *(bytes++);
      if (_sendBufferWrite >= SERIAL_MODEM_SIM5218_SOCKET_BUFFER) {
        if (!writeBufferToSocket())
          return written;
      }
    }
    return written;
  }

  virtual void onSocketRead() {
    writeBufferToSocket();
  }

  virtual bool connectVoice() {
    return false;
  }

protected:

  bool writeBufferToSocket(bool forceClear=false) {
    if (_sendBufferWrite == 0)
      return false;

    SerialModem.writeCommand(cgb_sprintf(PMEM_STR("AT+TCPWRITE=%d"), _sendBufferWrite));

    PMemString feedCheck = PMEM_STR(">");
    response_check_t feedResponseCheck[] = {
      {feedCheck, false},
      {NULL, NULL}
    };
    if (!SerialModem.getResponse(feedResponseCheck, 30000))
      return false;

    SerialModem.writeBytes((const uint8_t *)&_sendBuffer[0], _sendBufferWrite);

    PMemString writeCheck = PMEM_STR("+TCPWRITE:");
    PMemString _RESPONSE_ERROR = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_ERROR));
    response_check_t writeResponseCheck[] = {
      {writeCheck, false},
      {_RESPONSE_ERROR, true},
      {NULL, NULL}
    };
    if (SerialModem.parseBasicResponse(SerialModem.getResponse(writeResponseCheck, 30000)) == Modem::SUCCESS ||
        forceClear)
      _sendBufferWrite = 0;
    return _sendBufferWrite == 0;
  }

  char _sendBuffer[SERIAL_MODEM_SIM5218_SOCKET_BUFFER];
  size_t _sendBufferWrite;
};

#define DRIVER_SIM5218 (new DriverSIM5218())

#endif // SERIAL_MODEM_DRIVERS_SIM5218_H