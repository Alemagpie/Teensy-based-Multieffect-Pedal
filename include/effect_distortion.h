#ifndef EFFECT_DISTORTION_H
#define EFFECT_DISTORTION_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "EffectHandler.h"

class AudioEffectDistortion : public AudioStream//, public EffectHandler
{
    //distortion function: y = a * tanh(by) + a/2 * tanh((b-4)x) with a->[1, 5], b->[-5, 3] (a=gain, b=bias)
    public:
    AudioEffectDistortion(void) : AudioStream(1, inputQueueArray)/*, EffectHandler({CustomRange(1,4), CustomRange(1,5)})*/ { }

    virtual void setParamLevel(int index, float level);

    private:
    float gain; //param1
    float bias; //param2

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(uint32_t &value);
};

#endif