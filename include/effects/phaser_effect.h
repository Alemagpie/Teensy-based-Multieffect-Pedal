#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h>
#include "EffectAdapter.h"

#include "modules/LFOModule.h"
#include "modules/AllPassFilterModule.h"

class PhaserEffect : public AudioStream, public EffectAdapter {
    public:
    PhaserEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 20), CustomRange(0, 5), CustomRange(0, 1), CustomRange()}) {
    
    }

    LFOModule lfo_m;    //All filters swept by the same lfo
    AllPassFilterModule ap_m[4];    //Make sample go through modulated 4 all-pass filters, then combine with original
                                    //Notch frequencies are 
                                    //Frequency modulation has to be logarithmic: f = f_min * (f_max/f_min) ^ LFO
    uint16_t bass_baseFrequency = 900; //sweep range: 200Hz-1.6kHz -> center: 900Hz, delta: 700Hz
    uint16_t bass_depth = 700;
    uint16_t guitar_baseFrequency = 440;   //sweep range: 80Hz-800Hz -> center: 440Hz, delta: 360Hz
    uint16_t guitar_depth = 360;

    bool mode = true;   //True for guitar, false for bass

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    int16_t *lfoSamplePtr;
    virtual void update(void);
};

#endif