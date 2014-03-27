#ifndef SERIAL_MODEM_IMODEMDRIVER_H
#define SERIAL_MODEM_IMODEMDRIVER_H

class IModemDriver {
public:
  /// Initially supported by Base

  // Ping the modem for responsiveness
  virtual bool attention() = 0;

  // Check the current status of the modems network connection
  virtual NetworkStatus networkStatus() = 0;

  // Set the password to be used to access the SIM
  virtual bool setSIMPin(const char *pin) = 0;

  /// Initially supported by Drivers

  // Set the PDP APN to be used by the modem
  virtual bool setAPN(const char *apn) = 0;

  // Enable the GPS on the modem
  virtual bool gpsEnable();

  // Set the server to be used to GPS calculations
  virtual bool gpsSetServer(char *server, uint16_t port) = 0;
};

#endif // SERIAL_MODEM_IMODEMDRIVER_H