#ifndef _SERIAL_MODEM__H_
#define _SERIAL_MODEM__H_

// #define DEBUG

#include <inttypes.h>

enum NetworkStatus {
  NETWORK_STATUS_UNKNOWN        = 0x00,
  NETWORK_STATUS_DENIED         = 0x01,
  NETWORK_STATUS_NOT_REGISTERED = 0x02,
  NETWORK_STATUS_SEARCHING      = 0x03,
  NETWORK_STATUS_ROAMING        = 0x04,
  NETWORK_STATUS_REGISTERED     = 0x05
};


#include "platforms/platform.h"
#include "util.h"
#include "debug.h"
#include "drivers/_interface.h"

#define SERIAL_MODEM_SHARED_BUFFER 256


class SerialModemClass {
public:
  SerialModemClass();

  // This will start the modem if it has not been already and will also initialize the modem
  bool begin(SMSerialInterfaceClass serial, uint32_t baud=115200);

  bool ready();

  //
  // Hardware Interface
  //

  void setDriver(IModemDriver *driver);
  IModemDriver *driver();

  // Set the pin used to unlock the SIM.
  // By default there is no pin set and can always be cleared by setting to NULL.
  bool setSIMPin(char *pin);
  // Set the APN to be used for data access.
  // By default there is no APN set and the device default is used.
  bool setAPN(char *apn);

  NetworkStatus getNetworkStatus();

  //
  // Hardware Control
  //

  // Send a hardware start signal if the modem is not already known to be on.
  void hwStart();

  // Send a hardware shutdown signal if the modem is not already known to be off.
  void hwShutdown();

  // Set the pin used to signal the modem on/off
  void setHardwarePowerPin(uint8_t pin);

  // Set the pin used to signal the modem of DTR
  void setHardwareDTRPin(uint8_t pin);

  //
  // Modem Serial Handling
  //

  uint8_t sendBasicCommand(const char *cmd, uint32_t timeout=1000, char esc = Modem::ESC_CR);
  char * sendCommand(const char *cmd, uint32_t timeout=1000, char esc = Modem::ESC_CR, char *responseCheck=NULL);
  size_t writeBytes(const uint8_t *bytes, size_t size);
  uint8_t readLine(char *buffer, uint8_t size, unsigned int timeout);

  SMSerialInterfaceClass _hardware_serial;

private:
  bool assert_driver();

  IModemDriver *_driver;
  bool _powered_on;
  char *_sim_pin;
  uint8_t _hardware_power_pin;
  uint8_t _hardware_dtr_pin;
};

extern SerialModemClass SerialModem;

#include "drivers/_base.h"
#include "drivers/mtsmc_h5.h"
#include "drivers/sim5218.h"

#include <SerialModemClient.h>
#include <SerialModemGPS.h>

#endif // _SERIAL_MODEM__H_
