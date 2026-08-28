#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include <Bounce.h>

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

#define LONG_PRESS_TIME_MS 1000
#define HOLD_PRESS_TIME_MS 2000

class InputManager {
    public:
    void begin();
    void update();

    void leftShortPressed();
    void rightShortPressed();
    void centerShortPressed();


    uint16_t getParam(uint8_t index);

    private:
    bool isModifying = false;
    Bounce X_Button = Bounce(MODIFY_B_PIN, 15);
    Bounce L_Button = Bounce(EFFECT_L, 30);
    Bounce R_Button = Bounce(EFFECT_R, 30);
    Bounce C_Button = Bounce(EFFECT_SWITCH, 30);

    uint32_t xUp, xDown, leftUp, leftDown, rightUp, rightDown, centerUp, centerDown;
    bool xPressed, lPressed, rPressed, cPressed;
};

#endif