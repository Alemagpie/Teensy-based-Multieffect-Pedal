#include "effects/tremolo_effect.h"

void TremoloEffect::update(void)
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

            lp_m.process(lfoSample);
            lfoSample = signed_saturate_rshift(depth * lfoSample, 16, 15);
            g_m.setGain(lfoSample);
            g_m.process(sample);

            *inputSamplePtr = sample;
            inputSamplePtr++;
            lfoSamplePtr++;
        }
    }

    transmit(block);
    release(block);
    release(lfoBlock);
}

void TremoloEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    float value = Utility::calculateParamValueLin(ranges[index], (float) level / 65536.0f);

    switch(index) { 
        case 0:
        //change freq
        frequency = value;
        lfo_m.setFrequency(frequency);
        break;

        case 1:
        //change shape
        shape = (short) value;
        lfo_m.setShape(shape);
        break;

        case 2: 
        //change depth
        depth = (int16_t) (value * 32767.0f);
        break;

        case 3: default:
        break;
    }
}