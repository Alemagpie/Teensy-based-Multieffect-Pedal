#ifndef OD_MODULE
#define OD_MODULE

#include <Arduino.h>
#include <AudioStream.h> 
#include "Utility.h"

class DriveModule {
    DriveModule() { gain = bias = 0; }

    DriveModule(int16_t g, int16_t b) {
        gain = g;
        bias = b;
    }

    inline void process(int16_t &value) {
        value = Utility::fastTanh(saturate16( (int32_t)gain * value + bias ));
    }

    inline void setGain(int16_t value) { gain = value; }
    inline void setBias(int16_t value) { bias = value; }

    int16_t gain;
    int16_t bias;
};


#endif