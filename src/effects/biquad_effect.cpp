#include "effects/biquad_effect.h"

void BiquadEffect::update(void) {
    audio_block_t *block;
    int16_t sample;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        inputSamplePtr = block->data; 

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            sample = *inputSamplePtr;
            bq_m.process(sample);

            *inputSamplePtr = sample;
            inputSamplePtr++;
        }
    }
    
    transmit(block);
    release(block);
}

void BiquadEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t valueLin = (int16_t) Utility::calculateParamValueLin(ranges[index], (float)level/65536.0f );
    int16_t valueLog = (int16_t) Utility::calculateParamValueLog(ranges[index], (float)level/65536.0f);

    switch(index) {
        case 0:
            freq = valueLog;
            bq_m.setCutoff(freq);
        break;

        case 1:
            bq_m.setQ(valueLog);
        break;

        case 2:
            mode = (uint8_t) valueLin;
            bq_m.setMode(mode);
        break;
        
        default:
        break;
    }
}