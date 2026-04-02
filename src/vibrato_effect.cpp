#include "vibrato_effect.h"

void VibratoEffect::update(void) {
    audio_block_t *block, *lfo_block;
    int32_t lfoOffset, readIndex;

    block = receiveWritable();
    lfo_block = lfo.getReadOnly();

    if(!block) {
        return;
    }

    /*
    LFOModule lfo_m;
    DelayLineModule dl_m;
    LowPassFilterModule lp_m;

    if(enable) {
        inputSamplePtr = block->data;
        lfoSamplePtr = lfo_block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr++);
            lfoOffset = *lfoSamplePtr;
            readOffset = baseDelay - lfoOffset;
            *inputSamplePtr = dl_m.read(readOffset);
            lp_m.filter(*inputSamplePtr);
            inputSamplePtr++;
            lfoSamplePtr++;
        }
    } else {
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            dl_m.write(*inputSamplePtr++);
        }
    }
    */

    if(enabled) {
        inputSamplePtr = block->data;
        lfoSamplePtr = lfo_block->data;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            writeIndex++;

            if(writeIndex >= VIBRATO_BUFFER_LENGHT) {
            writeIndex = 0;
            }

            sampleQueue[writeIndex] = *inputSamplePtr;

            lfoOffset = signed_saturate_rshift(depth * (*lfoSamplePtr), 16, 15);
            
                Serial.println(lfoOffset);
            readIndex = writeIndex - (baseDelay - lfoOffset);

            while(readIndex < 0) { readIndex += VIBRATO_BUFFER_LENGHT; }

            int16_t output = sampleQueue[readIndex];
            lp.filter(&output);
            *inputSamplePtr = output;
            inputSamplePtr++;
            lfoSamplePtr++;
        }
    } else {
        //store in queue
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            writeIndex++;

            if(writeIndex >= VIBRATO_BUFFER_LENGHT) {
                writeIndex = 0;
            }

            sampleQueue[writeIndex] = *inputSamplePtr++;
        }
    }

    transmit(block);
    release(block);
}

void VibratoEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            freq = value;
            lfo.setFrequency(freq);
        break;

        case 1:
            depth = (uint16_t) value;
        break;

        case 2: 
            lp.setCutoff(value);
        break;
            
        case 3: 
            
        break;
        
        default:
        break;
    }
}