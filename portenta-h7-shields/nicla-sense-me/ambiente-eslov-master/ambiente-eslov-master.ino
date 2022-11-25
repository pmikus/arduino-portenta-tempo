#include "Arduino.h"
#include "Arduino_BHY2Host.h"
#include <WiFi.h>
#include <WiFiClient.h>

#include "arduino_secrets.h"


// Network SSID (name).
char ssid[] = SECRET_SSID;
// Network WPA key.
char pass[] = SECRET_PASS;

// WiFi client.
WiFiClient client;

// Nicla Sense ME sensors
Sensor temperature(SENSOR_ID_TEMP);
Sensor humidity(SENSOR_ID_HUM);
Sensor pressure(SENSOR_ID_BARO);
Sensor gas(SENSOR_ID_GAS);
SensorXYZ gyroscope(SENSOR_ID_GYRO);
SensorXYZ accelerometer(SENSOR_ID_ACC);
SensorQuaternion quaternion(SENSOR_ID_RV);
SensorBSEC bsec(SENSOR_ID_BSEC);

unsigned long getTime() {
  return WiFi.getTime();
}

void connectWiFi() {
    // initial WiFi status is IDLE:
    int status = WL_IDLE_STATUS;

    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }

    Serial.println("Connected to WiFi");
}

void printWiFiStatus() {
    // print the SSID of the network attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print WiFi shield's IP address/mask:
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("IP Subnet: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println((IPAddress)WiFi.gatewayIP());

    // print the received signal strength:
    Serial.print("signal strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    String fv = WiFi.firmwareVersion();
    Serial.println(fv);
}

void scanNetworks() {
    // scan for nearby networks:
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    Serial.print("SSID List: ");
    Serial.println(numSsid);
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        Serial.print(thisNet);
        Serial.print(") Network: ");
        Serial.println(WiFi.SSID(thisNet));
    }
}

void setup() {
    // initialize serial and wait for port to open:
    Serial.begin(9600);

    // scan for existing networks:
    scanNetworks();

    // Connect WiFi status:
    connectWiFi();

    // should be connected now, so print out the status:
    printWiFiStatus();

    Serial.println("Initialize the Nicla communication");
    BHY2Host.begin(false, NICLA_VIA_ESLOV);

    temperature.configure(1, 0);
    float temperature = tempSensor.value();
}

void loop() {
    BHY2Host.update();
    Serial.print("Temperature: ");
    Serial.println(temperature.value());
}
