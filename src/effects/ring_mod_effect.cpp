#include "effects/ring_mod_effect.h"


void RingModulatorEffect::update(void)
{
    //each block has 128 int_16 samples, packed in 64 uint_32
    //block containing data
	audio_block_t *block, *lfoBlock;

    block = receiveWritable();
    if(!block) {
        return;
    }

    lfoBlock = lfo_m.getReadOnly();
    if (!lfoBlock) {
        release(block);
        return;
    }
    
    if(enabled) {

        inputSamplePtr = block->data;
        lfoSamplePtr = lfoBlock->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            int16_t sample = *inputSamplePtr;
            int16_t lfoSample = *lfoSamplePtr;

            lfoSample = signed_saturate_rshift(depth * lfoSample, 16, 15);
            sample = signed_saturate_rshift(sample * lfoSample, 16, 15);

            *inputSamplePtr = mx_m.process(*inputSamplePtr, sample);
            inputSamplePtr++;
            lfoSamplePtr++;
        }
    }

    transmit(block);
    release(block);
    release(lfoBlock);
}

void RingModulatorEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    float valueLin = Utility::calculateParamValueLin(ranges[index], (float) level / 65536.0f);
    float valueLog = Utility::calculateParamValueLog(ranges[index], (float) level / 65536.0f);

    switch(index) { 
        case 0:
        //change freq
        freq = valueLog;
        lfo_m.setFrequency(freq);
        break;

        case 1:
        //change shape
        shape = (short) valueLin;
        lfo_m.setShape(shape);
        break;

        case 2: 
        //change depth
        depth = (int16_t) valueLin;
        break;

        case 3: 
        mix = (uint8_t) valueLin;
        mx_m.setGain(0, mix);
        mx_m.setGain(1, 256 - mix);
        break;
        
        default:
        break;
    }
}