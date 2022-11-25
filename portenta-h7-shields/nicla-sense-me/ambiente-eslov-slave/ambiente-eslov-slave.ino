#include "Arduino.h"
#include "Arduino_BHY2.h"


// Set DEBUG to true in order to enable debug print
#define DEBUG true

void setup(){
#if DEBUG
  Serial.begin(115200);
  BHY2.debug(Serial);
#endif
  BHY2.begin(NICLA_I2C, NICLA_VIA_ESLOV);
}

void loop(){
  // Update and then sleep
  BHY2.update(100);
}
