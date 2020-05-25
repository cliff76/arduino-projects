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

const int led = 13;
MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture

void setup()  
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);    // Make LED port writeable
  digitalWrite(led, HIGH); // Blink LED.   
  delay(1);                   
  digitalWrite(led, LOW);  
  delay(1); 

  Serial.println("Initializing...");
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  //*
  // Note: training can only be performed in setup(). 
  // The training functions are "lazy" and only do something if there are changes. 
  // They can be commented out to save memory and startup time once training has been performed.
  recognizer.callSign("Will"); // Train callsign Arduino (may take 20 seconds)
  recognizer.addSentence("Turn on the light"); // Add sentence 1
  recognizer.addSentence("Go dark");            // Add sentence 2
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
  recognizer.ask(F("Hello, my name is Will. Say, turn on the light. Or say, go dark!"));

  //  recognizer.setThreshold(5);      // uncomment and set to a higher value (valid range 2-95) if you have a problems due to a noisy environment.
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
void loop() // run over and over
{
  signed int res=recognizer.poll(); // Get result from MOVI, 0 denotes nothing happened, negative values denote events (see docs)
  if (res==1) {                     // Sentence 1.
    digitalWrite(led, HIGH);        // Turn on LED
    recognizer.say("and there was light!"); // Speak a sentence
  } 
  if (res==2) {                    // Sentence 2 
    digitalWrite(led, LOW);        // Turn off LED
    recognizer.say("and light was no more!"); // Speak a sentence
  }
  // Do more ...
}
