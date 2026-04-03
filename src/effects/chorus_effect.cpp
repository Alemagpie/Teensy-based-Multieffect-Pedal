#include "effects/chorus_effect.h"

void ChorusEffect::update(void) {
    audio_block_t *block;
    int32_t lfoOffset;
    int32_t voicesSum;

    block = receiveWritable();

    if(!block) {
        return;
    }

    for(int k = 0; k < MAX_CHORUS_VOICES; k++) {
        lfoSamplePtrs[k] = lfos_m[k].getReadOnly()->data;
    }

    if(enabled) {
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr);
            voicesSum = 0;

            for(int j = 0; j < voices; j++) {
                lfoOffset = signed_saturate_rshift(*lfoSamplePtrs[j] * depth, 16, 15);
                int delay = baseDelay - voicesOffset * j + lfoOffset;
                voicesSum += dl_m.read(delay);
            }
            
            int16_t norm = (int16_t)(voicesSum / (voices - 1));
            *inputSamplePtr = mx_m.process(*inputSamplePtr, norm);
            inputSamplePtr++;

            for(int k = 0; k < MAX_CHORUS_VOICES; k++) {
                lfoSamplePtrs[k]++;
            }
        }

    } else {
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr++);
        }
    }
    
    transmit(block);
    release(block);
}

void ChorusEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            freq = value + random(-2, +2);
            for (auto &&i : lfos_m)
            {
                i.setFrequency(freq);
            }
        break;

        case 1:
            depth = (uint8_t) value;
        break;

        case 2: 
            voices = (uint8_t) value;
        break;

        case 3: 
            mix = (uint8_t) value;
            mx_m.setGain(0, 256 - mix);
            mx_m.setGain(1, mix);
        break;
        
        default:
        break;
    }
}