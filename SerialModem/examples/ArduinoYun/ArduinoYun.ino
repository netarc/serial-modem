#include "SerialModem.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SerialModem.setDriver(DRIVER_SIM5218);
  SerialModem.begin(&Serial1, 115200);
  while (!SerialModem.ready()) {
    Serial.println("waiting for modem ready..");
    delay(100);
  }

  while (SerialModem.getNetworkStatus() < NETWORK_STATUS_ROAMING) {
    Serial.println("waiting for network registration");
    delay(2000);
  }

  while (!SerialModem.setAPN(PROGMEM_STR("m2m.com.attz"))) {
    Serial.println("setting APN");
    delay(2000);
  }
}

void loop() {

}
