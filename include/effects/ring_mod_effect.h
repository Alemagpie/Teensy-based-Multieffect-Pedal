#ifndef RING_MOD_H
#define RING_MOD_H

#include <Arduino.h>
#include <AudioStream.h>
#include "EffectAdapter.h"
#include "Utility.h"

#include "modules/LFOModule.h"
#include "modules/MixerModule.h"

class RingModulatorEffect : public AudioStream, public EffectAdapter {
    public:
    RingModulatorEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(5, 1500), CustomRange(0, 6), CustomRange(0, 32767), CustomRange(0, 256)}) {
        ID = 12;

        freq = 450;
        lfo_m.setFrequency(freq);
        lfo_m.setMode(0);   //Bipolar
        lfo_m.setShape(0);
        mx_m.setGain(0, 128);
        mx_m.setGain(1, 128);

        effectName = "Ring modulator";
        paramName = {"FRQ", "SHP", "DPT", "MIX"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    LFOModule lfo_m;
    MixerModule mx_m;

    float freq;
    uint8_t shape;
    uint16_t depth;
    uint8_t mix;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr, *lfoSamplePtr;
    virtual void update(void);
};

#endif