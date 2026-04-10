#ifndef REV_READ_EFFECT_H
#define REV_READ_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/DelayLineModule.h"
#include "modules/MixerModule.h"


class ReverseReadEffect : public AudioStream, public EffectAdapter {
    public:
    ReverseReadEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 8), CustomRange(0, 256), CustomRange(), CustomRange()}) {
		ID = 7;
        
        reverseSamples = 1;
        mx_m.setGain(256 - mix, mix);

        effectName = "Reverse Read";
        paramName = {"REV", "MIX", "---", "---"};

	}

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint8_t reverseSamples = 1;
    uint8_t mix = 128;
    uint8_t reverseLeft = 1;
    uint32_t baseDelay = 300;

    bool active = false;
    DelayLineModule dl_m;
    MixerModule mx_m;
	audio_block_t *inputQueueArray[1];
    int16_t* inputSamplePtr;

    virtual void update(void);
};

#endif