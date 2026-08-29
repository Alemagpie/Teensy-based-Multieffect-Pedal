#ifndef INPUT_STATE
#define INPUT_STATE

#include <Arduino.h>

enum class PressType : uint8_t { NONE, SHORT, LONG, HOLD };

struct InputState {
    //Left, Center, Right, Extra
    PressType switches[4] = { PressType::NONE, PressType::NONE, PressType::NONE, PressType::NONE };
    uint16_t params[4] = {0, 0, 0, 0};
    //float tapTempo;
};

#endif