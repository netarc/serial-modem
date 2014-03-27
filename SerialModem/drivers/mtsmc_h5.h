#ifndef SERIAL_MODEM_DRIVERS_MTSMC_H5_H
#define SERIAL_MODEM_DRIVERS_MTSMC_H5_H

using namespace Modem;

class DriverMTSMC_H5 : public BaseDriver {
public:
  DriverMTSMC_H5() {}

  virtual bool setAPN(const char *apn) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CGDCONT=1,\"IP\",\"APN\",\"%s\",0,0"), apn)) == Modem::SUCCESS;
  }

  virtual bool gpsEnable() {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+GPSSLSR=1,1"))) == Modem::SUCCESS;
  }

  virtual bool gpsSetServer(char *server, uint16_t port) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+LCSSLP=1,\"%s\",%d"), server, port)) == Modem::SUCCESS;
  }
};

#define DRIVER_MTSMC_H5 (new DriverMTSMC_H5())

#endif // SERIAL_MODEM_DRIVERS_MTSMC_H5_H