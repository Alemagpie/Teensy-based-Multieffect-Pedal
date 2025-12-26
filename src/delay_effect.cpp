#include "delay_effect.h"

void DelayEffect::update(void) {
    audio_block_t *block;
    uint32_t *start, *end;
    uint32_t d1, d2;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        //magic delay stuff here
    }

    transmit(block);
    release(block);
}

void DelayEffect::setParamLevel(int index, uint16_t level) {
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