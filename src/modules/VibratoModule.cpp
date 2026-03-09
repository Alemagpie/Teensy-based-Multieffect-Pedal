#include "modules/VibratoModule.h"

void VibratoModule::process(int16_t &lfoOffset, int16_t &value) {
    if(active) {
        int16_t readIndex;

        writeIndex++;

        if(writeIndex >= VIBRATO_BUFFER_LENGHT) {
            writeIndex = 0;
        }

        sampleQueue[writeIndex] = value;
        readIndex = writeIndex - (baseDelay - lfoOffset);

        if(readIndex < 0) { readIndex += VIBRATO_BUFFER_LENGHT; }

        value = sampleQueue[readIndex];

    } else {
        writeIndex++;

        if(writeIndex >= VIBRATO_BUFFER_LENGHT) {
            writeIndex = 0;
        }

        sampleQueue[writeIndex] = value;
    }
}