#ifndef BITCRUSHER_EFFECT_H
#define BITCRUSHER_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "Utility.h"
#include "HighPassFilter.h"
#include "LowPassFilter.h"
#include "UniformQuantizer.h"

class BitCrusherEffect : public AudioStream, public EffectAdapter{
    public:
    BitCrusherEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 14), CustomRange(1, 32), CustomRange(0, 255), CustomRange(1000, 12000)}) {
        bitRes = 8;
        quant.setResolution(bitRes);
        lp.setCutoff(8000);
        effectName = "Bitcrusher";
        paramName = {"RES", "DWS", "MIX", "LPF"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint8_t bitRes;
    uint8_t downSample = 1;
    uint8_t mix = 128;
    audio_block_t *inputQueueArray[1];
    LowPassFilter lp;

    UniformQuantizer quant;

    //number of samples outside a cycle to hold
    uint8_t holdSampleCount = 0;
    //sample to hold
    int16_t holdSample = 0;

    virtual void update(void);
};

#endif