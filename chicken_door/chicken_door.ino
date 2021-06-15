// Adafruit IO Servo Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-servo
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

#include <Wire.h>

#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
AdafruitIO_Feed *digital = io.feed("digital");

void setup() {
 
  Serial.begin(115200);
  while(! Serial);
  AFMS.begin();
  myMotor->setSpeed(200);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  digital->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  digital->get();

}
void loop() {
 uint8_t i;
  io.run();
}

void handleMessage(AdafruitIO_Data *data) {
 int reading = data->toInt();
  Serial.print("received <- ");

  if (reading == 1){
    Serial.println("Opening");
   myMotor->run(FORWARD);
  delay(65000);
  myMotor->run(RELEASE);}
  else
    Serial.println("Already Open");

  
  if (reading == 2){
    Serial.println("Closing");
     myMotor->run(BACKWARD);
  delay(65000);
  myMotor->run(RELEASE);}
   else
    Serial.println("Already closed");
         
     if (reading == 3){
    Serial.println("Resetting");
     myMotor->run(BACKWARD);
  delay(60000);
  myMotor->run(RELEASE);}
   else
    Serial.println("Already Open");
}
