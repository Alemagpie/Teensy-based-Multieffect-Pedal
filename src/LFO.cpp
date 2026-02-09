#include "LFO.h"

audio_block_t* LFO::getReadOnly() {
    audio_block_t *waveBlock = &block;
    int16_t sample;

    //fill buffer
    for(int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        ph = phase >> 17;
        switch(shapeSelect) {
            case 0:
            sample = sine();
            break;

            case 1:
            sample = square();
            break;

            case 2:
            sample = triang();
            break;

            case 3:
            sample = saw();
            break;

            case 4:
            sample = ramp();
            break;

            default:
            break;
        }

        if(modeSelect == UNIPOLAR) {
            sample = (sample + 32768) >> 1;
        }

        waveBlock->data[i] = sample;

        phase += phaseStep;
    }

    return waveBlock;
}

void LFO::setFrequency(float f) {
    if(f < 0.1f || f > 20.0f) {
        return;
    }

    phaseStep = f * (4294967296.0f / AUDIO_SAMPLE_RATE_EXACT); 
}

void LFO::setAmplitude(float a) {
    if(a < 0.0f || a > 1.0f) {
        return;
    }

    amplitude = (uint16_t) (65535.0f * a);
}

void LFO::setPhase(float angle) {
    if(angle < 0.0f || angle >= 360.0f) {
        return;
    }

    phase = angle * (float)(4294967296.0 / 360.0);
}

void LFO::setShape(short s) {
    shapeSelect = s;
}

void LFO::setMode(short m) {
    modeSelect = m;
}

int16_t LFO::sine() {
    return Utility::fastSin(ph);
}

int16_t LFO::square() {
    int16_t value = amplitude >> 1;
    return ph < 16384 ? value : -value;  
}

int16_t LFO::triang() {
    int32_t t = (ph < 16384) ? ph : (32767 - ph);
    return (((t << 2) - 32768) * amplitude) >> 16;
}

int16_t LFO::saw() {
    return (((ph << 1) - 32768) * amplitude) >> 16;
}

int16_t LFO::ramp() {
    return ((32768 - (ph << 1)) * amplitude) >> 16;
}