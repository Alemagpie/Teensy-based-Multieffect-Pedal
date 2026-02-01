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
    BitCrusherEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 14), CustomRange(0, 256), CustomRange(0, 5), CustomRange(0, 1)}) {
        bitRes = 5;
        quantizer.setResolution(bitRes);
        effectName = "Bitcrusher";
        paramName = {"RES", "MIX", "DWS", "   "};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint8_t bitRes;
    uint8_t downSample = 1;
    uint8_t mix = 128;
    audio_block_t *inputQueueArray[1];

    UniformQuantizer quantizer;

    virtual void update(void);
};

#endif