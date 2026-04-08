#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <vector>
#include "EffectAdapter.h"
#include "ScreenManager.h"
#include "SaveManager.h"

#include "effects/distortion_effect.h"
#include "effects/tremolo_effect.h"
#include "effects/bitcrusher_effect.h"
#include "effects/vibrato_effect.h"
#include "effects/chorus_effect.h"
#include "effects/volume_effect.h"

#define PARAM1_PIN A14
#define PARAM2_PIN A15
#define PARAM3_PIN A16
#define PARAM4_PIN A17
#define MODIFY_B_PIN 33 
#define MODIFY_L_PIN 34  
#define EFFECT_L 28
#define EFFECT_SWITCH 29
#define EFFECT_R 30
#define MODE1_PIN 25
#define MODE2_PIN 26

#define DEBUG false

//Managers
ScreenManager scrMan;
SaveManager svMan;
Save saveIDs;

//Hardware
AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;

//Effects
DistortionEffect dist;
TremoloEffect trem;
BitCrusherEffect bitcrush;
VibratoEffect vib;
ChorusEffect ch;
VolumeEffect vl;

std::vector<EffectAdapter *> allEffects = {&dist, &trem, &bitcrush, &vib, &ch, &vl};
std::vector<EffectAdapter *> availableEffects;

EffectAdapter* effects[5] = {&ch, &vib, &bitcrush, &dist, &trem};
std::vector<bool> isOn = {false, false, false, false, false};
int currentEffect = 0;
int effectCount = 5;

// Debug
AudioSynthWaveformSine s1;

#if DEBUG
AudioConnection s1e1(s1, 0, *(effects[0]->getAudioStreamComponent()), 0);
#else 
AudioConnection ie1(input, 0, *(effects[0]->getAudioStreamComponent()), 0);
#endif

//Connections
AudioConnection e1e2(*(effects[0]->getAudioStreamComponent()), 0, *(effects[1]->getAudioStreamComponent()), 0);
AudioConnection e2e3(*(effects[1]->getAudioStreamComponent()), 0, *(effects[2]->getAudioStreamComponent()), 0);
AudioConnection e3e4(*(effects[2]->getAudioStreamComponent()), 0, *(effects[3]->getAudioStreamComponent()), 0);
AudioConnection e4e5(*(effects[3]->getAudioStreamComponent()), 0, *(effects[4]->getAudioStreamComponent()), 0);
AudioConnection e5o(*(effects[4]->getAudioStreamComponent()), 0, output, 0);


bool isModifying = false;
Bounce modifyButton = Bounce(MODIFY_B_PIN, 15);
Bounce L_EffectButton = Bounce(EFFECT_L, 30);
Bounce R_EffectButton = Bounce(EFFECT_R, 30);
Bounce effectSwitchButton = Bounce(EFFECT_SWITCH, 30);

//Timing stuff
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 100;
const uint16_t SHORT_PRESS_TIME = 1500; //in ms
unsigned long switchDown = 0;
unsigned long switchUp = 0;
bool switchPressed = false;

enum State {
  PLAY,
  MODIFY,
  EDIT
};

State currentState = PLAY;

uint16_t readParameter(int index);
void toggleModify();
void disableModify();
void initAudioBoard();
void initPins();
void onEffectChange();
void loadEffects();
void connectEffects();

void testPerformance() {
  currentEffect = 0;
  for(int i = 0; i < 5; i++) {
    effects[currentEffect] -> toggleEnable();
    delay(1000);
    Serial.println(effects[currentEffect]->getEffectName());
    Serial.print("CPU usage: ");
    Serial.print(AudioProcessorUsage());
    Serial.print("%, max: ");
    Serial.println(AudioProcessorUsageMax());
    Serial.print("RAM usage: ");
    Serial.print(AudioMemoryUsage());
    Serial.print("%, max: ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
    effects[currentEffect] -> toggleEnable();
    currentEffect++;
  }
}

void setup() {
  initAudioBoard();
  initPins();
  scrMan.start();
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
    disableModify();
  }

  R_EffectButton.update();
  if(R_EffectButton.risingEdge()) {
    currentEffect++;

    if(currentEffect > effectCount - 1) {
      currentEffect = 0;  //1st effect
    }

    onEffectChange();
    disableModify();
  }

  effectSwitchButton.update();
  if(effectSwitchButton.risingEdge()) {
    switchDown = millis();
  }

  if(effectSwitchButton.fallingEdge()) {
    switchUp = millis();
    switchPressed = true;
  }

  if(switchPressed) {
    if(switchUp - switchDown <= SHORT_PRESS_TIME) {
      isOn[currentEffect] = effects[currentEffect] -> toggleEnable();
      onEffectChange();
    } else {
      Serial.println("EDIT MODE ON");
    }

    switchPressed = false;
  }

  //set modify led
  digitalWrite(MODIFY_L_PIN, currentState == MODIFY ? HIGH : LOW);

  if(currentState == MODIFY) {
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
  AudioMemory(20);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8); 

  s1.amplitude(0.5);
  s1.frequency(500);


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
  if(currentState != MODIFY) {
    currentState = MODIFY;
  } else {
    currentState = PLAY;
  }
}

void disableModify() {
  currentState = PLAY;
}

void onEffectChange() {
    scrMan.drawUI(
      currentEffect + 1, 
      effects[currentEffect]->getEffectName(), 
      *(effects[currentEffect]->getParamNames()), 
      isOn
    );
}

void loadEffects() {
  __disable_irq();
  svMan.loadSettings(saveIDs);

  /*for(int i = 0; i < 5; i++) {
    uint8_t id = saveIDs.effectIDs[i];
    for(int j = i+1; j < 5; j++) {
      if(saveIDs.effectIDs[j] == id) {
        break;
      }
    }
  }*/

  //Populates available effects vector
  std::copy(allEffects.begin(), (allEffects.begin() + allEffects.size()), availableEffects.begin());
  for(int i = 0; i < 5; i++) {
    uint8_t id = saveIDs.effectIDs[i];
    for(int j = 0; j < availableEffects.size(); j++) {
      if(availableEffects[j]->getEffectID() == id) {
        availableEffects.erase(availableEffects.begin() + j);
        break;
      }
    }
  }

  //Assigns effects
  for(int i = 0; i < 5; i++) {
    uint8_t id = saveIDs.effectIDs[i];
    for(int j = 0; j < allEffects.size(); j++) {
      if(allEffects[j]->getEffectID() == id) {
        effects[i] = allEffects[j];
        break;
      }
    }
  }
  
}

void connectEffects() {
  ie1.connect(input, 0, *(effects[0]->getAudioStreamComponent()), 0);
  e1e2.connect(*(effects[0]->getAudioStreamComponent()), 0, *(effects[1]->getAudioStreamComponent()), 0);
  e2e3.connect(*(effects[1]->getAudioStreamComponent()), 0, *(effects[2]->getAudioStreamComponent()), 0);
  e3e4.connect(*(effects[2]->getAudioStreamComponent()), 0, *(effects[3]->getAudioStreamComponent()), 0);
  e4e5.connect(*(effects[3]->getAudioStreamComponent()), 0, *(effects[4]->getAudioStreamComponent()), 0);
  e5o.connect(*(effects[4]->getAudioStreamComponent()), 0, output, 0);
  __enable_irq();
}