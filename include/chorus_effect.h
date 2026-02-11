#ifndef CHORUS_EFFECT_H
#define CHORUS_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "LFO.h"
#include "LowPassFilter.h"

#define CHORUS_BUFFER_LENGHT  (20*AUDIO_BLOCK_SAMPLES)
#define MAX_CHORUS_VOICES 4

class ChorusEffect : public AudioStream, public EffectAdapter {
    public:
    ChorusEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1, 5), CustomRange(1, 150), CustomRange(1, 5), CustomRange(0, 255)}) {
        effectName = "Chorus";
        paramName = {"RT", "DPT", "VOC", "MIX"};

        freq = 5;
        depth = 193;
        voices = 2;
        mix = 128;

        //setup 4 bipolar sine LFO and randomize their phase
        for (int i = 0; i < MAX_CHORUS_VOICES; i++)
        {
            lfos[i].setAmplitude(1);
            lfos[i].setFrequency(5);
            lfos[i].setShape(0);
            lfos[i].setMode(0);
            lfos[i].setPhase(i * 90);
        }

        memset(sampleQueue, 0, sizeof(sampleQueue));
	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint16_t writeIndex = 0;
    uint16_t baseDelay = 100; //base distance (without LFO) in samples between read and write index
    int16_t* inputSamplePtr;
    int16_t* lfoSamplePtrs[MAX_CHORUS_VOICES];

    bool active = false;
    LFO lfos[MAX_CHORUS_VOICES];    //the LFO offset goes from 1 to 5 ms (roughly 44 to 220 samples)

    uint16_t freq;
    uint16_t depth;
    uint16_t voices;
    uint16_t mix;

    int16_t sampleQueue[CHORUS_BUFFER_LENGHT];
	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif