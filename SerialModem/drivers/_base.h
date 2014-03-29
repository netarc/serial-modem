#ifndef SERIAL_MODEM_DRIVERS_BASE_H
#define SERIAL_MODEM_DRIVERS_BASE_H

using namespace Modem;

class BaseDriver : public IModemDriver {
public:
  ////////////////////////////////////////////////////////////////////////////////
  // Hardware
  ////////////////////////////////////////////////////////////////////////////////

  virtual bool powerOn() {
    if (_hardware_config.pinPower == -1)
      return true;

    hw_setPinMode(_hardware_config.pinPower, hw_OUTPUT);
    hw_digitalWrite(_hardware_config.pinPower, hw_HIGH);
    plt_delay(200);
    hw_digitalWrite(_hardware_config.pinPower, hw_LOW);
    plt_delay(3000);
    for (int i=0; i<10; i++) {
      plt_delay(750);
      if (attention())
        return true;
    }
    return false;
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
    return SerialModem.sendBasicCommand(PROGMEM_STR("AT"), 250) == Modem::SUCCESS;
  }

  virtual bool setEchoCommand(bool state) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("ATE%i"), state ? 1 : 0), 250) == Modem::SUCCESS;
  }

  virtual bool setErrorVerbose(bool state) {
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CMEE=%i"), state ? 2 : 0)) == Modem::SUCCESS;
  }

  virtual NetworkStatus networkStatus() {
    __PROGMEM_STR tokenMatch = PROGMEM_STR(" ,\n\r");

    char *response = SerialModem.sendCommand(PROGMEM_STR("AT+CREG?"));

    if (!response || !strstr(response, PROGMEM_STR("OK")))
      return NETWORK_STATUS_UNKNOWN;

    char *r = strstr(response, PROGMEM_STR("+CREG:"));
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
    return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CPIN=%s"), pin)) == Modem::SUCCESS;
  }
};

#endif // SERIAL_MODEM_DRIVERS_BASE_H