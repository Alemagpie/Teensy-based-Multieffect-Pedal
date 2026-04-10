#ifndef CHORUS_EFFECT_H
#define CHORUS_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/LFOModule.h"
#include "modules/DelayLineModule.h"
#include "modules/MixerModule.h"

#define MAX_CHORUS_VOICES 4

class ChorusEffect : public AudioStream, public EffectAdapter {
    public:
    ChorusEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1, 5), CustomRange(1, 30), CustomRange(1, 5), CustomRange(0, 255)}) {
        ID = 5;

        freq = 2;
        depth = 10;
        voices = 2;
        mix = 128;
        mx_m.setGain(0, 128);
        mx_m.setGain(1, 128);

        //setup 4 bipolar sine LFO with differente phases
        for (int i = 0; i < MAX_CHORUS_VOICES; i++)
        {
            lfos_m[i].setAmplitude(1);
            lfos_m[i].setFrequency(2.5f + random(-2, +2));
            lfos_m[i].setShape(0);
            lfos_m[i].setMode(0);
            lfos_m[i].setPhase(i * 90);
        }

        effectName = "Chorus";
        paramName = {"RT", "DPT", "VOC", "MIX"}; 
	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint16_t writeIndex = 0;
    uint16_t baseDelay = 150; //base distance (without LFO) in samples between read and write index
    int16_t* inputSamplePtr;
    int16_t* lfoSamplePtrs[MAX_CHORUS_VOICES];
    int16_t voicesOffset = 30;  //distance between each tap

    bool active = false;
    LFOModule lfos_m[MAX_CHORUS_VOICES];    //the LFO offset goes from 1 to 5 ms (roughly 44 to 220 samples)
    DelayLineModule dl_m;
    MixerModule mx_m;

    float freq;
    uint8_t depth;
    uint8_t voices;
    uint8_t mix;

	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif