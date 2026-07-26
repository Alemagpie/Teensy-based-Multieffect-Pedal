#include "effects/phaser_effect.h"

void PhaserEffect::update(void) {
    audio_block_t *block, *lfo_block;
    int16_t lfoOffset;

    block = receiveWritable();
    lfo_block = lfo_m.getReadOnly();

    if(!block) {
        return;
    } 

    if(enabled) {
        inputSamplePtr = block->data;
        lfoSamplePtr = lfo_block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            //lfoOffset = *lfoSamplePtr;//signed_saturate_rshift(*lfoSamplePtr * depth, 16, 15);    //lfo sweep is mapped exponentially
            //Sweep the filters
            uint16_t maxDepth = mode ? guitar_depth : bass_depth;
            uint16_t base_freq = mode ? guitar_baseFrequency : bass_baseFrequency;
            float f_min = (float)base_freq - (float)maxDepth;
            float f_max = (float)base_freq + (float)maxDepth;
            for(int j = 0; j < 4; j++) {
                ap_m[j].setCutoff(f_min * powf(f_max / f_min, (float)*lfoSamplePtr / 32767.0f));
            }

            //Run the sample through the filters
            int16_t dry = *inputSamplePtr;
            int16_t wet = dry;
            for(int j = 0; j < 4; j++) {
                ap_m[j].process(wet);    //Right now the filtering is done in-place
            }

            //Blend with original
            *inputSamplePtr = saturate16(2 * mx_m.process(dry, wet));   //The 2* is there to compensate for amplitude loss
            inputSamplePtr++;
            lfoSamplePtr++;            
        }
    }

    transmit(block);
    release(block);
}

void PhaserEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float valueLin = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );
    float valueLog = Utility::calculateParamValueLog(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            speed = valueLog;
            lfo_m.setFrequency(speed);
        break;

        case 1:
            depth = (mode) ? (uint16_t) (guitar_depth * valueLin) : (uint16_t) (bass_depth * valueLin);
        break;

        case 2:
            mix = valueLin;
            mx_m.setGain(1, mix);
        break;

        case 3:
            mode = (valueLin < 0.5f) ? true : false;
        break;

        default:
        break;
    }

}