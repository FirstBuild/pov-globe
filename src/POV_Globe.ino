/*
 * Project PhotonicGlobe
 * Description:
 * Author:
 * Date:
 */

#include "RBD_Timer.h"

#define hallEffectSensorPin 6

volatile int rotationCounter = 0;
int rotationsPerSec = 0;
int rotationsPerMin = 0;
RBD::Timer printOutput;

void rpmCounter() {
  rotationCounter++;
}

void setup() {
  Serial.begin(9600);
  printOutput.setTimeout(1000);

  pinMode(hallEffectSensorPin, INPUT_PULLUP);
  attachInterrupt(hallEffectSensorPin, rpmCounter, FALLING);
  printOutput.restart();
}

void loop() {
  if (printOutput.onExpired()) {
    detachInterrupt(hallEffectSensorPin);
    rotationsPerMin = rotationCounter * 60;
    rotationsPerSec = rotationCounter;

    // Rotations per minute
    Serial.print("RPM: ");
    Serial.println(rotationsPerMin);

    // Rotations per second
    Serial.print("RPS: ");
    Serial.println(rotationsPerSec);

    rotationCounter = 0;

    printOutput.restart();
    attachInterrupt(hallEffectSensorPin, rpmCounter, FALLING);
  }
}
