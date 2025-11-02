#ifndef DISTORTION_H
#define DISTORTION_H

#include <AudioStream.h> 
#include <Arduino.h>
#include "EffectAdapter.h"

class AudioEffectDistortion : public AudioStream, public EffectAdapter
{
    //distortion function: y = a * tanh(by) + a/2 * tanh((b-4)x) with a->[1, 5], b->[-5, 3] (a=gain, b=bias)
    public:
    AudioEffectDistortion(void) : AudioStream(1, inputQueueArray), EffectAdapter({Range(1,4), Range(1,5)}) { }

    void setParamLevel(int index, float level) override;

    private:
    float gain; //param1
    float bias; //param2

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(uint32_t &value);
};

#endif