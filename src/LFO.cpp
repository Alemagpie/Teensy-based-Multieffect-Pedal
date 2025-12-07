#include "LFO.h"

audio_block_t* LFO::sendReadOnly() {
    audio_block_t *waveBlock = &block;

    for(int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        switch(shapeSelect) {
            case 0:
            waveBlock->data[i] = sine();
            break;

            case 1:
            waveBlock->data[i] = square();
            break;

            case 2:
            waveBlock->data[i] = triang();
            break;

            case 3:
            waveBlock->data[i] = saw();
            break;

            case 4:
            waveBlock->data[i] = ramp();
            break;

            default:
            break;
        }
    }

    return waveBlock;
}

void LFO::setFrequency(float f) {
    if(f < 0.1f || f > 20.0f) {
        return;
    }

    //modify phaseStep
}

void LFO::setAmplitude(float a) {
    if(a < 0.0f || a > 1.0f) {
        return;
    }

    amplitude = (int16_t) (32767.0f * a);
}

int16_t LFO::sine() {
    //use LUT and interpolation
}

int16_t LFO::square() {
    return ph < 16384 ? amplitude : 0;  //no full swing, just on or off
}

int16_t LFO::triang() {
    int32_t t = ph < 16384 ? ph : (32767 - ph);
    return signed_saturate_rshift(((t<<2) - 32767) * amplitude, 16, 15);
}

int16_t LFO::saw() {
    return signed_saturate_rshift((32767 - (ph<<1)) * amplitude, 16, 15);
}

int16_t LFO::ramp() {
    return signed_saturate_rshift(((ph<<1) - 32767) * amplitude, 16, 15);
}