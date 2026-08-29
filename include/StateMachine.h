#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#define EXTRA_LED_PIN 34

#define PARAM_UPDATE_TIMER_MS 100

#include "InputState.h"
#include "UIManager.h"
#include "EffectChainManager.h"
#include "PresetManager.h"

class StateMachine {
    public: 
    //Instantiates the other managers' ptrs
    void setup(InputState* in, UIManager* u, EffectChainManager* e, PresetManager* m) {
        input = in;
        ui = u;
        chain = e;
        preset = m;
    }

    enum SystemState {
        PLAY,
        EDIT
    };
    SystemState getState() { return currentState; }
    //Handles state transitions based on the values in the shared input state
    void handleInput();

    private:
    SystemState currentState = SystemState::PLAY;
    bool isModifying = false;
    InputState* input;
    UIManager* ui;
    EffectChainManager* chain;
    PresetManager* preset;

    unsigned long lastUpdate = 0;
};

#endif