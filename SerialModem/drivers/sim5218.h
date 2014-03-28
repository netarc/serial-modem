#ifndef SERIAL_MODEM_DRIVERS_SIM5218_H
#define SERIAL_MODEM_DRIVERS_SIM5218_H

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
};

#define DRIVER_SIM5218 (new DriverSIM5218())

#endif // SERIAL_MODEM_DRIVERS_SIM5218_H