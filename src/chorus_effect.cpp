#include "chorus_effect.h"

#include "vibrato_effect.h"

void ChorusEffect::update(void) {
    audio_block_t *block;
    int16_t lfoOffset, voicesOffset, readIndex;
    int32_t voicesSum;

    block = receiveWritable();

    for(int k = 0; k < MAX_CHORUS_VOICES; k++) {
        lfoSamplePtrs[k] = lfos[k].getReadOnly()->data;
    }

    if(!block) {
        return;
    }

    if(enabled && voices > 1) {
        inputSamplePtr = block->data;
        voicesOffset = baseDelay / voices;

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            writeIndex++;

            if(writeIndex >= CHORUS_BUFFER_LENGHT) {
            writeIndex = 0;
            }

            sampleQueue[writeIndex] = *inputSamplePtr;

            //for each voice find the detuned sample
            for(int j = 0; j < voices; j++) {
                lfoOffset = signed_saturate_rshift(
                (int32_t)depth * (*lfoSamplePtrs[j]),
                16,
                15);

            int16_t delay = baseDelay + voicesOffset * j + lfoOffset;

            readIndex = writeIndex - delay;

            if(readIndex < 0)
                readIndex += CHORUS_BUFFER_LENGHT;

                voicesSum += sampleQueue[readIndex];
            }

            //normalize sum of voices 
            *inputSamplePtr = (*inputSamplePtr * (255 - mix) + (voicesSum / voices) * mix)>>8;
            inputSamplePtr++;

            for(int k = 0; k < MAX_CHORUS_VOICES; k++) {
                lfoSamplePtrs[k]++;
            }
        }
    } else {
        //store in queue
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            writeIndex++;

            if(writeIndex >= CHORUS_BUFFER_LENGHT) {
            writeIndex = 0;
            }

            sampleQueue[writeIndex] = *inputSamplePtr++;
        }
    }

    transmit(block);
    release(block);
}

void ChorusEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float value = Utility::calculateParamValue(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            freq = value;
            for (auto &&i : lfos)
            {
                i.setFrequency(freq);
            }
        break;

        case 1:
            depth = (uint16_t) value;
        break;

        case 2: 
            voices = (uint16_t) value;
        break;

        case 3: 
            mix = value;
        break;
        
        default:
        break;
    }
}