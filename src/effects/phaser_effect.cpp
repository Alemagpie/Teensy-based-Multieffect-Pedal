#include "effects/phaser_effect.h"

void PhaserEffect::update(void) {
    audio_block_t *block, *lfo_block;
    int32_t lfoOffset;

    block = receiveWritable();
    lfo_block = lfo_m.getReadOnly();

    if(!block) {
        return;
    } 

    if(enabled) {
        inputSamplePtr = block->data;
        lfoSamplePtr = lfo_block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            lfoOffset = signed_saturate_rshift(*lfoSamplePtr * depth, 16, 15);
            //Sweep the filters
            for(int i = 0; i < 4; i++) {
                uint16_t base_freq = mode ? guitar_baseFrequency : bass_baseFrequency;
                ap_m[i].setCutoff(base_freq + lfoOffset);
            }

            //Run the sample through the filters
            int16_t dry = *inputSamplePtr;
            int16_t wet = dry;
            for(int i = 0; i < 4; i++) {
                ap_m[i].process(wet);    //Right now the filtering is done in-place
            }

            //Blend with original
            *inputSamplePtr = saturate16(2 * mx_m.process(dry, wet));   //The 2* is there to compensate for amplitude loss
            inputSamplePtr++;
            lfoSamplePtr++;            
        }
    }
}

void PhaserEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            speed = value;
            lfo_m.setFrequency(speed);
        break;

        case 1:
            depth = (mode) ? (uint16_t) (guitar_depth * value) : (uint16_t) (bass_depth * value);
        break;

        case 2:
            mix = value;
            mx_m.setGain(0, 256 - mix);
            mx_m.setGain(1, mix);
        break;

        case 3:
            mode = (value < 0.5f) ? true : false;
        break;

        default:
        break;
    }

}