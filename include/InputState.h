#ifndef INPUT_STATE
#define INPUT_STATE

#include <Arduino.h>

enum class PressType : uint8_t { NONE, SHORT, LONG, HOLD };

struct InputState {
    //Left, Center, Right, TAP_TEMPO, EXPR assign, Encoder PB
    PressType switches[6] = { PressType::NONE, PressType::NONE, PressType::NONE, PressType::NONE, PressType::NONE, PressType::NONE };
    int8_t encoderDelta = 0;
    uint32_t tapTempo_ms;
};

#endif