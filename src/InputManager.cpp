#include "InputManager.h"

void InputManager::begin() {
    for(int i = 0; i < 8; i++)
        edges[i] = 0;

    //expr btn = par3, clk = md1, dt = md2, enc pb = par1, expr ped = par2/4
        
    //tap tempo btn and led
    pinMode(SWITCH_X, INPUT_PULLDOWN);
    pinMode(MODIFY_L_PIN, OUTPUT);

    pinMode(PARAM1_PIN, INPUT_PULLUP); //encoder push button (par1)
    pinMode(PARAM2_PIN, INPUT_PULLDOWN); //...
    pinMode(PARAM3_PIN, INPUT_PULLDOWN); //expression assign button (par3)
    pinMode(PARAM4_PIN, INPUT); //expression pedal (par2/4)

    //Main switches
    pinMode(SWITCH_L, INPUT_PULLDOWN);
    pinMode(SWITCH_C, INPUT_PULLDOWN);
    pinMode(SWITCH_R, INPUT_PULLDOWN);

    //Encoder digital pins
    pinMode(MODE1_PIN, INPUT_PULLUP);   //encoder clk
    pinMode(MODE2_PIN, INPUT_PULLUP);   //encoder dt

    prev_CLK_state = digitalRead(MODE1_PIN);
}

void InputManager::update() {
    uint32_t now = millis();

    for(int i = 0; i < 6; i++) {
        buttons[i].update();

        if(i == 4) { //encoder push button -> pullup
            if(buttons[i].fallingEdge())
                edges[2*i] = millis();

            if(buttons[i].risingEdge()) 
                edges[2*i+1] = millis();

        } else { //any other button -> pulldown
            if(buttons[i].risingEdge())
                edges[2*i] = millis();

            if(buttons[i].fallingEdge()) 
                edges[2*i+1] = millis();

        }

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

    CLK_state = digitalRead(MODE1_PIN);
    if(CLK_state != prev_CLK_state && CLK_state == LOW) {
       inputState.encoderDelta = (digitalRead(MODE2_PIN) == LOW) ? 1 : -1;
    } else {
        inputState.encoderDelta = 0;
    }
    prev_CLK_state = CLK_state;
}