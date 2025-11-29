#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "Utility.h"
#include "HighPassFilter.h"
#include "LowPassFilter.h"

class DistortionEffect : public AudioStream, public EffectAdapter
{
    //distortion function: y = a * tanh(by) + a/2 * tanh((b-4)x) with a->[5, 15], b->[-5, 3] (a=gain, b=bias)
    public:
    DistortionEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(5, 15), CustomRange(-5, 3)}) {gain = 10.0f; curve = 3.0f; setInternalParams(); }

    void setParamLevel(int index, float level) override;
    void init(float p1, float p2, float p3, float p4) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    HighPassFilter hpF;
    LowPassFilter lpF;

    float gain = 5.0f; //param1
    float curve = 2.0f; //param2
    float bias; //param3 (range from -0.2 to 0.2) - currently unused

    //internal simplified parameters for performance
    int16_t gain1, gain2, curve1, curve2;
    void setInternalParams();

    audio_block_t *inputQueueArray[1];

    virtual void update(void);
    void processSignal(int16_t &value);
};

#endif