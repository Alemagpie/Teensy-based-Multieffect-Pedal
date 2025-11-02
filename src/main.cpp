#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <vector>

#define PARAM1_PIN A10
#define PARAM2_PIN A11
#define MODIFY_B_PIN A12

void _init();
void createConnections();
void toggleModify();
float readParameter(int index);
void selectNextEffect();
void selectPreviousEffect();
void toggleEffect(int index);


AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;

const int effectsCount = 2;
std::vector<bool> activeEffects(effectsCount);

//compile-time connections (m = mixer, e = effect, i = input, o = output)
//direct connections always go to channel 0 of mixers, effects to channel 1
AudioMixer4 mixers[effectsCount];
AudioConnection im1_patchCord(input, 0, mixers[0], 0); 
AudioConnection m1m2_patchCord(mixers[0], 0, mixers[1], 0);
AudioConnection m2o_patchCord(mixers[1], 0, output, 0);
//run-time connections
AudioConnection ie1_patchCord, e1m1_patchCord, m1e2_patchCord, e2m2_patchCord;
/*
         +--Effect1-+         +--Effect2--+
  input--+----------+--mixer1--+-----------+--mixer2--output
*/

Bounce modifyButton = Bounce(MODIFY_B_PIN, 15);

//Holds pointers to the 5 current effects
std::vector<EffectAdapter*> effects(effectsCount);
bool isModifying;
uint8_t currentEffect = 0;

AudioEffectChorus chorus; //example

void setup() {
  _init();

  for(int i = 0; i < effectsCount; i++) {
    activeEffects[i] = false;
    //clean passthru
    mixers[i].gain(1, 0.0);
    mixers[i].gain(0, 1.0);
  }

  effects[0] = &chorus; //example
}

void loop() {
  //If is in modifying state set parameter leves
  if(isModifying) {
    effects[currentEffect]->setParamLevel(0, readParameter(0));
    effects[currentEffect]->setParamLevel(1, readParameter(1));
  }

  //Reads state of modify button
  modifyButton.update();

  if(modifyButton.fallingEdge()) {
    toggleModify();
  }
}

//-------------------------------------------

void _init() {
  AudioMemory(12);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8);  

  pinMode(MODIFY_B_PIN, INPUT_PULLUP);
  pinMode(PARAM1_PIN, INPUT);
  pinMode(PARAM2_PIN, INPUT);

  createConnections();
}

void createConnections() {
  AudioNoInterrupts();
  //ie1_patchCord.connect(input, 0, dynamic_cast<AudioStream&>(*effects[0]), 0);
  //e1m1_patchCord.connect(dynamic_cast<AudioStream&>(*effects[0]), 0, mixers[0], 1);
  //m1e2_patchCord.connect(mixers[0], 0, dynamic_cast<AudioStream&>(*effects[1]), 0);
  //e2m2_patchCord.connect(dynamic_cast<AudioStream&>(*effects[1]), 0, mixers[1], 1);
  AudioInterrupts();
}

float readParameter(int index) {
  if(index >= 0 && index < 2) {
    switch(index) {
      case 0:
      return analogRead(PARAM1_PIN)/1023;
      break;

      case 1:
      return analogRead(PARAM2_PIN)/1023;
      break;
    }
  }
  
}

//allows/stops the ability to change parameters
void toggleModify() {
  isModifying = !isModifying;
}

void selectNextEffect() {
  currentEffect = (currentEffect == effectsCount - 1) ? 0 : currentEffect + 1;
}

void selectPreviousEffect() {
  currentEffect = (currentEffect == 0) ? effectsCount - 1 : currentEffect - 1;
}

//turns on/off selected effect
void toggleEffect(int index) {
  if(index < 0 || index > effectsCount - 1) {
    return;
  }

  if(activeEffects[index]) {
    mixers[index].gain(1, 0.0);
    mixers[index].gain(0, 1.0);
  } else {
    mixers[index].gain(0, 0.0);
    mixers[index].gain(1, 1.0);
  }
}


/*
Effects to convert:
 - bitcrusher
 - combine
 - delay
 - envelope
 - fade
 - flanger
 - granular
 - tremolo (multiply)
 - wavefolder

Effects to create:
 - distortion
 - eq
*/