#include "SerialModem.h"
#include "platforms/platform.h"


SerialModemClass SerialModem;
// The CircularBuffer is created with a reference to the shared buffer so compile
// time has an accurate SRAM usage calculation vs allocating it at run-time
char g_sharedBuffer[SERIAL_MODEM_SHARED_BUFFER];
CircularBuffer *Modem::g_circularBuffer = new CircularBuffer(g_sharedBuffer, SERIAL_MODEM_SHARED_BUFFER);

char PROGMEM Modem::RESPONSE_OK[]     = {"OK"};
char PROGMEM Modem::RESPONSE_ERROR[]  = {"ERROR"};

SerialModemClass::SerialModemClass() : _driver(NULL),
                                       _sim_pin(NULL),
                                       _powered_on(false) {
}

////////////////////////////////////////////////////////////////////////////////
// Configuration
////////////////////////////////////////////////////////////////////////////////

bool SerialModemClass::setSerial(SMSerialInterfaceClass serial, uint32_t baud) {
  _hardware_serial = serial;
  _hardware_serial->begin(baud);
}

void SerialModemClass::setDriver(IModemDriver *driver) {
  if (!driver)
    return;
  if (_driver)
    free(_driver);
  _driver = driver;
  _driver->configHardware(-1, -1);
}

IModemDriver * SerialModemClass::driver() {
  return _driver;
}

bool SerialModemClass::setSIMPin(char *pin) {
  return !!pin &&
         assert_driver() &&
         _driver->setSIMPin(pin);
}

bool SerialModemClass::setAPN(char *apn) {
  return !!apn &&
         assert_driver() &&
         _driver->setAPN(apn);
}


void SerialModemClass::configHardware(uint8_t pinPower, uint8_t pinDTR) {
  if (!assert_driver())
    return;
  _driver->configHardware(pinPower, pinDTR);
}

////////////////////////////////////////////////////////////////////////////////
// Operation
////////////////////////////////////////////////////////////////////////////////

bool SerialModemClass::ready() {
  if (!assert_driver())
    return false;

  if ( (!_powered_on && (_powered_on = _driver->attention())) ||
       (_powered_on = _driver->powerOn()) ) {
    onPowerOn();
    return true;
  }
  return false;
}

NetworkStatus SerialModemClass::getNetworkStatus() {
  return assert_driver() ?
         _driver->networkStatus() : NETWORK_STATUS_UNKNOWN;
}

//
// Modem Serial Handling
//

size_t SerialModemClass::writeBytes(const uint8_t *bytes, size_t size) {
  if (!assert_driver())
    return 0;
#ifdef DEBUG
  // could this be shortened/cleaned up?
  // we are just sniffing outgoing bytes to check for line-breaks for cleaner debug output..
  static bool gWriteIsNewline=true;
  if (gWriteIsNewline)
    DLog("> ");
  int offset = 0;
  char tmp[83];
  do {
    int chunk = size - offset;
    memset(&tmp, 0, 83);
    strncpy(tmp, (char *)bytes+offset, chunk > 80 ? 80 : chunk);
    DLog(tmp);
    offset+=80;
  } while (offset < size);
  gWriteIsNewline = (bytes[size-1] == '\n');
#endif
  return _hardware_serial->write(bytes, size);
}

uint8_t SerialModemClass::sendBasicCommand(const char *cmd, uint32_t timeout, char esc) {
  if (!assert_driver())
    return ERROR;

  return parseBasicResponse(sendCommand(cmd, NULL, timeout, esc));
}

char * SerialModemClass::sendCommand(const char *cmd, response_check_t *responseChecks, uint32_t timeout, char esc) {
  if (!assert_driver())
    return NULL;

  writeCommand(cmd, esc);
  if (responseChecks == NULL) {
    PMemString _RESPONSE_OK = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_OK));
    PMemString _RESPONSE_ERROR = PMemString(PMEM_STR_REF_ARRAY(RESPONSE_ERROR));
    response_check_t basicResponseCheck[] = {
      {_RESPONSE_OK, true},
      {_RESPONSE_ERROR, true},
      {NULL, NULL}
    };
    return getResponse(basicResponseCheck, timeout);
  }
  else {
    return getResponse(responseChecks, timeout);
  }
}

void SerialModemClass::writeCommand(const char *cmd, char esc) {
  if (!assert_driver())
    return;

  DLog("$ %s\n", cmd);

  // Clean the input buffer
  while(_hardware_serial->available() > 0) _hardware_serial->read();
  _hardware_serial->write(cmd);
  if (esc)
    _hardware_serial->write(esc);
}

char * SerialModemClass::getResponse(response_check_t *responseChecks, uint32_t timeout) {
  g_circularBuffer->resetLeft();

  bool started=false;
  uint32_t started_at = plt_millis();
  uint16_t previousRead = 0;
  uint16_t bytesRead = 0;
  do {
    plt_delay(10);

    previousRead = bytesRead;
    while (_hardware_serial->available()) {
      char ch = _hardware_serial->read();
      if (ch == ESC_CR ||
          ch == ESC_NL ||
          (ch >= 32 && ch <= 126) ) {
        g_circularBuffer->appendCircular(ch);
        bytesRead++;
      }
    }

    if (!started) {
      for (response_check_t *r=responseChecks; (r != NULL && r->name != NULL); r++) {
        if (g_circularBuffer->substring(r->name,
                                        r->escape ? ESC_CR : 0)) {
          started = true;
          break;
        }
      }
    }
    else if (previousRead == bytesRead) {
      break;
    }

    if ((plt_millis() - started_at) >= timeout) {
      DLog(" * sendCommand timeout\n");
      break;
    }
  } while(true);
  DLog("$> %s\n", g_circularBuffer->realignLeft());
  return started ? g_circularBuffer->realignLeft() : NULL;
}

uint8_t SerialModemClass::parseBasicResponse(char *response) {
  if (!response)
    return Modem::NO_RESPONSE;
  else if (strcasestr(response, PMemString(PMEM_STR_REF_ARRAY(RESPONSE_OK))))
    return Modem::SUCCESS;
  else if (strcasestr(response, PMemString(PMEM_STR_REF_ARRAY(RESPONSE_ERROR))))
    return Modem::ERROR;
  else
    return Modem::FAILURE;
}

void SerialModemClass::onPowerOn() {
  if (assert_driver()) {
    _driver->setEchoCommand(false);
    _driver->setErrorVerbose(true);
  }
  // writeCommand("AT&V"); // read current configuration
  // writeCommand("AT+CFUN?");
}

bool SerialModemClass::assert_driver() {
  if (!_driver) {
    DLog("* ERROR: no specified hardware interface\n");
    return false;
  }
  if (!_hardware_serial) {
    DLog("* ERROR: no specified hardware serial\n");
    return false;
  }
  return true;
}
