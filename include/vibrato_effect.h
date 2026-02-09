#ifndef VIBRATO_EFFECT_H
#define VIBRATO_EFFECT_H


#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "LFO.h"
#include "Utility.h"
#include "HighPassFilter.h"
#include "LowPassFilter.h"

#define DELAY_LENGHT  (16*AUDIO_BLOCK_SAMPLES)

//Effect that creates an "echo"
class VibratoEffect : public AudioStream, public EffectAdapter {
    public:
    VibratoEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1, 15), CustomRange(0, 32767), CustomRange(0, 1), CustomRange(0, 1)}) {
		effectName = "Vibrato";
        paramName = {"RT", "DPT", "---", "---"};

        //setup bipolar sine LFO
        lfo.setAmplitude(1);
        lfo.setFrequency(8);
        lfo.setShape(0);
        lfo.setMode(0);

        memset(sampleQueue, 0, sizeof(sampleQueue));
	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint16_t index = 0;
    int16_t* samplePtr;
    int8_t maxOffset = 4; //max 4 blocks (4*128 samples) of offset (11.6ms of delay)

    bool active = false;
    LFO lfo;

    int16_t sampleQueue[DELAY_LENGHT];
	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif