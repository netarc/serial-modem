#ifndef SERIAL_MODEM_DRIVERS_MTSMC_H5_H
#define SERIAL_MODEM_DRIVERS_MTSMC_H5_H

// AT Commands for Universal IP Products
// http://www.multitech.com/en_US/DOCUMENTS/Collateral/manuals/S000457L.pdf
// AT Commands for version 12.00.023 -H5 HSPA+ and 12.00.223 -H6 Products
// http://www.multitech.com/en_US/DOCUMENTS/Collateral/manuals/HPSA+%20AT%20Commands%20S000528C.pdf

using namespace Modem;

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
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CGDCONT=1,\"IP\",\"APN\",\"%s\",0,0"), apn)) == Modem::SUCCESS;
  }

  virtual bool gpsEnable() {
    return SerialModem.sendBasicCommand(PROGMEM_STR("AT+GPSSLSR=1,1")) == Modem::SUCCESS;
  }

  virtual bool gpsSetServer(char *server, uint16_t port) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+LCSSLP=1,\"%s\",%d"), server, port)) == Modem::SUCCESS;
  }
};

#define DRIVER_MTSMC_H5 (new DriverMTSMC_H5())

#endif // SERIAL_MODEM_DRIVERS_MTSMC_H5_H