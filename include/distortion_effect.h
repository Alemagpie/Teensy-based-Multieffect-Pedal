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
    DistortionEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 12), CustomRange(0, 7000), CustomRange(2500, 8000), CustomRange(0, 32767)}) {
        volume = 16384;
        gain = 7;
        bias = 3500;
        lpF.setCutoff(5250);

        hpF.setCutoff(10); //permanent

        effectName = "Distortion";
        paramName = {"GN ", "BS ", "TRB", "VOL"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    HighPassFilter hpF;
    LowPassFilter lpF;

    int16_t volume;
    int16_t gain;
    int16_t bias;

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(int16_t &value);
};

#endif