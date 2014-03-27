#ifndef SERIAL_MODEM_DRIVERS_BASE_H
#define SERIAL_MODEM_DRIVERS_BASE_H

using namespace Modem;

class BaseDriver : public IModemDriver {
public:
  virtual bool attention() {
    return SerialModem.sendBasicCommand(PROGMEM_STR("AT")) == Modem::SUCCESS;
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