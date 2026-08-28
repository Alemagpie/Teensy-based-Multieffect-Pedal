#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#define EXTRA_LED_PIN 34

#include "InputState.h"
#include "UIManager.h"
#include "EffectChainManager.h"

class StateMachine {
    public: 
    StateMachine(InputState& in, UIManager& u, EffectChainManager& e) 
        : input(in), ui(u), chain(e) {};

    enum SystemState {
        PLAY,
        MODIFY,
        EDIT
    };
    SystemState getState() { return currentState; }
    void handleInput();

    private:
    SystemState currentState;
    InputState& input;
    UIManager& ui;
    EffectChainManager& chain;
};

#endif