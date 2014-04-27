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

  // Enable or Disable command echoing
  virtual bool setEchoCommand(bool state) = 0;

  // Enable or Disable Error verbosity
  virtual bool setErrorVerbose(bool state) = 0;

  // Check the current status of the modems network connection
  virtual NetworkStatus networkStatus() = 0;

  // Set the password to be used to access the SIM
  virtual bool setSIMPin(const char *pin) = 0;

  // Establish network data connection
  virtual bool connectData() = 0;
  virtual bool openSocketTCP(const char *address, uint16_t port) = 0;
  virtual bool closeSocket() = 0;
  virtual int writeSocket(const uint8_t *bytes, size_t size) = 0;
  virtual void onSocketRead() = 0;
  // Establish network voice connection
  virtual bool connectVoice() = 0;

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