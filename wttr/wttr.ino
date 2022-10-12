#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiSSLClient.h>

#include "arduino_secrets.h"


// Network SSID (name).
char ssid[] = SECRET_SSID;
// Network WPA key.
char pass[] = SECRET_PASS;

// WiFi client.
WiFiClient client;

// Open weather map api key.
String apiKey = SECRET_APIKEY;

// The city you want the weather for.
String location = "torino,IT";

// Open wather map server name.
char server[] = "api.openweathermap.org";

// initial WiFi status is IDLE:
int status = WL_IDLE_STATUS;

void connectWiFi() {
    // Attempt to connect to WiFi network.
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(1000);
    }
}

void getWeather() {
  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.print("GET /data/2.5/forecast?");
      client.print("q="+location);
      client.print("&appid="+apiKey);
      client.print("&cnt=3");
      client.println("&units=metric");
      client.println("Host: api.openweathermap.org");
      client.println("Connection: close");
      client.println();
  } else {
      Serial.println("unable to connect");
  }

  delay(1000);
  String line = "";

  while (client.connected()) {
      line = client.readStringUntil('\n');

      //Serial.println(line);
      Serial.println("parsingValues");

      //create a json buffer where to store the json data
      DynamicJsonDocument doc(5000);

      DeserializationError error = deserializeJson(doc, line);
      if (error)
          Serial.println("parseObject() failed");
          return;
  }
}

void printWiFiStatus() {
    // Print the SSID of the network attached to.
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // Print WiFi shield's IP address/mask.
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("IP Subnet: ");
    Serial.println(WiFi.subnetMask());
}

void scanNetworks() {
    // scan for nearby networks:
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    Serial.print("SSID List:");
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
}

void loop() {
    // Read weather via API.
    getWeather();

    delay(10000);
}
