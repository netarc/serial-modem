#ifndef _SERIAL_MODEM_INTERFACE__H_
#define _SERIAL_MODEM_INTERFACE__H_

enum sm_interface {
  INTERFACE_MTSMC_H5  = 0x01,
  INTERFACE_SIM5218   = 0x02
};

enum sm_network_status {
  NETWORK_STATUS_UNKNOWN        = 0x00,
  NETWORK_STATUS_DENIED         = 0x01,
  NETWORK_STATUS_NOT_REGISTERED = 0x02,
  NETWORK_STATUS_SEARCHING      = 0x03,
  NETWORK_STATUS_ROAMING        = 0x04,
  NETWORK_STATUS_REGISTERED     = 0x05
};

// Since SRAM is valued more than PROGMEM our hardware interfaces are through static
// methods vs a virtual class inheritance allocated at runtime.

bool sm_interface_attention();
sm_network_status sm_interface_network_status();
bool sm_interface_set_sim_pin(char *pin);
bool sm_interface_set_apn(char *apn);
bool sm_interface_gps_set_slp(char *slp, uint16_t port);
bool sm_interface_gps_enable();

#endif // _SERIAL_MODEM_INTERFACE__H_
