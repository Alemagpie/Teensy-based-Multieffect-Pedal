#ifndef GAIN_MODULE
#define GAIN_MODULE

#include <Arduino.h>
#include <AudioStream.h> 
#include "Utility.h"

class GainModule {
    GainModule() { gain = 0; }
    GainModule(int16_t g) { gain = g; }

    inline void process(int16_t &value1, int16_t &value2) {
        signed_saturate_rshift(value1 * value2, 16, 15);
    }

    inline void setGain(int16_t value) { gain = value; }

    private:
    int16_t gain;
};

#endif