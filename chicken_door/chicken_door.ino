#include "config.h"
#include <AdafruitIO.h>
#include <AdafruitIO_WiFi.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
AdafruitIO_Feed *digital = io.feed("digital");

unsigned long motorStartTime = 0;
bool motorRunning = false;
int motorDirection = RELEASE;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 1000); // Wait up to 1s for Serial
  Serial.println("Starting...");

  AFMS.begin();
  myMotor->setSpeed(200);
  Serial.println("Motor Shield initialized");

  Serial.print("Connecting to Adafruit IO");
  io.connect();
  digital->onMessage(handleMessage);

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  digital->get();
}

void loop() {
  io.run();
  if (motorRunning && millis() - motorStartTime >= (motorDirection == FORWARD ? 65000UL : 60000UL)) {
    myMotor->run(RELEASE);
    motorRunning = false;
    Serial.println("Motor stopped");
  }
}

void handleMessage(AdafruitIO_Data *data) {
  int reading = data->toInt();
  Serial.print("received <- ");
  Serial.println(reading);

  if (reading == 1 && !motorRunning) {
    Serial.println("Opening");
    myMotor->setSpeed(200);
    myMotor->run(FORWARD);
    motorStartTime = millis();
    motorRunning = true;
    motorDirection = FORWARD;
  } else if (reading == 2 && !motorRunning) {
    Serial.println("Closing");
    myMotor->setSpeed(200);
    myMotor->run(BACKWARD);
    motorStartTime = millis();
    motorRunning = true;
    motorDirection = BACKWARD;
  } else if (reading == 3 && !motorRunning) {
    Serial.println("Resetting");
    myMotor->setSpeed(200);
    myMotor->run(BACKWARD);
    motorStartTime = millis();
    motorRunning = true;
    motorDirection = BACKWARD;
  } else {
    Serial.println(motorRunning ? "Motor busy" : "Invalid command");
  }
}
