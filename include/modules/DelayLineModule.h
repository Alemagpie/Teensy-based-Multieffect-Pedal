#ifndef D_LINE_MODULE
#define D_LINE_MODULE

#include "Module.h"

#define DELAY_BUFFER_LENGHT  (16*AUDIO_BLOCK_SAMPLES)

class DelayLineModule : Module {
    public:
    DelayLineModule() { memset(sampleQueue, 0, sizeof(sampleQueue)); }

    void write(int16_t &value);
    int16_t read(uint16_t offset);
    inline void reset() override { memset(sampleQueue, 0, sizeof(sampleQueue)); }

    private:
    uint16_t writeIndex = 0;
    //uint16_t baseDelay = 330; //base distance (without LFO) in samples between read and write index

    int16_t sampleQueue[DELAY_BUFFER_LENGHT];
};

#endif