#include "effects/reverse_read_effect.h"

void ReverseReadEffect::update(void) {
    audio_block_t *block;
    uint16_t readOffset;
    int16_t dry, wet, out;

    if(!block) {
        return;
    } 

    if(enabled) {
        inputSamplePtr = block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr);
            dry = *inputSamplePtr;

            if(reverseSamples > 1) {
                readOffset = baseDelay - reverseLeft;
                wet = dl_m.read(readOffset);

                reverseLeft--;

                if(reverseLeft == 0) {
                    reverseLeft = reverseSamples;
                }
                
                out = mx_m.process(dry, wet);
            }
            
            *inputSamplePtr = out;
            inputSamplePtr++;
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

void ReverseReadEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            reverseSamples = 1 << (uint8_t) value;
            reverseLeft = reverseSamples;
        break;

        case 1:
            mix = (uint8_t) value;
            mx_m.setGain(256 - mix, mix);
        break;

        case 2: 
            
        break;
            
        case 3: 
            
        break;
        
        default:
        break;
    }
}