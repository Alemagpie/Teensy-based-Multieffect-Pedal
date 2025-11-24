#include "distortion_effect.h"

void DistortionEffect::update(void)
{
    //block containing data
	audio_block_t *block;
    uint32_t *start, *end;
    //two parts of block->data
    uint32_t d1, d2;

    block = receiveWritable();

    if(!block) {
        return;
    }

    start = (uint32_t *)(block->data);
    end = start + AUDIO_BLOCK_SAMPLES/2;

    while(start < end) {
        d1 = *start;
        d2 =*(start+1);

        //processing
        processSignal(d1);
        processSignal(d2);

        *start++ = d1;
        *start++ = d2;
    }

    transmit(block);
    release(block);
}


void DistortionEffect::processSignal(uint32_t &value) {
    //input value to normalized float (function works between -1.0 and 1.0)
    float x = ((float)value / 2147483647.5f) - 1.0f;
    float y = gain * Utility::fastTanh(bias*x) + (gain/2) * Utility::fastTanh((bias-4)*x);  //optimized with LUT
    //clamp output (clip)
    y = (y < -1.0f) ? -1.0f : (y > 1.0f ? 1.0f : y);
    //map back to uint32
    value = (uint32_t)((y + 1.0f) * 2147483647.5f);
}


void DistortionEffect::setParamLevel(int index, float level) {
    if(index < 0 || index > parameterCount - 1 || level < 0 || level > 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int value = (int)Utility::calculateParamValue(ranges[index], level);

    switch(index) {
        case 0:
        //change gain
        gain = value;
        break;

        case 1:
        //change bias
        bias = value;
        break;

        //don't do anything, the last two parameters are inactive
        case 2: case 3: default:
        break;
    }
}

void DistortionEffect::init(float p1, float p2, float p3, float p4) {}