#ifndef D_LINE_MODULE
#define D_LINE_MODULE

#include "Module.h"

#define DELAY_BUFFER_LENGHT  (16*AUDIO_BLOCK_SAMPLES)   //default, smaller buffer
#define EXT_DELAY_BUFFER_LENGHT  (700*AUDIO_BLOCK_SAMPLES)   //max 2 s, used in effects like delays

template <size_t BufferLenght>
class DelayLineModule : Module {
    public:
    DelayLineModule() { 
        memset(sampleQueue, 0, sizeof(sampleQueue)); 
    }

    void write(int16_t &value) {
        writeIndex++;

        if(writeIndex >= BufferLenght) {
            writeIndex = 0;
        }

        sampleQueue[writeIndex] = value;
    }

    int16_t read(uint16_t offset) {
        int32_t readIndex = writeIndex - offset;

        while(readIndex < 0) { readIndex += BufferLenght; }

        return sampleQueue[readIndex];
    }

    inline void reset() override { memset(sampleQueue, 0, sizeof(sampleQueue)); writeIndex = 0; }

    private:
    uint32_t writeIndex = 0;

    int16_t sampleQueue[BufferLenght];
};

#endif