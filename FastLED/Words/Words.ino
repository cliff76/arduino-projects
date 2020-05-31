//include relevant library
#include <Arduino.h>
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 50
//define pwm data pin
#define DATA_PIN 3

#define ALL_LIGHTS -1
// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB colors[3] = {CRGB::Red, CRGB::Green, CRGB::Blue};
int DELAY = 50;
String letterIndex = "--ABC-DEFG-H------Q-P-O-NM-LKJI-----RSTU-VWX-Y-Z--";

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void cycle(CRGB whichColor) {
  for(int i=0; i< NUM_LEDS; i++){
    // Turn the LED on, then pause
    leds[i] = whichColor;
    FastLED.show();
    delay(DELAY);
    // turn off the LED, then pause
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(DELAY);
  }
}

void colorAll(CRGB whichColor) {
  for(int i=0; i< NUM_LEDS; i++){
    // Turn the LED on, then pause
    leds[i] = whichColor;
  }
    FastLED.show();
}

void fade(int stripIdx, int colorIdx) {
  for(int i=0; i < 256; i++ ) {
    uint8_t result = 255-ease8InOutCubic(i);
    CRGB theColor;
    switch(colorIdx) {
      case 1:
        theColor = CRGB(0, result, 0);
        break;
      case 2:
        theColor = CRGB(0, 0, result);
        break;
      default:
        theColor = CRGB(result, 0, 0);
        break;
    }
    if(stripIdx < 0) {
      colorAll(theColor);
    } else {
      leds[stripIdx] = theColor;
      FastLED.show();
    }
    Serial.print("result = ");
    Serial.print(result);
    Serial.print("\n");
    delay(10);
  }
}

void displayLetter(char theLetter, int letterDuration) {
  if(isSpace(theLetter)) {
    return;
  }
  int lightIndex = letterIndex.indexOf(String(theLetter));
  fade(lightIndex,0);
}

void writeWords(String theword,int letterDuration)
{
  int stringLen = theword.length()+1;
  char char_array[stringLen];
  theword.toCharArray(char_array,stringLen);
  for(int i=0;i<stringLen-1;i++)
  {
    displayLetter(char_array[i],letterDuration);
    delay(1000);
  }
}

void loop() {
  fade(ALL_LIGHTS, 2);
  writeWords("HELLO THIS IS WILL", 1);
  delay(1000);
}
