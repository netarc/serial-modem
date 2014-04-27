#ifndef SERIAL_MODEM_DRIVERS_MTSMC_H5_H
#define SERIAL_MODEM_DRIVERS_MTSMC_H5_H

// AT Commands for Universal IP Products
// http://www.multitech.com/en_US/DOCUMENTS/Collateral/manuals/S000457L.pdf
// AT Commands for version 12.00.023 -H5 HSPA+ and 12.00.223 -H6 Products
// http://www.multitech.com/en_US/DOCUMENTS/Collateral/manuals/HPSA+%20AT%20Commands%20S000528C.pdf


class DriverMTSMC_H5 : public BaseDriver {
public:
  DriverMTSMC_H5() {}

  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Software
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool setAPN(const char *apn) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT#APNSERV=\"%s\""), apn)) == Modem::SUCCESS;
    // return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CGDCONT=1,\"IP\",\"%s\""), apn)) == Modem::SUCCESS;
  }

  virtual bool gpsEnable() {
    return SerialModem.sendBasicCommand(PMEM_STR("AT+GPSSLSR=1,1")) == Modem::SUCCESS;
  }

  virtual bool gpsSetServer(char *server, uint16_t port) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+LCSSLP=1,\"%s\",%d"), server, port)) == Modem::SUCCESS;
  }

  virtual bool connectData() {
    // MTSMC_H5 cannot do voice & data at same time?
    if (_connectedVoice)
      return false;

    if (_connectedData)
      return true;

    char *response;
    SerialModem.sendBasicCommand(PMEM_STR("AT#GPRSMODE=1"));
    // response = SerialModem.sendCommand(PMEM_STR("AT#VSTATE"), 500, ESC_CR, PMEM_STR("STATE:"));
    if (!strcasestr(response, "CONNECTED")) {
      PMemString checkResponse = PMEM_STR("Ok_Info_GprsActivation");
      PMemString _RESPONSE_OK = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_OK));
      PMemString _RESPONSE_ERROR = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_ERROR));
      response_check_t responseCheck[] = {
        {_RESPONSE_OK, true},
        {_RESPONSE_ERROR, true},
        {checkResponse, true},
        {NULL, NULL}
      };
      response = SerialModem.sendCommand(PMEM_STR("AT#CONNECTIONSTART"), responseCheck, 30000, ESC_CR);
      _connectedData = !!strstr(response, checkResponse);
    }

    return _connectedData;
  }

  virtual bool openSocketTCP(const char *address, uint16_t port) {
    if (!_connectedData || _connectedSocket)
      return false;

    if (SerialModem.sendBasicCommand(PMEM_STR("AT#DLEMODE=1,1")) != Modem::SUCCESS ||
        SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT#TCPPORT=1,%d"), port)) != Modem::SUCCESS ||
        SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT#TCPSERV=1,\"%s\""), address)) != Modem::SUCCESS)
      return false;

    PMemString checkResponse = PMEM_STR("Ok_Info_WaitingForData");
    PMemString _RESPONSE_OK = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_OK));
    PMemString _RESPONSE_ERROR = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_ERROR));
    response_check_t responseCheck[] = {
      {_RESPONSE_OK, true},
      {_RESPONSE_ERROR, true},
      {checkResponse, true},
      {NULL, NULL}
    };
    char *response = SerialModem.sendCommand(PMEM_STR("AT#OTCP=1"), responseCheck, 10000);
    _connectedSocket = !!response && !!strcasestr(response, checkResponse);
    return _connectedSocket;
  }

  virtual bool closeSocket() {
    if (!_connectedSocket)
      return false;
    _connectedSocket = false;
    uint8_t escape = ESC_DLE;
    SerialModem.writeBytes(&escape, 1);
  }

  virtual int writeSocket(const uint8_t *bytes, size_t size) {
    if (!_connectedData || !_connectedSocket)
      return 0;
    size_t n = 0;
    while (size--) {
      if (*bytes == ESC_ETX) {
        uint8_t escape = ESC_DLE;
        SerialModem.writeBytes(&escape, 1);
      }
      SerialModem.writeBytes(bytes, 1);
      bytes++;
      n++;
    }
    return n;
  }

  virtual bool connectVoice() {
    return false;
  }

  // Disconnect
  // SerialModem.sendBasicCommand(PMEM_STR("AT#CONNECTIONSTOP"), 10000)
};

#define DRIVER_MTSMC_H5 (new DriverMTSMC_H5())

#endif // SERIAL_MODEM_DRIVERS_MTSMC_H5_H