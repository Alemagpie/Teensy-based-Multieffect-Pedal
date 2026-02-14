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

#define VIBRATO_BUFFER_LENGHT  (16*AUDIO_BLOCK_SAMPLES)

class VibratoEffect : public AudioStream, public EffectAdapter {
    public:
    VibratoEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1, 10), CustomRange(1 , 220), CustomRange(2000, 12000), CustomRange(0, 1)}) {
		effectName = "Vibrato";
        paramName = {"RT", "DPT", "TN", "---"};

        freq = 5;
        depth = 132;
        
        lp.setCutoff(7000);

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
    uint16_t writeIndex = 0;
    uint16_t baseDelay = 330; //base distance (without LFO) in samples between read and write index
    int16_t* inputSamplePtr;
    int16_t* lfoSamplePtr;

    bool active = false;
    LFO lfo;    //the LFO offset goes from 1 to 5 ms (roughly 44 to 220 samples)
    LowPassFilter lp;

    uint16_t freq;
    uint16_t depth;

    int16_t sampleQueue[VIBRATO_BUFFER_LENGHT];
	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif