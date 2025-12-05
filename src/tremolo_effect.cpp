#include "tremolo_effect.h"

void TremoloEffect::update(void)
{
    //each block has 128 int_16 samples, packed in 64 uint_32
    //block containing data
	audio_block_t *block, *lfoBlock;
    uint32_t *start, *end, *lfoStart;
    //two parts of block->data
    uint32_t d1, d2, d3, d4;

    block = receiveWritable(0);
    lfoBlock = receiveReadOnly(1);

    if(!block) {
        return;
    }

    start = (uint32_t *)(block->data);
    lfoStart = (uint32_t *)(lfoBlock->data);
    end = start + AUDIO_BLOCK_SAMPLES/2;

    while(start < end) {
        d1 = *start;
        d2 = *(start+1);
        d3 = *lfoStart++;
        d4 = *lfoStart++;

        d1 = pack_16b_16b(
            signed_saturate_rshift(multiply_16tx16t(d1, d3), 16, 15),
            signed_saturate_rshift(multiply_16bx16b(d1, d3), 16, 15)
        );
        d2 = pack_16b_16b(
            signed_saturate_rshift(multiply_16tx16t(d2, d4), 16, 15),
            signed_saturate_rshift(multiply_16bx16b(d2, d4), 16, 15)
        );

        *start++ = d1;
        *start++ = d2;
    }

    transmit(block);
    release(block);
    release(lfoBlock);
}

void TremoloEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1 || level < 0 || level > 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t value = (int16_t) Utility::calculateParamValue(ranges[index], level>>15);

    switch(index) {
        case 0:
        //change freq
        LFO.frequency(value);
        break;

        case 1:
        //change shape

        break;

        //don't do anything, the last two parameters are inactive
        case 2: case 3: default:
        break;
    }
}


void TremoloEffect::init(float p1, float p2, float p3, float p4) {}