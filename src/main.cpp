#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <vector>
#include "EffectAdapter.h"

#include "distortion_effect.h"
#include "tremolo_effect.h"

#define PARAM1_PIN A14
#define PARAM2_PIN A15
#define PARAM3_PIN A16
#define PARAM4_PIN A17
#define MODIFY_B_PIN A12
#define MODIFY_L_PIN A13

AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;
AudioSynthWaveformSine s;


DistortionEffect dist;
TremoloEffect trem;
AudioMixer4 mixer;
AudioConnection im(input, 0, mixer, 0);

AudioConnection ie1(input, 0, dist, 0);
AudioConnection e1m(dist, 0, mixer, 1); //effects always on channel 1

/*
AudioConnection ie1(input, 0, trem, 0);
AudioConnection e1m(trem, 0, mixer, 1); //effects always on channel 1
*/
AudioConnection mo(mixer, 0, output, 0);

bool isModifying = false;
Bounce modifyButton = Bounce(MODIFY_B_PIN, 15);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 100;

uint16_t readParameter(int index);
void toggleModify();
void initAudioBoard();
void initPins();

void setup() {
  initAudioBoard();
  initPins();
}

void loop() {
  //Reads state of modify button
  modifyButton.update();
  if(modifyButton.risingEdge()) {
    toggleModify();
  }

  //set modify led
  digitalWrite(MODIFY_L_PIN, isModifying ? HIGH : LOW);

  if(isModifying) {
    unsigned long now = millis();
    if (now - lastUpdate >= updateInterval) {
        lastUpdate = now;
        
        dist.setParamLevel(0, readParameter(0));
        dist.setParamLevel(1, readParameter(1));
        dist.setParamLevel(2, readParameter(2));
        dist.setParamLevel(3, readParameter(3));
        
        /*
        trem.setParamLevel(0, readParameter(0));
        trem.setParamLevel(1, readParameter(1));
        trem.setParamLevel(2, readParameter(2));
        trem.setParamLevel(3, readParameter(3));
        */
    }
  }
}

void initAudioBoard() {
  AudioMemory(12);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8); 
  mixer.gain(0, 0.0f);
  mixer.gain(1, 1.0f);
}

void initPins() {
  pinMode(MODIFY_B_PIN, INPUT_PULLDOWN);
  pinMode(MODIFY_L_PIN, OUTPUT);
  pinMode(PARAM1_PIN, INPUT);
  pinMode(PARAM2_PIN, INPUT);
  pinMode(PARAM3_PIN, INPUT);
  pinMode(PARAM4_PIN, INPUT);
}

//Read parameter pot
uint16_t readParameter(int index) {  
  switch(index) {
      case 0:
      return analogRead(PARAM1_PIN)*65535/1023;
      break;

      case 1:
      return analogRead(PARAM2_PIN)*65535/1023;
      break;

      case 2:
      return analogRead(PARAM3_PIN)*65535/1023;
      break;

      case 3:
      return analogRead(PARAM4_PIN)*65535/1023;
      break;

      default:
      return 0;
      break;
  }
}

//allows/stops the ability to change parameters
void toggleModify() {
  isModifying = !isModifying;
}