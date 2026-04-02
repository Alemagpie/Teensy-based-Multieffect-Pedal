#ifndef AUDIO_MODULE
#define AUDIO_MODULE

#include <Arduino.h>
#include <AudioStream.h> 
#include "Utility.h"

class Module {
    public:
    virtual inline void process(int16_t &value) {};
    virtual inline void reset() {};

    protected:
    bool active = true;
};

#endif