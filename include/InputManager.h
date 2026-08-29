#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Bounce.h>
#include "InputState.h"

#define PARAM1_PIN A14
#define PARAM2_PIN A15
#define PARAM3_PIN A16
#define PARAM4_PIN A17
#define SWITCH_X 33 
#define MODIFY_L_PIN 34  
#define SWITCH_L 28
#define SWITCH_C 29
#define SWITCH_R 30
#define MODE1_PIN 25
#define MODE2_PIN 26

#define LONG_PRESS_TIME_MS 1000
#define HOLD_PRESS_TIME_MS 2000

class InputManager {
    public:
    void begin();
    //Has to be called once per frame, updates the shared input state
    void update();

    InputState& getInputState() { return inputState; }

    private:
    bool isModifying = false;

    Bounce buttons[4] = {
        Bounce(SWITCH_L, 30), 
        Bounce(SWITCH_C, 30), 
        Bounce(SWITCH_R, 30), 
        Bounce(SWITCH_X, 15)
    };
    
    //In order: L rising edge, L falling edge, C, R, X
    uint32_t edges[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    InputState inputState;
};



#endif