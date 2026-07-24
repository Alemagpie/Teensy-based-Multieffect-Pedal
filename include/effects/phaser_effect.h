#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h>
#include "EffectAdapter.h"

#include "modules/LFOModule.h"
#include "modules/AllPassFilterModule.h"
#include "modules/MixerModule.h"

class PhaserEffect : public AudioStream, public EffectAdapter {
    public:
    PhaserEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 10), CustomRange(0, 1), CustomRange(0, 256), CustomRange(0, 1)}) {
        ID = 9;
        //The depth parameter must not be used directly (depth = value) but as an offset (depth = baseDepth + depth * value)

        speed = 3;
        mode = true;
        depth = guitar_depth;

        lfo_m.setAmplitude(0.5);
        lfo_m.setFrequency(speed);
        lfo_m.setMode(0);   //Bipolar
        lfo_m.setShape(0);  //Sine

        mix = 128;

        effectName = "Phaser";
        paramName = {"RT", "DPT", "MIX", "MD"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    MixerModule mx_m;
    LFOModule lfo_m;    //All filters are swept by the same lfo
    AllPassFilterModule ap_m[4];    //Make sample go through modulated 4 all-pass filters, then combine with original
                                    //Notch frequencies are 
                                    //Frequency modulation has to be logarithmic: f = f_min * (f_max/f_min) ^ LFO
    uint16_t bass_baseFrequency = 900; //sweep range: 200Hz-1.6kHz -> center: 900Hz, delta: 700Hz
    uint16_t bass_depth = 700;
    uint16_t guitar_baseFrequency = 440;   //sweep range: 80Hz-800Hz -> center: 440Hz, delta: 360Hz
    uint16_t guitar_depth = 360;

    uint16_t speed;
    uint16_t depth;
    uint8_t mix;

    bool mode = true;   //True for guitar, false for bass

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    int16_t *lfoSamplePtr;
    virtual void update(void);
};

#endif