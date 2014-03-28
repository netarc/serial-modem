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

typedef struct {
  uint8_t pinPower;
  uint8_t pinDTR;
} hardware_config_t;


#include "platforms/platform.h"
#include "util.h"
#include "debug.h"
#include "drivers/_interface.h"

#define SERIAL_MODEM_SHARED_BUFFER 256



class SerialModemClass {
public:
  SerialModemClass();

  ////////////////////////////////////////////////////////////////////////////////
  // Configuration
  ////////////////////////////////////////////////////////////////////////////////

  // Set the serial interface for this modem
  bool setSerial(SMSerialInterfaceClass serial, uint32_t baud=115200);

  void setDriver(IModemDriver *driver);
  IModemDriver *driver();

  // Set the pin used to unlock the SIM.
  // By default there is no pin set and can always be cleared by setting to NULL.
  bool setSIMPin(char *pin);
  // Set the APN to be used for data access.
  // By default there is no APN set and the device default is used.
  bool setAPN(char *apn);

  void configHardware(uint8_t pinPower, uint8_t pinDTR=-1);

  ////////////////////////////////////////////////////////////////////////////////
  // Operation
  ////////////////////////////////////////////////////////////////////////////////

  bool ready();

  NetworkStatus getNetworkStatus();

  //
  // Modem Serial Handling
  //

  uint8_t sendBasicCommand(const char *cmd, uint32_t timeout=500, char esc = Modem::ESC_CR);
  char * sendCommand(const char *cmd, uint32_t timeout=500, char esc = Modem::ESC_CR, char *responseCheck=NULL);
  size_t writeBytes(const uint8_t *bytes, size_t size);
  uint8_t readLine(char *buffer, uint8_t size, unsigned int timeout);

  SMSerialInterfaceClass _hardware_serial;

protected:
  void onPowerOn();

  bool assert_driver();

  IModemDriver *_driver;
  bool _powered_on;
  char *_sim_pin;
};

extern SerialModemClass SerialModem;

#include "drivers/_base.h"
#include "drivers/mtsmc_h5.h"
#include "drivers/sim5218.h"

#include <SerialModemClient.h>
#include <SerialModemGPS.h>

#endif // _SERIAL_MODEM__H_
