#include "distortion_effect.h"

void DistortionEffect::update(void)
{
    //each block has 128 int_16 samples, packed in 64 uint_32
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
        d2 = *(start+1);

        //unpack two uint32 into four int_16 samples
        uint32_t word1 = d1;
        uint32_t word2 = d2;
        int16_t sample1 = (int16_t)(word1 & 0xFFFF);
        int16_t sample2 = (word1 >> 16) & 0xFFFF;
        int16_t sample3 = (int16_t)(word2 & 0xFFFF);
        int16_t sample4 = (word2 >> 16) & 0xFFFF;

        //distortion function
        processSignal(sample1);
        processSignal(sample2);
        processSignal(sample3);
        processSignal(sample4);

        /*
        //block dc bias
        hpF.filter(&sample1);
        hpF.filter(&sample2);
        hpF.filter(&sample3);
        hpF.filter(&sample4);

        //shape tone
        lpF.filter(&sample1);
        lpF.filter(&sample2);
        lpF.filter(&sample3);
        lpF.filter(&sample4);
        */

        //repack four int_16 processed samples into two uint32
        d1 = pack_16b_16b(sample1, sample2);
        d1 = pack_16b_16b(sample3, sample4);

        *start++ = d1;
        *start++ = d2;
    }

    transmit(block);
    release(block);
}


void DistortionEffect::processSignal(int16_t &value) {
    //working with Q15 for performance
    int16_t p1, p2;
    //b*x
    p1 = signed_saturate_rshift((int32_t)value * curve1, 16, 15);
    //(b-4)*x
    p2 = signed_saturate_rshift((int32_t)value * curve2, 16, 15);
    //tanh(bx)
    p1 = Utility::fastTanh(p1);
    //tanh((b-4)x)
    p2 = Utility::fastTanh(p2);
    //a*tanh(bx)
    p1 = signed_saturate_rshift((int32_t)p1 * gain1, 16, 15);
    //a/2*tanh((b-4)x)
    p2 = signed_saturate_rshift((int32_t)p2 * gain2, 16, 15);
    value = saturate16((int32_t)p1 + (int32_t)p2);
}


void DistortionEffect::setParamLevel(int index, float level) {
    if(index < 0 || index > parameterCount - 1 || level < 0.0f || level > 1.0f) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    float value = Utility::calculateParamValue(ranges[index], level);

    switch(index) {
        case 0:
        //change gain
        gain = value;
        break;

        case 1:
        //change curve
        curve = value;
        break;

        //don't do anything, the last two parameters are inactive
        case 2: case 3: default:
        break;
    }

    setInternalParams();
}

void DistortionEffect::setInternalParams() {
    gain1 = (int16_t)(gain * 32767.0f);
    gain2 = (int16_t)((gain * 0.5f) * 32767.0f);
    curve1 = (int16_t)(bias * 32767.0f);
    curve2 = (int16_t)((bias - 4.0f) * 32767.0f);
}

void DistortionEffect::init(float p1, float p2, float p3, float p4) {}