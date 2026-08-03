#ifndef ENV_FILTER_EFFECT_H
#define ENV_FILTER_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h>
#include "EffectAdapter.h"
#include "Utility.h"

#include "modules/BiquadFilterModule.h"
#include "modules/EnvelopeModule.h"

class EnvelopeFilterEffect : public AudioStream, public EffectAdapter {
    public:
    EnvelopeFilterEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 1), CustomRange(0.5, 20), CustomRange(0, 1), CustomRange(0, 3)}) {
        ID = 10;

        bq_m.setCutoff(2500);
        bq_m.setQ(0.707f);
        mode = 0;
        sens = 0.707f;
        bq_m.setMode(mode);
        env_m.setAttack(15);
        env_m.setRelease(225);
        env_m.setGain(20);  //Has to be higher for guitar

        effectName = "Env filter";
        paramName = {"SNS", " Q ", "A/R", "MD"};
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    BiquadFilterModule bq_m;
    EnvelopeModule env_m;

    uint8_t sampleCount = 0;
    float f_min = 50;   //100 for guitar
    float f_max = 2000; //5000 for guitar
    float freq_amount = 0;
    //atk/rel in ms 
    uint16_t atk_min = 1;
    uint16_t atk_max = 30;
    uint16_t rel_min = 35;
    uint16_t rel_max = 400;
    float atk_rel_amount = 0;
    float sens;
    float q = 0.707f;
    uint8_t mode;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    virtual void update(void);
};

#endif