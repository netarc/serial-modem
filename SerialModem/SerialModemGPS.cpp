#include "SerialModem.h"
#include "SerialModemGPS.h"

using namespace Modem;

SerialModemGPS::SerialModemGPS() : _configured(false) {
}

void SerialModemGPS::requestPosition() {
  if (!_configured) {
    // GPS Server
    sm_interface_gps_set_slp("supl.google.com", 7276);
    // SerialModem.sendBasicCommand("AT+CGPSURL=\"supl.google.com:7276\"") == Modem::SUCCESS;

    // GPS SSL not-required
    SerialModem.sendBasicCommand("AT+CGPSSSL=0") == Modem::SUCCESS;

    // GPS Enable
    sm_interface_gps_enable();
    // SerialModem.sendBasicCommand("AT+CGPS=1,2") == Modem::SUCCESS;
    _configured = true;
  }


  // if (SerialModem.sendBasicCommand("AT+CGPS=1,2") == Modem::SUCCESS) {
    char *response = SerialModem.sendCommand("AT+CGPSINFO", 1000);
    char *gpsInfo = strstr(response, "+CGPSINFO:");
    DLog(" * Found GPS Info:\n");
    while (*gpsInfo++ != 13)
      DLog("%c", *gpsInfo);
    DLog("\n");

    // SerialModem.sendBasicCommand("AT+CGPS=0") == Modem::SUCCESS;
  // }
}