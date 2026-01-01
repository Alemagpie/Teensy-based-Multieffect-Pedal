#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H


#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include "EffectAdapter.h"
#include "Utility.h"
#include "HighPassFilter.h"
#include "LowPassFilter.h"

#define DELAY_QUEUE_SIZE  (176512 / AUDIO_BLOCK_SAMPLES) //max for teensy 4.1

//Effect that creates an "echo"
class DelayEffect : public AudioStream, public EffectAdapter {
    public:
    DelayEffect() : AudioStream(1, inputQueueArray), EffectAdapter({CustomRange(3, 12), CustomRange(0, 7000), CustomRange(2500, 8000), CustomRange(0, 32767)}) {
		headIndex = 0;
		tailIndex = 0;
		maxBlocks = 0;
        maxSampleCount = AUDIO_BLOCK_SAMPLES * (DELAY_QUEUE_SIZE - 1);
		memset(sampleQueue, 0, sizeof(sampleQueue));
	}

    void setDelay(float ms) {
        if (ms < 0.0f) {
            ms = 0.0f;
        }

        //number of samples based on ms of delay
        uint32_t sampleCount = (ms * (AUDIO_SAMPLE_RATE_EXACT / 1000.0f)) + 0.5f;         

        if (sampleCount > maxSampleCount) {
            sampleCount = maxSampleCount;
        }

        delaySamples = sampleCount;

        //number of blocks needed
        uint32_t blockCount = (sampleCount + (AUDIO_BLOCK_SAMPLES - 1)) / AUDIO_BLOCK_SAMPLES + 1;

        if (!active) {
            // First time enabling delay
            maxBlocks = blockCount;
            active = true;
        } else {
            // Delay already active, just update block count
            maxBlocks = blockCount;
        }
    }

    void setParamLevel(int index, uint16_t level) override;
    AudioStream* getAudioStreamComponent() override {return this;}

    private:
    uint16_t headIndex;    
	uint16_t tailIndex;

    uint32_t delaySamples = 0;
    uint32_t maxBlocks = 0;
    uint32_t maxSampleCount;
    bool active = false;

    audio_block_t *sampleQueue[DELAY_QUEUE_SIZE];
	audio_block_t *inputQueueArray[1];

    virtual void update(void);
};

#endif