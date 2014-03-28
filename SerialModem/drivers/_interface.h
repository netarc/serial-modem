#ifndef SERIAL_MODEM_IMODEMDRIVER_H
#define SERIAL_MODEM_IMODEMDRIVER_H

class IModemDriver {
public:
  void configHardware(uint8_t pinPower, uint8_t pinDTR) {
    _hardware_config.pinPower = pinPower;
    _hardware_config.pinDTR = pinDTR;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool powerOn() = 0;
  virtual bool powerOff() = 0;
  virtual bool powerCycle() = 0;

  ////////////////////////////////////////////////////////////////////////////////
  // Software
  ////////////////////////////////////////////////////////////////////////////////

  // Ping the modem for responsiveness
  virtual bool attention() = 0;

  // Echo back all commands for verification
  virtual bool setEchoCommand(bool enabled) = 0;

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

protected:
  hardware_config_t _hardware_config;
};

#endif // SERIAL_MODEM_IMODEMDRIVER_H