/*
 * Project PhotonicGlobe
 * Description:
 * Author:
 * Date:
 */

#include "RBD_Timer.h"

volatile int rpmCount = 0;
RBD::Timer printOutput;

void rpmCounter() {
  rpmCount++;
}

void setup() {
  Serial.begin(9600);
  printOutput.setTimeout(3000);

  pinMode(6, INPUT_PULLUP);
  attachInterrupt(6, rpmCounter, FALLING);

  printOutput.restart();
}

void loop() {
  if (printOutput.onExpired()) {
    Serial.print("RPM: ");
    Serial.println(rpmCount);
    printOutput.restart();
  }
}
