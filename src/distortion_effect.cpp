#include "distortion_effect.h"

void DistortionEffect::update(void)
{
	audio_block_t *block;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        inputSamplePtr = block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            int16_t sample = *inputSamplePtr;
            d_m.process(sample);
            g_m.process(sample);
            hp_m.process(sample);
            lp_m.process(sample);
            *inputSamplePtr = sample;
            *inputSamplePtr++;
        }
    }
    
    transmit(block);
    release(block);
}


void DistortionEffect::processSignal(int16_t &value) {
    value = signed_saturate_rshift( 
        volume * Utility::fastTanh(
            saturate16(
                (int32_t)gain * value + bias
            )
        ), 
    16, 15);
}


void DistortionEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t value = (int16_t) Utility::calculateParamValueLin(ranges[index], (float)level/65536.0f );

    switch(index) {
        case 0:
        //change gain
        gain = value;
        d_m.setGain(gain);
        break;

        case 1:
        //change curve
        bias = value;
        d_m.setBias(bias);
        break;

        case 2: 
        lp_m.setCutoff(value);
        break;

        case 3: 
        volume = value;
        g_m.setGain(volume);
        break;
        
        default:
        break;
    }
}