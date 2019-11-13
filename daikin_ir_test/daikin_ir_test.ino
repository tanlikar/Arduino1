#include "DaikinIRESP8266.h"

DaikinIRESP8266 daikin = DaikinIRESP8266(4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  daikin.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  daikin.setPower(true);
  daikin.setTemp(25);
  daikin.setFan(MAX);
  daikin.setSwingAuto(true);
  daikin.sendComm();
  delay(3000);
}
