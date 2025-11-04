#ifndef EFFECT_DISTORTION_H
#define EFFECT_DISTORTION_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"

class AudioEffectDistortion : public AudioStream, EffectAdapter
{
    //distortion function: y = a * tanh(by) + a/2 * tanh((b-4)x) with a->[1, 5], b->[-5, 3] (a=gain, b=bias)
    public:
    AudioEffectDistortion(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1,5), CustomRange(-5,3)}) { }

    virtual void setParamLevel(int index, float level);

    private:
    float gain; //param1
    float bias; //param2

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(uint32_t &value);
};

#endif