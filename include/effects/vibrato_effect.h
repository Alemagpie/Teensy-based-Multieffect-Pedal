#ifndef VIBRATO_EFFECT_H
#define VIBRATO_EFFECT_H


#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/LFOModule.h"
#include "modules/DelayLineModule.h"
#include "modules/LowPassFilterModule.h"

#define VIBRATO_BUFFER_LENGHT  (16*AUDIO_BLOCK_SAMPLES)

class VibratoEffect : public AudioStream, public EffectAdapter {
    public:
    VibratoEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1, 10), CustomRange(1 , 220), CustomRange(2000, 12000), CustomRange(0, 1)}) {
		ID = 3;
        
        freq = 5;
        //setup bipolar sine LFO
        lfo_m.setAmplitude(1);
        lfo_m.setFrequency(freq);
        lfo_m.setShape(0);
        lfo_m.setMode(0);
        depth = 132;
        
        lp_m.setCutoff(7000);

        effectName = "Vibrato";
        paramName = {"RT", "DPT", "TN", "---"};

        memset(sampleQueue, 0, sizeof(sampleQueue));
	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint16_t baseDelay = 330; //base distance (without LFO) in samples between read and write index
    int16_t* inputSamplePtr;
    int16_t* lfoSamplePtr;

    bool active = false;

    LFOModule lfo_m;
    DelayLineModule dl_m;
    LowPassFilterModule lp_m;

    uint16_t freq;
    uint16_t depth;

    int16_t sampleQueue[VIBRATO_BUFFER_LENGHT];
	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif