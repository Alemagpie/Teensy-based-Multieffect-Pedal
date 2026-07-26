#ifndef BIQUAD_EFFECT_H
#define BIQUAD_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h>
#include "EffectAdapter.h"
#include "Utility.h"

#include "modules/BiquadFilterModule.h"

class BiquadEffect : public AudioStream, public EffectAdapter {
    public:
    BiquadEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(82, 10000), CustomRange(0.5, 20), CustomRange(0, 5), CustomRange()}) {
        ID = 8;

        freq = 1000;
        bq_m.setCutoff(freq);
        bq_m.setQ(0.707f);
        mode = 0;
        bq_m.setMode(mode);

        effectName = "Biquad filter";
        paramName = {"FRQ", " Q ", "MD", "---"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    BiquadFilterModule bq_m;

    float freq;
    uint8_t mode;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    virtual void update(void);
};

#endif