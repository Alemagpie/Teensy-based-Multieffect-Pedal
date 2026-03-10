#include "modules/DelayLineModule.h"

void DelayLineModule::write(int16_t &value) {
    writeIndex++;

    if(writeIndex >= DELAY_BUFFER_LENGHT) {
        writeIndex = 0;
    }

    sampleQueue[writeIndex] = value;
}

int16_t DelayLineModule::read(uint16_t offset) {
    int16_t readIndex = writeIndex - offset;

    if(readIndex < 0) { readIndex += DELAY_BUFFER_LENGHT; }

    return sampleQueue[readIndex];
}