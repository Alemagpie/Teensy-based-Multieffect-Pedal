#ifndef SAH_MODULE
#define SAH_MODULE

#include <Arduino.h>
#include <dspinst.h>
#include <AudioStream.h> 

class SampleAndHoldModule {
    SampleAndHoldModule() { hold = 1; }
    SampleAndHoldModule(uint8_t h) { setHold(h); }

    void process(int16_t &value);
    inline void setHold(uint8_t h) { hold = h != 0 ? h : 1; }

    private:
    uint8_t hold;
    uint8_t holdSampleCount;
    uint8_t remainingSamples;
    int16_t holdSample = 0;
};

#endif