// MOVI(tm) Voice Dialog Shield - Version: Latest 
#include <MOVIShield.h>

/****************************************************************************
 * This is an example for the use of Audeme's MOVI(tm) Voice Control Shield *
 * ----> http://www.audeme.com/MOVI/                                        *
 * This code is inspired and maintained by Audeme but open to change        *
 * and organic development on GITHUB:                                       *
 * ----> https://github.com/audeme/MOVIArduinoAPI                           *
 * Written by Gerald Friedland for Audeme LLC.                              *
 * Contact: fractor@audeme.com                                              *
 * BSD license, all text above must be included in any redistribution.      *
 ****************************************************************************
 *
 * This basic example shows how to use MOVI(tm)'s API to train the call 
 * sign "Arduino" and two sentences. When the sentences are recognized
 * they switch on and off an LED on PIN D13. Many boards have a hardwired 
 * LED on board already connected to D13.
 *
 * Circuitry:
 * LED is pin D13 and GND
 * Arduino UNO R3, MEGA2560 R3, or Arduino Leonardo R3.
 * Connect speaker to MOVI.
 * IMPORTANT: Always use external power supply with MOVI. 
 * 
 * Other Arduino-compatible boards:  
 * Consult MOVI's User Manual before connecting MOVI.
 *
 * If you long-press the button on the MOVI (for a couple seconds), 
 * MOVI will revert back to the call sign and sentences trained here.
 */

#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_PIC32)
#include <SoftwareSerial.h> // This is nice and flexible but only supported on AVR and PIC32 architecture, other boards need to use Serial1 
#endif
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 50
//define pwm data pin
#define DATA_PIN 13
// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB colors[3] = {CRGB::Red, CRGB::Green, CRGB::Blue};
int DELAY = 50;


#define CALL_SIGN "willbyers"
#define ALL_LIGHTS -1
String letterIndex = "--ABC-DEFG-H------Q-P-O-NM-LKJI-----RSTU-VWX-Y-Z--";
const int led = 13;
MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture

void setup()  
{
  Serial.begin(9600);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.println("Initializing...");
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  //*
  // Note: training can only be performed in setup(). 
  // The training functions are "lazy" and only do something if there are changes. 
  // They can be commented out to save memory and startup time once training has been performed.
  recognizer.callSign(CALL_SIGN); // Train callsign Arduino (may take 20 seconds)
  String allTraining[3] = {
    "Are you there",
    "Are you in trouble",
    "talk to me"
  };
  for(int i = 0; i < 3; i++) {
    recognizer.addSentence(allTraining[i]);
  }
  Serial.println("Training...");
  recognizer.train();                           // Train (may take 20seconds) 
  //*/
  if (recognizer.getFirmwareVersion()>=1.1f) { // With 1.1 firmware, we can use the better synthesizer
      recognizer.setSynthesizer(SYNTH_ESPEAK);
//      recognizer.setSynthesizer(SYNTH_PICO); // SYNTH_PICO does not support a male voice
  }
  // Set either with either MALE_VOICE or FEMALE_VOICE constants
  recognizer.setVoiceGender(MALE_VOICE);
  Serial.println("Prompting...");
  String intro = String("Stranger Things. You can ask for ") + String(CALL_SIGN);
  recognizer.ask(intro);

  //  recognizer.setThreshold(5);      // uncomment and set to a higher value (valid range 2-95) if you have a problems due to a noisy environment.
}

void loop() // run over and over
{
  signed int res=recognizer.poll(); // Get result from MOVI, 0 denotes nothing happened, negative values denote events (see docs)
  if (res==1) {                     // Sentence 1.
    recognizer.say("He is here!");
    writeWords("YES",1);
  } else if (res==2) {                    // Sentence 2 
    recognizer.say("You cannot talk to Will!"); // Speak a sentence
    writeWords("HELP ME",1);
  } else if (res==3) {                    // Sentence 2 
    recognizer.say("Haha haha haha haha"); // Speak a sentence
    writeWords("I SEE DEMAGORGONS",1);
  }
  Serial.println("Polling...");
}

void blink()
{
  for(int i=0; i<3; i++) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }
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
    //delay(DELAY);
  }
}

void doCycles(int howMany, CRGB whichColor) {
  for(int i=0; i<howMany; i++) {
    cycle(whichColor);
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
