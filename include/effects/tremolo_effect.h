#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/GainModule.h"
#include "modules/LowPassFilterModule.h"
#include "modules/LFOModule.h"

class TremoloEffect : public AudioStream, public EffectAdapter {
    public:
    TremoloEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 20), CustomRange(0, 5), CustomRange(0, 1), CustomRange()}) {
        ID = 2;
        
        depth = 32767;
        
        lfo_m.setAmplitude(1.0);
        lfo_m.setFrequency(8);
        lfo_m.setShape(3);

        //Mode set to unipolar (permanent)
        lfo_m.setMode(1);

        lp_m.setCutoff(100);  //permanent

        effectName = "Tremolo";
        paramName = {"FRQ", "SHP", "DPT", "---"}; 
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    float frequency;
    int shape;
    int16_t depth;

    LFOModule lfo_m;
    GainModule g_m;
    LowPassFilterModule lp_m;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    int16_t *lfoSamplePtr;
    virtual void update(void);
}; 

#endif