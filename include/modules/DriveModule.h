#ifndef DRIVE_MODULE
#define DRIVE_MODULE

#include "Module.h"

class DriveModule : Module {
    public:
    DriveModule() { gain = bias = 0; }

    DriveModule(int16_t g, int16_t b) {
        gain = g;
        bias = b;
    }

    inline void process(int16_t &value) override {
        value = Utility::fastTanh(saturate16( (int32_t)gain * value + bias ));
    }

    inline void reset() override { gain = bias = 0; }

    inline void setGain(int16_t value) { gain = value; }
    inline void setBias(int16_t value) { bias = value; }

    private:
    int16_t gain;
    int16_t bias;
};


#endif