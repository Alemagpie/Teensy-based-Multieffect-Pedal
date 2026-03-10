#ifndef GAIN_MODULE
#define GAIN_MODULE

#include "Module.h"

class GainModule : Module {
    public:
    GainModule() { gain = 0; }
    GainModule(int16_t g) { gain = g; }

    inline void process(int16_t &value) override {
        value = signed_saturate_rshift(gain * value, 16, 15);
    }

    //Used for tremolo, to avoid setting a different gain value for each sample
    inline void process(int16_t &g, int16_t &value) {
        value = signed_saturate_rshift(g * value, 16, 15);
    }

    inline void reset() override { gain = 0; }

    inline void setGain(int16_t value) { gain = value; }

    private:
    int16_t gain;
};

#endif