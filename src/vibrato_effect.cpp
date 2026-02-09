#include "vibrato_effect.h"

void VibratoEffect::update(void) {
    audio_block_t *block;
    uint32_t *start, *end;
    uint32_t d1, d2;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        //magic vibrato stuff here
    } else {
        //store in queue
        samplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            index++;

            if(index >= DELAY_LENGHT) {
            index = 0;
            }

            sampleQueue[index] = *samplePtr++;
        }
    }

    transmit(block);
    release(block);
}

void VibratoEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t value = (int16_t) Utility::calculateParamValue(ranges[index], (float)level/65536.0f );

    switch(index) {
        case 0:

        break;

        case 1:

        break;

        case 2: 
        
        break;

        case 3: 
        
        break;
        
        default:
        break;
    }
}