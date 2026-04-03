#include "effects/bitcrusher_effect.h"

void BitCrusherEffect::update(void) {
    audio_block_t *block;
    int16_t sample, originalSample;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        inputSamplePtr = block->data; 

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            sample = originalSample = *inputSamplePtr;
            sh_m.process(sample);
            q_m.process(sample);
            sample = mx_m.process(originalSample, sample);
            lp_m.process(sample);

            *inputSamplePtr = sample;
            inputSamplePtr++;
        }
    }
    
    transmit(block);
    release(block);
}

void BitCrusherEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t value = (int16_t) Utility::calculateParamValueLin(ranges[index], (float)level/65536.0f );

    switch(index) {
        case 0:
        bitRes = value;
        q_m.setResolution(bitRes);
        break;

        case 1:
        downSample = value;
        sh_m.setHold(downSample);
        break;

        case 2: 
        mix = value;
        mx_m.setGain(0, 256 - mix);
        mx_m.setGain(1, mix);
        break;

        case 3: 
        lp_m.setCutoff(value);
        break;
        
        default:
        break;
    }
}