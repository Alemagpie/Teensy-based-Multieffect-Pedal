#ifndef GAIN_MODULE
#define GAIN_MODULE

#include "Module.h"

class GainModule : Module {
    GainModule() { gain = 0; }
    GainModule(int16_t g) { gain = g; }

    inline void process(int16_t &gain, int16_t &value) {
        signed_saturate_rshift(gain * value, 16, 15);
    }

    inline void reset() override { gain = 0; }

    inline void setGain(int16_t value) { gain = value; }

    private:
    int16_t gain;
};

#endif