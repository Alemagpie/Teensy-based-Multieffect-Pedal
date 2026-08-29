#include <Arduino.h>
#include <Audio.h>
#include "EffectChainManager.h"
#include "InputManager.h"
#include "PresetManager.h"
#include "StateMachine.h"
#include "UIManager.h"

//Hardware
AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;

//Managers
InputManager in_man;
StateMachine st_mac;
UIManager ui_man;
EffectChainManager effch_man;
PresetManager pr_man;

void setup() {
    effch_man.setup(&input, &output, &sgtl5000);
    in_man.begin();
    ui_man.start();
    pr_man.setup(&effch_man);
    st_mac.setup(&in_man.getInputState(), &ui_man, &effch_man, &pr_man);
}

void loop() {
    in_man.update();
    st_mac.handleInput();
}