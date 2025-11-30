#ifndef tremolo_effect_h
#define tremolo_effect_h

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"

class TremoloEffect : public AudioStream, public EffectAdapter {
    public:
    TremoloEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(0, 20), CustomRange(0, 1)}) {
        LFO.frequency(5);
        LFO.amplitude(1.0f);
        lfoConnection.connect(LFO, 0, *this, 1);
    }

    void setParamLevel(int index, float level) override;
    void init(float p1, float p2, float p3, float p4) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    int frequency;
    float shape;

    AudioSynthWaveformSine LFO;
    AudioConnection lfoConnection;

    audio_block_t *inputQueueArray[2];
    virtual void update(void);
};

#endif