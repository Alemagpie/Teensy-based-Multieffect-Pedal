#ifndef SQUARER_EFFECT_H
#define SQUARER_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/MixerModule.h"
#include "modules/LowPassFilterModule.h"

class SquarerEffect : public AudioStream, public EffectAdapter
{
    public:
    SquarerEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 256), CustomRange(0, 256), CustomRange(0, 256), CustomRange()}) {
        ID = 11;
        dry = sq1 = sq2 = 128;
        mx_m.setGain(0, dry);
        mx_m.setGain(1, sq1);
        mx_m.setGain(2, sq2);

        effectName = "Squarer";
        paramName = {"DRY", "SQ1", "SQ2", "LPF"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:

    MixerModule mx_m;
    LowPassFilterModule lp_m;

    //Original sample, same octave square wave, octave below square wave
    uint8_t dry, sq1, sq2;  

    uint16_t threshold = 200;
    uint16_t crossings = 0;
    bool isNeg = true;
    bool dwnOctaveState = true;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;

    virtual void update(void);
};

#endif