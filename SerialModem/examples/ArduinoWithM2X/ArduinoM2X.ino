#include "jsonlite.h"
#include "M2XStreamClient.h"
#include "SerialModem.h"

SerialModemClient client;
char m2xFeedId[] = "feed-id";    // Feed you want to post to
char m2xKey[] = "access-key";    // Your M2X access key
M2XStreamClient m2xClient(&client, m2xKey);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SerialModem.setHardwareInterface(INTERFACE_SIM5218);
  SerialModem.begin(&Serial, 115200);
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
  int result = m2xClient.post(m2xFeedId, "temperature", 12.1f);
  Serial.print("M2x client response code: ");
  Serial.println(result);

  delay(10000);
}
