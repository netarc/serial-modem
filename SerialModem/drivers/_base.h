#ifndef SERIAL_MODEM_DRIVERS_BASE_H
#define SERIAL_MODEM_DRIVERS_BASE_H

using namespace Modem;

class BaseDriver : public IModemDriver {
public:
  BaseDriver() {
    _connectedData = _connectedVoice = _connectedSocket = false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool powerOn() {
    if (_hardware_config.pinPower == 255)
      return attention();

    hw_setPinMode(_hardware_config.pinPower, hw_OUTPUT);
    hw_digitalWrite(_hardware_config.pinPower, hw_HIGH);
    plt_delay(200);
    hw_digitalWrite(_hardware_config.pinPower, hw_LOW);
    plt_delay(3000);

    return attention();
  }

  virtual bool powerOff() {
    return !attention();
  }

  virtual bool powerCycle() {
    return !attention();
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Software
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool attention() {
    return SerialModem.sendBasicCommand(PMEM_STR("AT"), 250) == Modem::SUCCESS;
  }

  virtual bool setEchoCommand(bool state) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("ATE%i"), state ? 1 : 0), 250) == Modem::SUCCESS;
  }

  virtual bool setErrorVerbose(bool state) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CMEE=%i"), state ? 2 : 0)) == Modem::SUCCESS;
  }

  virtual NetworkStatus networkStatus() {
    PMemString tokenMatch = PMEM_STR(" ,\n\r");

    char *response = SerialModem.sendCommand(PMEM_STR("AT+CREG?"));

    if (!response || !strstr(response, PMemString(PMEM_STR_REF_ARRAY(RESPONSE_OK))))
      return NETWORK_STATUS_UNKNOWN;

    char *r = strstr(response, PMEM_STR("+CREG:"));
    char *tkn = strtok(r, tokenMatch);
    tkn = strtok(NULL, tokenMatch);
    tkn = strtok(NULL, tokenMatch);
    uint8_t status = atoi(tkn);
    switch (status) {
      case 0: return NETWORK_STATUS_NOT_REGISTERED;
      case 1: return NETWORK_STATUS_REGISTERED;
      case 2: return NETWORK_STATUS_SEARCHING;
      case 3: return NETWORK_STATUS_DENIED;
      case 5: return NETWORK_STATUS_ROAMING;
      default: return NETWORK_STATUS_UNKNOWN;
    }
  }

  virtual bool setSIMPin(const char *pin) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PMEM_STR("AT+CPIN=%s"), pin)) == Modem::SUCCESS;
  }

  virtual void onSocketRead() {}

protected:
  bool _connectedSocket;
  bool _connectedData;
  bool _connectedVoice;
};

#endif // SERIAL_MODEM_DRIVERS_BASE_H