#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"

class PlaceholderEffect : public AudioStream, public EffectAdapter {
    public:
    PlaceholderEffect(void) : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(1,2), CustomRange(1, 2)}) { }

    void setParamLevel(int index, float level) override {return;}
    void init(float p1, float p2, float p3, float p4) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    audio_block_t *inputQueueArray[1];
    virtual void update(void);
};