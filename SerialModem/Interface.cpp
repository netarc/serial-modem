#include "SerialModem.h"

using namespace Modem;

bool sm_interface_attention() {
  return SerialModem.sendBasicCommand(PROGMEM_STR("AT")) == Modem::SUCCESS;
}

sm_network_status sm_interface_network_status() {
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


bool sm_interface_set_sim_pin(char *pin) {
  return SerialModem.sendBasicCommand(cgb_sprintf(PROGMEM_STR("AT+CPIN=%s"), pin)) == Modem::SUCCESS;
  return false;
}

bool sm_interface_set_apn(char *apn) {
  const char *cmd;
  switch(SerialModem._hardware_interface) {
    case INTERFACE_MTSMC_H5:
      cmd = cgb_sprintf(PROGMEM_STR("AT+CGDCONT=1,\"IP\",\"APN\",\"%s\",0,0"), apn);
      break;
    case INTERFACE_SIM5218:
      cmd = cgb_sprintf(PROGMEM_STR("AT+CGSOCKCONT=1,\"IP\",\"%s\""), apn);
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cmd) == Modem::SUCCESS;
}

bool sm_interface_gps_set_slp(char *slp, uint16_t port) {
  const char *cmd;
  switch(SerialModem._hardware_interface) {
    case INTERFACE_MTSMC_H5:
      cmd = cgb_sprintf(PROGMEM_STR("AT+LCSSLP=1,\"%s\",%d"), slp, port);
      break;
    case INTERFACE_SIM5218:
      cmd = cgb_sprintf(PROGMEM_STR("AT+CGPSURL=\"%s:%d\""), slp, port);
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cmd) == Modem::SUCCESS;
}

bool sm_interface_gps_enable() {
  const char *cmd;
  switch(SerialModem._hardware_interface) {
    case INTERFACE_MTSMC_H5:
      cmd = cgb_sprintf(PROGMEM_STR("AT+GPSSLSR=1,1"));
      break;
    case INTERFACE_SIM5218:
      cmd = cgb_sprintf(PROGMEM_STR("AT+CGPS=1,2"));
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cmd) == Modem::SUCCESS;
}
