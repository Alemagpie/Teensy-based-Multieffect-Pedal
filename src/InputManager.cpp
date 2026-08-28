#include "InputManager.h"

void InputManager::begin() {
    xUp = xDown = leftUp = leftDown = rightUp = rightDown = centerUp = centerDown = 0;
    xPressed = lPressed = rPressed = cPressed = 0;
}

void InputManager::update() {
    X_Button.update();
    L_Button.update();
    C_Button.update();
    R_Button.update();

    if(X_Button.risingEdge()) xUp = millis();
    if(X_Button.fallingEdge()) xDown = millis();

    if(L_Button.risingEdge()) leftUp = millis();
    if(L_Button.fallingEdge()) leftDown = millis();

    if(C_Button.risingEdge()) centerUp = millis();
    if(C_Button.fallingEdge()) centerDown = millis();

    if(R_Button.risingEdge()) rightUp = millis();
    if(R_Button.fallingEdge()) rightDown = millis();
}