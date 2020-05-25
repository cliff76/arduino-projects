//include relevant library
#include <Arduino.h>
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 50
//define pwm data pin
#define DATA_PIN 3
// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB colors[3] = {CRGB::Red, CRGB::Green, CRGB::Blue};
int DELAY = 50;
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

void loop() {
  for(int i=0; i < 256; i++ ) {
    uint8_t result = 255-ease8InOutCubic(i);
    colorAll(CRGB(result,0,0));
    Serial.print("result = ");
    Serial.print(result);
    Serial.print("\n");
    delay(10);
  }
  delay(1000);
}
//Instead of "leds[0] = CRGB::Red;" you can use the RGB values like so
//leds[0].red = 255;
//leds[0].green = 0;
//leds[0].blue = 0;
