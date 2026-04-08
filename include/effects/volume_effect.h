#ifndef VOLUME_EFFECT_H
#define VOLUME_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectAdapter.h"

#include "modules/GainModule.h"

class VolumeEffect : public AudioStream, public EffectAdapter {
    public:
    VolumeEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 32767), CustomRange(), CustomRange(), CustomRange()}) {
        ID = 6;

        effectName = "Volume";
        paramName = {"VOL", "---", "---", "---"}; 
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    GainModule g_m;
    int16_t volume;

    audio_block_t *inputQueueArray[1];
    int16_t *inputSamplePtr;
    virtual void update(void);
};

#endif