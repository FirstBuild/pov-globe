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
#define CLK A5
#define USE_HW_SPI A3

#define NUM_LEDS 260
#define BRIGHTNESS 150
#define FRAMES_PER_SECOND 60

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 150
#define COOLING  100

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

RBD::Timer printOutput;
Adafruit_DotStar strip = Adafruit_DotStar(NUM_LEDS);

uint32_t color = 0xFF0022;      // 'On' color (starts orange)

void rpmCounter();
void draw_column();
void modifyTime();

unsigned long startTimeOfRotation = micros();
unsigned long timeLengthOfRotation = micros() - startTimeOfRotation;
unsigned long timePerColumn = timeLengthOfRotation / 360;
volatile bool update = false;
int cur_col = 0;
int pos = 0;

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  pinMode(hallEffectSensorPin, INPUT_PULLUP);
  attachInterrupt(hallEffectSensorPin, rpmCounter, FALLING);
}

void rpmCounter() {
  update = true;
}

void loop() {
  //while (cur_col != pos) {
    draw_column();
    
    static unsigned long tracker = micros();
    if ((micros() - tracker) > timePerColumn) {
      if (++cur_col == 360) {
        cur_col = 0;
      }
      tracker = micros();
    } 

  //while(startTimeOfRotation < timePerColumn) {} 

  //modifyTime();
  //while (startTimeOfRotation >= timePerColumn) {
  //  modifyTime();
  //}

  //if (pos == 0) {
    if (update) {
      update = false;
      timeLengthOfRotation = micros() - startTimeOfRotation;
      timePerColumn = timeLengthOfRotation / 360;
      startTimeOfRotation = micros();
      cur_col = 0;
      //pos = 0;
    }
  //}
}

void modifyTime() {
    startTimeOfRotation -= timePerColumn;
    if (++pos == 360) {
      pos = 0;
    }
}

void draw_column() {
  if (cur_col > 54) {
    strip.clear();
  }
  for (int i = 0; i < 18; i++) {
    if (largeHelloText[i][cur_col]) {
      strip.setPixelColor(i + 55, color);
    } else {
      strip.setPixelColor(i + 55, 0);
    }
  }
  strip.show();
}
