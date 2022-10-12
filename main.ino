#include <ArduinoJson.h>
#include <arduino_secrets.h>
#include <WiFi.h>


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


void setup() {
    // Initialize serial and wait for port to open.
    Serial.begin(9600);

    // Wait for serial port to connect. Needed for native USB port only.
    while (!Serial) {
        ;
    }

    // Connect WiFi status.
    connectWiFi();

    // Should be connected now, so print out the status.
    printWiFiStatus();
}

void loop() {
    // Read weather via API.
    getWeather();

    delay(10000);
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
    StaticJsonBuffer<5000> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(line);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }
  }
}


void connectWiFi() {
    // Initial WiFi status is IDLE.
    int status = WL_IDLE_STATUS;

    // Attempt to connect to WiFi network.
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: " + ssid);
        status = WiFi.begin(ssid, pass);
        delay(500);
    }
}

void printWiFiStatus() {
  // Print the SSID of the network attached to.
  Serial.print("SSID: " + WiFi.SSID());

  // Print WiFi shield's IP address.
  Serial.print("IP Address: " + WiFi.localIP());
}