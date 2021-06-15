
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
