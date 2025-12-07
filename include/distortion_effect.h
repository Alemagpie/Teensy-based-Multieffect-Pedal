#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "Utility.h"
#include "HighPassFilter.h"
#include "LowPassFilter.h"

class DistortionEffect : public AudioStream, public EffectAdapter
{
    public:
    DistortionEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 12), CustomRange(0, 7000), CustomRange(2500, 8000)}) {
        gain = 10;
        bias = 0;
        lpF.setCutoff(8000);
        hpF.setCutoff(10);
    }

    void setParamLevel(int index, uint16_t level) override;
    void init(float p1, float p2, float p3, float p4) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    HighPassFilter hpF;
    LowPassFilter lpF;

    int16_t gain;
    int16_t bias;

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(int16_t &value);
};

#endif