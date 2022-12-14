/* 
 * Upload this sketch to a Host board to use it as an I2C bridge between 
 * Nicla Sense ME and the Arduino Cloud. 
 * Nicla Sense ME board needs to run the basic App.ino sketch and to be plugged
 * as a shield on top of the Host board.
 * 
 * Before uploading this sketch to the Host board:
 * - add your Host device to Arduino Cloud
 * - setup temperature and seconds Things in Arduino Cloud
 * - get the THING_ID and copy it in thingProperties.h
 * - create a Dashboard in Arduino Cloud, like a live chart of the temperature
*/

#include "thingProperties.h"

#include "Arduino.h"
#include "Arduino_BHY2Host.h"
#include <WiFi.h>
#include <WiFiClient.h>


// Set DEBUG to true in order to enable debug print
#define DEBUG true

// Network SSID (name).
char ssid[] = SECRET_SSID;
// Network WPA key.
char pass[] = SECRET_PASS;

// WiFi client.
WiFiClient client;

// WiFi server.
WiFiServer server(80);

// Nicla Sense ME sensors.
Sensor sensorTemp(SENSOR_ID_TEMP);
Sensor sensorHum(SENSOR_ID_HUM);
Sensor sensorBaro(SENSOR_ID_BARO);
Sensor sensorGas(SENSOR_ID_GAS);
SensorXYZ sensorGyro(SENSOR_ID_GYRO);
SensorXYZ sensorAcc(SENSOR_ID_ACC);
SensorQuaternion sensorRv(SENSOR_ID_RV);
SensorBSEC sensorBsec(SENSOR_ID_BSEC);

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

void setup() {
    // initialize serial and wait for port to open:
    Serial.begin(115200);
    while(!Serial) {
        ;      
    }
  
    // defined in thingProperties.h:
    initProperties();

    // scan for existing networks:
    scanNetworks();

    // connect WiFi status:
    connectWiFi();

    // should be connected now, so print out the status:
#if DEBUG
    printWiFiStatus();
#endif

    // initilize Nicla Sense ME:
#ifdef ARDUINO_ARCH_MBED
    BHY2Host.begin();
#else
    BHY2Host.begin(false, NICLA_VIA_ESLOV);
#endif
    sensorTemp.begin();
    sensorHum.begin();
    sensorBaro.begin();
    sensorGas.begin();

    // start the web server on port 80:
    server.begin();
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected

      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            BHY2Host.update();
              
            temperature = sensorTemp.value();
            humidity = sensorHum.value();
            pressure = sensorBaro.value();
            gas = sensorGas.value();

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<html><head>");
            client.print("<style>");
            client.print("* { font-family: sans-serif;}");
            client.print("body { padding: 2em; font-size: 2em; text-align: center;}");
            client.print("a { -webkit-appearance: button;-moz-appearance: button;appearance: button;text-decoration: none;color: initial; padding: 25px;} #red{color:red;} #green{color:green;} #blue{color:blue;}");
            client.print("</style></head>");
            client.print("<body><h1> AMBIENTE </h1>");
            client.print("<h2><span id=\"red\">Temperature</span></h2>");
            client.print(temperature, 1);
            client.print("<h2><span id=\"red\">Humidity</span></h2>");
            client.print(humidity, 1);
            client.print("<h2><span id=\"red\">Pressure</span></h2>");
            client.print(pressure);
            client.print("<h2><span id=\"red\">Gas</span></h2>");
            client.print(gas);
            client.print("</body></html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
