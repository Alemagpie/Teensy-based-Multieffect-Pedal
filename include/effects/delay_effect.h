#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/DelayLineModule.h"
#include "modules/MixerModule.h"

class DelayEffect : public AudioStream, public EffectAdapter {
    DelayEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(45, 88000), CustomRange(0, 230), CustomRange(0, 256), CustomRange(0, 256)}) {
		ID = 7;
        
        mx_m.setGain(dry, wet);
        time = 22000;   //0.5s;
        feedback = 100;

        effectName = "Delay";
        paramName = {"TM", "REP", "DRY", "WET"};
	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint8_t dry = 256;
    uint8_t wet = 128;
    uint8_t feedback;
    uint32_t time;

    bool active = false;
    DelayLineModule<700*AUDIO_BLOCK_SAMPLES> dl_m;  //about 2s of max delay
    MixerModule mx_m;
	audio_block_t *inputQueueArray[1];
    int16_t* inputSamplePtr;

    virtual void update(void);
};

#endif