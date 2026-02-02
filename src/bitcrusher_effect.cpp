#include "bitcrusher_effect.h"

void BitCrusherEffect::update(void) {
    audio_block_t *block;
    uint32_t *start, *end;
    //two parts of block->data
    uint32_t d1, d2;

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

            int16_t drySample1 = sample1;
            int16_t drySample2 = sample2;
            int16_t drySample3 = sample3;
            int16_t drySample4 = sample4;
            
            //Downsampling
            if(downSample > 1) {
                if(holdSampleCount == 0) {
                    holdSampleCount = downSample;
                    holdSample = sample1;
                }

                if(holdSampleCount >= 4) {
                    sample4 = sample3 = sample2 = sample1 = holdSample;
                    holdSampleCount -= 4;
                } else {
                    switch(holdSampleCount) {
                        case 1:
                        sample1 = holdSample;
                        holdSample = sample3 = sample4 = sample2;
                        holdSampleCount = downSample - 3;   
                        break;

                        case 2:
                        sample1 = sample2 = holdSample;
                        holdSample = sample4 =  sample3;
                        holdSampleCount = downSample - 2;   
                        break;

                        case 3:
                        sample1 = sample2 = sample3 = holdSample;
                        holdSample = sample4;
                        holdSampleCount = downSample - 1;   
                        break;
                    }
                }
            }

            //Quantize signal
            quant.quantize(sample1);
            quant.quantize(sample2);
            quant.quantize(sample3);
            quant.quantize(sample4);

            //mix dry and wet signals
            sample1 = signed_saturate_rshift((int32_t) (drySample1 * (255 - mix)) + (int32_t) (sample1 * mix), 16, 8);
            sample2 = signed_saturate_rshift((int32_t) (drySample2 * (255 - mix)) + (int32_t) (sample2 * mix), 16, 8);
            sample3 = signed_saturate_rshift((int32_t) (drySample3 * (255 - mix)) + (int32_t) (sample3 * mix), 16, 8);
            sample4 = signed_saturate_rshift((int32_t) (drySample4 * (255 - mix)) + (int32_t) (sample4 * mix), 16, 8);

            //Filter high end of signal
            lp.filter(&sample1);
            lp.filter(&sample2);
            lp.filter(&sample3);
            lp.filter(&sample4);

            //repack four int_16 processed samples into two uint32
            d1 = pack_16b_16b(sample1, sample2);
            d2 = pack_16b_16b(sample3, sample4);

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

    int16_t value = (int16_t) Utility::calculateParamValue(ranges[index], (float)level/65535.0f );

    switch(index) {
        case 0:
        bitRes = value;
        quant.setResolution(bitRes);
        break;

        case 1:
        downSample = value;
        Serial.print(value);
        break;

        case 2: 
        mix = value;
        break;

        case 3: 
        //change bit resolution (doesnt do anything ad of rn)
        lp.setCutoff(value);
        break;
        
        default:
        break;
    }
}