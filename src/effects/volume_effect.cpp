#include "effects/volume_effect.h"

void VolumeEffect::update(void)
{
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
            g_m.process(sample);

            *inputSamplePtr = sample;
            inputSamplePtr++;
        }
    }

    transmit(block);
    release(block);
}

void VolumeEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    float value = Utility::calculateParamValueLin(ranges[index], (float) level / 65536.0f);

    switch(index) { 
        case 0:
        volume = (int16_t) value;
        g_m.setGain(volume);
        break;

        case 1: case 2: case 3: default:
        break;
    }
}