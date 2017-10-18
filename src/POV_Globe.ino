/*
 * Project PhotonicGlobe
 * Description:
 * Author:
 * Date:
 */

#include "RBD_Timer.h"
#include "dotstar.h"
#include "bitArrayImages.h"

#define hallEffectSensorPin 6
#define CLK 4
#define DATA 2

#define NUM_LEDS 260
#define BRIGHTNESS 150
#define FRAMES_PER_SECOND 60

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 150
#define COOLING  100

volatile int rotationCounter = 0;
volatile bool update = false;
int rotationsPerSec = 0;
int rotationsPerMin = 0;
long timePerSlice = 0;

RBD::Timer printOutput;
RBD::Timer rapidUpdate;

Adafruit_DotStar strip = Adafruit_DotStar(NUM_LEDS, DATA , CLK);

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

int      head  = 0, tail = -260; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0022;      // 'On' color (starts orange)

void rpmCounter() {
  update=true;
}

void setup() {
  printOutput.setTimeout(1000);

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  pinMode(hallEffectSensorPin, INPUT_PULLUP);
  attachInterrupt(hallEffectSensorPin, rpmCounter, FALLING);
  printOutput.restart();
}

void loop() {
  //hallEffectHandler();
  //static unsigned long test = micros();
  //static int count = 0;

  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUM_LEDS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0x00A5FF;             //   Yes, reset to red
  }
  if(++tail >= NUM_LEDS) tail = 0; // Increment, reset tail index

  //if (printOutput.onExpired()) {
  //    timePerSlice = ((micros() - test) * 1000) / 360;
  //    test = micros();
  //    printOutput.setTimeout(timePerSlice);
  //    printOutput.restart();
  //}
  //showLights(count);
  //count++;
//
  //if (update) {
  //  update = false;
  //  count = 0;
  //}
}


void showLights(int index) {

  if (index == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
    }
  } else {
  }
}


void roundTest() {
  if (printOutput.onExpired()) {
    static unsigned int tracker = 0;
    pattern(tracker);
    tracker++;
    if (tracker == NUM_LEDS) {
      tracker = 0;
    }
    printOutput.restart();
  }
}

void pattern(int test) {
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
