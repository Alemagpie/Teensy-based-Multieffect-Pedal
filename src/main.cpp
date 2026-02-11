#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <vector>
#include "EffectAdapter.h"
#include "ScreenManager.h"

#include "distortion_effect.h"
#include "tremolo_effect.h"
#include "bitcrusher_effect.h"
#include "vibrato_effect.h"

#define PARAM1_PIN A14
#define PARAM2_PIN A15
#define PARAM3_PIN A16
#define PARAM4_PIN A17
#define MODIFY_B_PIN 33 
#define MODIFY_L_PIN 34  
#define EFFECT_L 28
#define EFFECT_SWITCH 29
#define EFFECT_R 30

ScreenManager sm;

AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;


DistortionEffect dist;
TremoloEffect trem;
BitCrusherEffect bitcrush;
VibratoEffect vib;

EffectAdapter* effects[] = {&vib, &bitcrush, &dist, &trem};
int currentEffect = 0;
int effectCount = 4;

// Debug
AudioSynthWaveformSine s1;
//AudioConnection s1e1(s1, 0, *(effects[0]->getAudioStreamComponent()), 0);

AudioConnection ie1(input, 0, *(effects[0]->getAudioStreamComponent()), 0);
AudioConnection e1e2(*(effects[0]->getAudioStreamComponent()), 0, *(effects[1]->getAudioStreamComponent()), 0);
AudioConnection e2e3(*(effects[1]->getAudioStreamComponent()), 0, *(effects[2]->getAudioStreamComponent()), 0);
AudioConnection e3e4(*(effects[2]->getAudioStreamComponent()), 0, *(effects[3]->getAudioStreamComponent()), 0);
AudioConnection e4o(*(effects[3]->getAudioStreamComponent()), 0, output, 0);


bool isModifying = false;
Bounce modifyButton = Bounce(MODIFY_B_PIN, 15);
Bounce L_EffectButton = Bounce(EFFECT_L, 15);
Bounce R_EffectButton = Bounce(EFFECT_R, 15);
Bounce effectSwitchButton = Bounce(EFFECT_SWITCH, 15);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 100;

uint16_t readParameter(int index);
void toggleModify();
void initAudioBoard();
void initPins();
void onEffectChange();

void setup() {
  initAudioBoard();
  initPins();
  sm.start();
  delay(1000);
  onEffectChange();
}

void loop() {
  //Reads state of modify button
  modifyButton.update();
  if(modifyButton.risingEdge()) {
    toggleModify();
  }

  L_EffectButton.update();
  if(L_EffectButton.risingEdge()) {
    currentEffect--;

    if(currentEffect < 0) {
      currentEffect = effectCount - 1;  //last effect
    }

    onEffectChange();
  }

  R_EffectButton.update();
  if(R_EffectButton.risingEdge()) {
    currentEffect++;

    if(currentEffect > effectCount - 1) {
      currentEffect = 0;  //1st effect
    }

    onEffectChange();
  }

  effectSwitchButton.update();
  if(effectSwitchButton.risingEdge()) {
    effects[currentEffect] -> toggleEnable();
  }

  //set modify led
  digitalWrite(MODIFY_L_PIN, isModifying ? HIGH : LOW);

  if(isModifying) {
    unsigned long now = millis();
    if (now - lastUpdate >= updateInterval) {
        lastUpdate = now;

        effects[currentEffect] -> setParamLevel(0, readParameter(0));
        effects[currentEffect] -> setParamLevel(1, readParameter(1));   
        effects[currentEffect] -> setParamLevel(2, readParameter(2));
        effects[currentEffect] -> setParamLevel(3, readParameter(3));
    }
  }
}

void initAudioBoard() {
  AudioMemory(12);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8); 

  s1.amplitude(0.5);
  s1.frequency(200);
}

void initPins() {
  pinMode(MODIFY_B_PIN, INPUT_PULLDOWN);
  pinMode(MODIFY_L_PIN, OUTPUT);

  pinMode(PARAM1_PIN, INPUT);
  pinMode(PARAM2_PIN, INPUT);
  pinMode(PARAM3_PIN, INPUT);
  pinMode(PARAM4_PIN, INPUT);

  pinMode(EFFECT_L, INPUT_PULLDOWN);
  pinMode(EFFECT_SWITCH, INPUT_PULLDOWN);
  pinMode(EFFECT_R, INPUT_PULLDOWN);
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

void onEffectChange() {
    sm.drawEffect(currentEffect + 1, effects[currentEffect]->getEffectName(), *(effects[currentEffect]->getParamNames()));
}