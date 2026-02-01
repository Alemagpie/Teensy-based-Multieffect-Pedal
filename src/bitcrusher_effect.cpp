#include "bitcrusher_effect.h"

void BitCrusherEffect::update(void) {
    audio_block_t *block;
    uint32_t *start, *end;
    //two parts of block->data
    uint32_t d1, d2;

    //number of cycles to hold the sample for when downsampling
    uint8_t holdCycles = 0;
    //sample to hold
    int16_t holdSample;

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
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

            //downsample
            if(downSample > 1) {
                switch(downSample) {
                    case 2:
                    //1/2x sampling rate
                    sample2 = sample1;
                    sample4 = sample3;
                    break;

                    case 4:
                    //1/4x sampling rate
                    sample4 = sample3 = sample2 = sample1;
                    break;

                    case 8: case 16: case 32:
                    if(holdCycles == 0) {
                        holdCycles = downSample / 4;
                        holdSample = sample1;
                    }

                    sample4 = sample3 = sample2 = sample1 = holdSample;
                    holdCycles--;
                    break;
                }
            }

            int16_t drySample1 = sample1;
            int16_t drySample2 = sample2;
            int16_t drySample3 = sample3;
            int16_t drySample4 = sample4;

            quantizer.quantize(sample1);
            quantizer.quantize(sample2);
            quantizer.quantize(sample3);
            quantizer.quantize(sample4);

            //mix dry and wet signals
            sample1 = signed_saturate_rshift((int32_t) (drySample1 * (255 - mix)) + (int32_t) (sample1 * mix), 16, 8);
            sample2 = signed_saturate_rshift((int32_t) (drySample2 * (255 - mix)) + (int32_t) (sample2 * mix), 16, 8);
            sample3 = signed_saturate_rshift((int32_t) (drySample3 * (255 - mix)) + (int32_t) (sample3 * mix), 16, 8);
            sample4 = signed_saturate_rshift((int32_t) (drySample4 * (255 - mix)) + (int32_t) (sample4 * mix), 16, 8);

            //repack four int_16 processed samples into two uint32
            d1 = pack_16b_16b(sample1, sample2);
            d1 = pack_16b_16b(sample3, sample4);

            *start++ = d1; 
            *start++ = d2;
        }
    }
    
    transmit(block);
    release(block);
}

void BitCrusherEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    int16_t value = (int16_t) Utility::calculateParamValue(ranges[index], (float)level/65536.0f );

    switch(index) {
        case 0:
        //change bit resolution
        bitRes = value;
        quantizer.setResolution(bitRes);
        break;

        case 1:
        mix = value;
        break;

        case 2: 
        downSample = 1 << (int) value;
        break;

        case 3: 

        break;
        
        default:
        break;
    }
}