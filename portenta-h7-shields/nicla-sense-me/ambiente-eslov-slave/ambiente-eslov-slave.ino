#include "Arduino.h"
#include "Arduino_BHY2.h"

// Set DEBUG to true in order to enable debug print
#define DEBUG false

void setup(){
#if DEBUG
    Serial.begin(9600);
    BHY2.debug(Serial);
#endif
    BHY2.begin(NICLA_BLE_AND_I2C, NICLA_VIA_ESLOV);
}

void loop(){
    BHY2.update(100);
}