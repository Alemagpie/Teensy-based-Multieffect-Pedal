#include "tremolo_effect.h"

void TremoloEffect::update(void)
{
    //each block has 128 int_16 samples, packed in 64 uint_32
    //block containing data
	audio_block_t *block, *lfoBlock;
    uint32_t *start, *end, *lfoStart;
    //two parts of block->data
    uint32_t d1, d2, d3, d4;

    block = receiveWritable();
    if(!block) {
        return;
    }

    lfoBlock = lfo.getReadOnly();
    if (!lfoBlock) {
        release(block);
        return;
    }
    
    if(enabled) {
        start = (uint32_t *)(block->data);
        lfoStart = (uint32_t *)(lfoBlock->data);
        end = start + AUDIO_BLOCK_SAMPLES/2;

        while(start < end) {
            d1 = *start;
            d2 = *(start+1);
            d3 = *lfoStart++;
            d4 = *lfoStart++;

            int16_t sample1 = (int16_t)(d3 & 0xFFFF);
            int16_t sample2 = (d3 >> 16) & 0xFFFF;
            int16_t sample3 = (int16_t)(d4 & 0xFFFF);
            int16_t sample4 = (d4 >> 16) & 0xFFFF;

            //convert LFO to be within [0, 1] (unipolar)
            sample1 = (sample1 + 32768) >> 1;
            sample2 = (sample2 + 32768) >> 1;
            sample3 = (sample3 + 32768) >> 1;
            sample4 = (sample4 + 32768) >> 1;

            //filter LFO to prevent pops due to LFO discontinuity (square, saw, ramp)
            lp.filter(&sample1);
            lp.filter(&sample2);
            lp.filter(&sample3);
            lp.filter(&sample4);

            //multiply LFO samples by depth
            sample1 = signed_saturate_rshift(depth * sample1, 16, 15);
            sample2 = signed_saturate_rshift(depth * sample2, 16, 15);
            sample3 = signed_saturate_rshift(depth * sample3, 16, 15);
            sample4 = signed_saturate_rshift(depth * sample4, 16, 15);

            d3 = pack_16b_16b((32767 - sample1), (32767 - sample2));
            d4 = pack_16b_16b((32767 - sample3), (32767 - sample4));

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

    float value = Utility::calculateParamValue(ranges[index], (float) level / 65536.0f);

    switch(index) {
        case 0:
        //change freq
        lfo.setFrequency(value);
        break;

        case 1:
        //change shape
        lfo.setShape((short) value);
        break;

        case 2: 
        //change depth
        depth = (int16_t) (value * 32767.0f);
        Serial.print(depth);
        Serial.printf("\n");
        break;

        case 3: default:
        break;
    }
}