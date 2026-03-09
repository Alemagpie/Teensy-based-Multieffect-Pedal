#ifndef VIBR_MODULE
#define VIBR_MODULE

#include "Module.h"

#define VIBRATO_BUFFER_LENGHT  (16*AUDIO_BLOCK_SAMPLES)

class VibratoModule : Module {
    VibratoModule() { memset(sampleQueue, 0, sizeof(sampleQueue)); }

    void process(int16_t &lfoOffset, int16_t &value);
    void reset() override;

    private:
    uint16_t writeIndex = 0;
    uint16_t baseDelay = 330; //base distance (without LFO) in samples between read and write index

    int16_t sampleQueue[VIBRATO_BUFFER_LENGHT];
};

#endif