#include "InputManager.h"

void InputManager::begin() {
    for(int i = 0; i < 8; i++)
        edges[i] = 0;

        
    pinMode(SWITCH_X, INPUT_PULLDOWN);
    pinMode(MODIFY_L_PIN, OUTPUT);

    pinMode(PARAM1_PIN, INPUT);
    pinMode(PARAM2_PIN, INPUT);
    pinMode(PARAM3_PIN, INPUT);
    pinMode(PARAM4_PIN, INPUT);

    pinMode(SWITCH_L, INPUT_PULLDOWN);
    pinMode(SWITCH_C, INPUT_PULLDOWN);
    pinMode(SWITCH_R, INPUT_PULLDOWN);
}

void InputManager::update() {
    uint32_t now = millis();

    inputState.params[0] = analogRead(PARAM1_PIN)*65535/1023;
    inputState.params[1] = analogRead(PARAM2_PIN)*65535/1023;
    inputState.params[2] = analogRead(PARAM3_PIN)*65535/1023;
    inputState.params[3] = analogRead(PARAM4_PIN)*65535/1023;

    for(int i = 0; i < 4; i++) {
        buttons[i].update();

        if(buttons[i].risingEdge())
            edges[2*i] = millis();

        if(buttons[i].fallingEdge()) 
            edges[2*i+1] = millis();


        if(edges[2*i] != 0 && edges[2*i+1] != 0) {
            if(inputState.switches[i] != PressType::HOLD) {
                inputState.switches[i] = (edges[2*i+1] - edges[2*i] < LONG_PRESS_TIME_MS) ? PressType::SHORT : PressType::LONG;
            } else {
                inputState.switches[i] = PressType::NONE;
            }

            edges[2*i+1] = edges[2*i] = 0;
        } else {
            if(edges[2*i] != 0 && edges[2*i+1] == 0 && (now - edges[2*i] > HOLD_PRESS_TIME_MS)) {
                inputState.switches[i] = PressType::HOLD;
            } else {
                inputState.switches[i] = PressType::NONE;
            }
        }
    }
}