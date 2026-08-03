#include "effects/squarer_effect.h"

void SquarerEffect::update(void) {
    audio_block_t *block;
    int32_t lfoOffset;
    int32_t voicesSum;

    block = receiveWritable();

    if(!block) {
        return;
    }


    if(enabled) {
        inputSamplePtr = block->data;
        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            int16_t d, s1, s2, out;

            //Dry sample
            d = *inputSamplePtr;

            //Same octave square wave
            if(d > threshold && isNeg) {
                s1 = 32767;
                crossings++;
            } else if(d < -threshold && !isNeg) {
                s1 = -32767;
                crossings++;
            }

            //Down octave square wave
            if((crossings % 2 == 0) && crossings != 0) {
                crossings = 0;
                dwnOctaveState = !dwnOctaveState;
                s2 = dwnOctaveState ? 32767 : -32767;
            }
            
            out = saturate16(2 * mx_m.process(d, s1, s2));  //Makeup gain of 2
            lp_m.process(out);
            *inputSamplePtr = out;
            inputSamplePtr++;
        }
    } 

    transmit(block);
    release(block);
}

void SquarerEffect::setParamLevel(int writeIndex, uint16_t level) {
    if(writeIndex < 0 || writeIndex > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[writeIndex] = level;

    float valueLin = Utility::calculateParamValueLin(ranges[writeIndex], (float)level/65536.0f );
    float valueLog = Utility::calculateParamValueLog(ranges[writeIndex], (float)level/65536.0f );

    switch(writeIndex) {
        case 0:
            dry = (uint8_t) valueLin;
            mx_m.setGain(0, dry);
        break;

        case 1:
            sq1 = (uint8_t) valueLin;
            mx_m.setGain(1, sq1);
        break;

        case 2: 
            sq2 = (uint8_t) valueLin;
            mx_m.setGain(2, sq2);
        break;

        case 3: 
            lp_m.setCutoff(valueLog);
        break;
        
        default:
        break;
    }
}