#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "LFO.h"
#include "LowPassFilter.h"

class TremoloEffect : public AudioStream, public EffectAdapter {
    public:
    TremoloEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 20), CustomRange(0, 5), CustomRange(0, 1), CustomRange()}) {
        depth = 32767;
        
        lfo.setAmplitude(1.0);
        lfo.setFrequency(8);
        lfo.setShape(3);

        //Mode set to unipolar (permanent)
        lfo.setMode(1);

        lp.setCutoff(100);  //permanent

        effectName = "Tremolo";
        paramName = {"FRQ", "SHP", "DPT", "---"}; 
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    float frequency;
    float shape;
    int16_t depth;
    LFO lfo;
    LowPassFilter lp;

    audio_block_t *inputQueueArray[1];
    virtual void update(void);
};

#endif