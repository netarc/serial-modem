#include "SerialModem.h"

using namespace Modem;

bool sm_interface_attention() {
  return SerialModem.sendBasicCommand(PROGMEM_STR("AT")) == Modem::SUCCESS;
}

sm_network_status sm_interface_network_status() {
  char *response = SerialModem.sendCommand(PROGMEM_STR("AT+CREG?"));

  if (!response || !strstr(response, "OK"))
    return NETWORK_STATUS_UNKNOWN;

  char *r = strstr(response, "+CREG:");
  char *tkn = strtok(r, " ,\n\r");
  tkn = strtok(NULL, " ,\n\r");
  tkn = strtok(NULL, " ,\n\r");
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
      cmd = PROGMEM_STR("AT+CGDCONT=1,\"IP\",\"APN\",\"%s\"");
      break;
    case INTERFACE_SIM5218:
      cmd = PROGMEM_STR("AT+CGSOCKCONT=1,\"IP\",\"%s\"");
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cgb_sprintf(cmd, apn)) == Modem::SUCCESS;
}

bool sm_interface_gps_set_slp(char *slp, uint16_t port) {
  const char *cmd;
  switch(SerialModem._hardware_interface) {
    case INTERFACE_MTSMC_H5:
      cmd = PROGMEM_STR("AT+LCSSLP=1,\"%s\",%d");
      break;
    case INTERFACE_SIM5218:
      cmd = PROGMEM_STR("AT+CGPSURL=\"%s:%d\"");
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cgb_sprintf(cmd, slp, port)) == Modem::SUCCESS;
}

bool sm_interface_gps_enable() {
  const char *cmd;
  switch(SerialModem._hardware_interface) {
    case INTERFACE_MTSMC_H5:
      cmd = PROGMEM_STR("AT+GPSSLSR=1,1");
      break;
    case INTERFACE_SIM5218:
      cmd = PROGMEM_STR("AT+CGPS=1,2");
      break;
    default:
      return false;
  }
  return SerialModem.sendBasicCommand(cmd) == Modem::SUCCESS;
}
