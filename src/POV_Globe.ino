/*
 * Project PhotonicGlobe
 * Description:
 * Author:
 * Date:
 */

#include "RBD_Timer.h"
#include "FastLED.h"
FASTLED_USING_NAMESPACE;

#define hallEffectSensorPin 6
#define CLK 4
#define DATA 2

#define NUM_LEDS 130
#define BRIGHTNESS 150

volatile int rotationCounter = 0;
volatile bool update = false;
int rotationsPerSec = 0;
int rotationsPerMin = 0;
long timePerSlice = 0;

RBD::Timer printOutput;
RBD::Timer rapidUpdate;
CRGB leds[NUM_LEDS];

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

void rpmCounter() {
  update=true;
}

void setup() {
  printOutput.setTimeout(1000);

  //  On the Photon, SPI is implemented via SOFTWARE not HARDWARE.
  //FastLED.addLeds<APA102, DATA, CLK, EOrder::BGR, DATA_RATE_MHZ(14)>(leds, NUM_LEDS);

  FastLED.addLeds<APA102, DATA, CLK>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  pinMode(hallEffectSensorPin, INPUT_PULLUP);
  attachInterrupt(hallEffectSensorPin, rpmCounter, FALLING);
  printOutput.restart();
}

void loop() {
  //hallEffectHandler();
  static unsigned long test = micros();
  static int count = 0;

  if (printOutput.onExpired()) {
      timePerSlice = ((micros() - test) * 1000) / 360;
      test = micros();
      printOutput.setTimeout(timePerSlice);
      printOutput.restart();
  }
  showLights(count);
  count++;

  if (update) {
    update = false;
    count = 0;
  }

}


void showLights(int index) {
  if (index == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
  } else {
    FastLED.clear();
  }
  FastLED.show();
}


void roundTest() {
  if (printOutput.onExpired()) {
    static unsigned int tracker = 0;
    pattern(tracker);
    tracker++;
    if (tracker == NUM_LEDS) {
      FastLED.clear();
      FastLED.show();
      tracker = 0;
    }
    printOutput.restart();
  }
}

void pattern(int test) {
  leds[test] = CRGB::Red;
  FastLED.show();
}

void hallEffectHandler() {
  if (printOutput.onExpired()) {
    noInterrupts();
    rotationsPerMin = rotationCounter * 60;
    rotationsPerSec = rotationCounter;

    timePerSlice = (int) floor((1.0 / rotationsPerSec) * 1000) ;
    // Rotations per minute
    Serial.print("RPM: ");
    Serial.println(rotationsPerMin);

    // Rotations per second
    Serial.print("RPS: ");
    Serial.println(rotationsPerSec);

    rotationCounter = 0;


    printOutput.restart();
    interrupts();
  }
}
