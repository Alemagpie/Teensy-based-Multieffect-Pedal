#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/DelayLineModule.h"
#include "modules/MixerModule.h"

class DelayEffect : public AudioStream, public EffectAdapter {
    public: //the delay buffer isn't big enough, it should be at least 1370 * AUDIO_SAMPLE_BLOCK long
    DelayEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(45, 176400), CustomRange(0, 256), CustomRange(), CustomRange()}) {
		ID = 7;
        
        mx_m.setGain(256 - mix, mix);

        effectName = "Delay";
        paramName = {"TM", "REP", "MIX", "LPF"};

	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint8_t mix = 128;
    int16_t feedback;
    uint32_t time;

    bool active = false;
    DelayLineModule dl_m;
    MixerModule mx_m;
	audio_block_t *inputQueueArray[1];
    int16_t* inputSamplePtr;

    virtual void update(void);
};

#endif