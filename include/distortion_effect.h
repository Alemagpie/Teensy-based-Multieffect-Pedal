#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "Utility.h"

class DistortionEffect : public AudioStream, public EffectAdapter
{
    //distortion function: y = a * tanh(by) + a/2 * tanh((b-4)x) with a->[1, 5], b->[-5, 3] (a=gain, b=bias)
    public:
    DistortionEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1,5), CustomRange(-5, 3)}) { }

    void setParamLevel(int index, float level) override;
    void init(float p1, float p2, float p3, float p4) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    float gain; //param1
    float curve; //param2
    float bias; //param3 (range from -0.2 to 0.2) - currently unused

    //internal simplified parameters for performance
    int16_t gain1, gain2, curve1, curve2;
    void setInternalParams();

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(int16_t &value);
};

#endif