#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "arduino_secrets.h"


const char THING_ID[] = SECRET_ID;

const char SSID[] = SECRET_SSID;    // Network SSID (name)
const char PASS[] = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

float temperature;
int humidity;
int pressure;
int gas;
int iaq;

void initProperties(){
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(temperature, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(humidity, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(pressure, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(gas, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(iaq, READ, 5 * SECONDS, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
