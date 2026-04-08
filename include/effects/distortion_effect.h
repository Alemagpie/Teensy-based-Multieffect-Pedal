#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/DriveModule.h"
#include "modules/GainModule.h"
#include "modules/HighPassFilterModule.h"
#include "modules/LowPassFilterModule.h"

class DistortionEffect : public AudioStream, public EffectAdapter
{
    public:
    DistortionEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 12), CustomRange(0, 16380), CustomRange(1000, 8000), CustomRange(0, 32767)}) {
        ID = 1;

        volume = 16384;
        gain = 7;
        bias = 0;
        d_m.setGain(gain);
        d_m.setBias(bias);
        g_m.setGain(volume);
        hp_m.setCutoff(10);
        lp_m.setCutoff(5250);

        effectName = "Distortion";
        paramName = {"GN ", "BS ", "TRB", "VOL"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:

    DriveModule d_m;
    GainModule g_m;
    HighPassFilterModule hp_m;
    LowPassFilterModule lp_m;

    int16_t volume;
    int16_t gain;
    int16_t bias;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;

    virtual void update(void);
    void processSignal(int16_t &value);
};

#endif