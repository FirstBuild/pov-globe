/*
 * Project PhotonicGlobe
 * Description:
 * Author:
 * Date:
 */

//#include <RBD_Timer.h>
#include <FastLED.h>
#include "bitArrayImages.h"

#define hallEffectSensorPin 3
#define CLK 14
#define DATA 7

#define NUM_LEDS 260
#define BRIGHTNESS 255

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 150
#define COOLING  100

//RBD::Timer printOutput;
CRGB leds[NUM_LEDS];

uint32_t color = 0xFF2200;      // 'On' color (starts orange)

void rpmCounter();
void draw_column();
void modifyTime();

unsigned long startTimeOfRotation = micros();
unsigned long timeLengthOfRotation = micros() - startTimeOfRotation;
unsigned long timePerColumn = timeLengthOfRotation / 360;
volatile bool update = false;
int cur_col = 0;
int pos = 0;
int imageOffset = 0;
int sideOffset = 0;

void setup() {
  FastLED.addLeds<APA102,DATA,CLK, BGR,DATA_RATE_MHZ(17)>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
  FastLED.show();

//  Serial.begin(38400);

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
      timeLengthOfRotation = (micros() - startTimeOfRotation);
      timePerColumn = timeLengthOfRotation / 360;
      startTimeOfRotation = micros();
      cli();
      update = false;
      sei();      
      cur_col = -25;
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
  if (cur_col < (28 - imageOffset) && cur_col > (132 - imageOffset)) {
    FastLED.clear();
    FastLED.show();
    return;
  }

  for (int i = 0; i < 20; i++) {
    if (cur_col > (44 - imageOffset) && cur_col < (116 - imageOffset)) {

      // Herores array has 73 columns
        int heroCount = cur_col - 43;
//        heroCount %= 73;
      if (heroesText[i][heroCount]) {
        leds[(96 + sideOffset) - i] = color;
      } else {
        leds[(96 + sideOffset) - i] = CRGB::Black;
      }
    } else {
       leds[(96 + sideOffset) - i] = CRGB::Black;
    }
    
    if (cur_col > (69 - imageOffset) && cur_col < (91 - imageOffset)) {
      // Of array has 23 columns
        int ofCount = cur_col - 69;
        if (ofText[i][ofCount]) {
          leds[(75 + sideOffset) - i] = color;
        } else {
          leds[(75 + sideOffset) - i] = CRGB::Black;
        }
    } else {
       leds[(75 + sideOffset)  - i] = CRGB::Black;
    }
  
    if (cur_col > (29 - imageOffset) && cur_col < (131 - imageOffset)) {
      //  FirstBuild array has 103 columns
        int fbCount = cur_col - 29;
//        fbCount %= 103;
        if (firstbuildText[i][fbCount]) {
          leds[(52 + sideOffset)  - i] = color;
        } else {
          leds[(52 + sideOffset) - i] = CRGB::Black;
        }
    } else {
       leds[(52 + sideOffset) - i] = CRGB::Black;
    }
  }
  FastLED.show();
}
