#include "effects/vibrato_effect.h"

void VibratoEffect::update(void) {
    audio_block_t *block, *lfo_block;
    int32_t lfoOffset, readOffset;

    block = receiveWritable();
    lfo_block = lfo_m.getReadOnly();

    if(!block) {
        return;
    } 

    if(enabled) {
        inputSamplePtr = block->data;
        lfoSamplePtr = lfo_block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr);
            lfoOffset = signed_saturate_rshift(*lfoSamplePtr * depth, 16, 15);
            readOffset = baseDelay - lfoOffset;
            *inputSamplePtr = dl_m.read(readOffset);
            lp_m.process(*inputSamplePtr);
            inputSamplePtr++;
            lfoSamplePtr++;
        }
    } else {
        //store in queue
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr++);
        }
    }

    transmit(block);
    release(block);
}

void VibratoEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            freq = value;
            lfo_m.setFrequency(freq);
        break;

        case 1:
            depth = (uint16_t) value;
        break;

        case 2: 
            lp_m.setCutoff(value);
        break;
            
        case 3: 
            
        break;
        
        default:
        break;
    }
}