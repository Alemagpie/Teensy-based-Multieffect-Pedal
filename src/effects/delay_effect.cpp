#include "effects/delay_effect.h"

void DelayEffect::update(void) {
    audio_block_t * block;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        inputSamplePtr = block->data;
        for(int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            int16_t sample = *inputSamplePtr;
            int16_t feedbackSample = dl_m.read(time);
            //The delay line holds the mix with no scaled feedback
            *inputSamplePtr = mx_m.process(*inputSamplePtr, feedbackSample);

            //Overwrite with scaled feedback
            feedbackSample = signed_saturate_rshift(feedback * feedbackSample, 16, 15);
            int16_t fullMix = mx_m.process(sample, feedbackSample);
            dl_m.write(fullMix);
            inputSamplePtr++;
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

void DelayEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            time = (uint32_t) value;    //Value change needs to be smoothed, probably creates noise when modified
        break;

        case 1:
            feedback = (uint8_t) value;
        break;

        case 2: //dry
            dry = (uint8_t) value;
            mx_m.setGain(0, dry);
        break;

        case 3: //wet
            wet = (uint8_t) value;
            mx_m.setGain(1, wet);
        break;
        
        default:
        break;
    }
}