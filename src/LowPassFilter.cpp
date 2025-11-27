#include "LowPassFilter.h"

void LowPassFilter::setCutoff(float freq) {
    // α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
    alpha = (1 - expf(-2 * PI * freq / AUDIO_SAMPLE_RATE))*32767.0f;
}

void LowPassFilter::reset() {
    alpha = 0.96f;  //value of a filter with cutoff freq of around 22kHz (full frequency spectrum)
    prev_input = prev_output = 0;
} 

void LowPassFilter::filter(int16_t *sample) {
    //y(nT) = y((n-1)T) + α*[x(nT)-y((n-1)T)]
    int16_t curr_input = *sample;
    int16_t curr_output = saturate16(
        (int32_t)prev_output + signed_saturate_rshift(alpha * saturate16((int32_t)curr_input - (int32_t)prev_input), 16, 15)
    ); //since alpha (or beta) is the float coefficient * 32767, should i also rshift by 15?

    *sample = curr_output;

    prev_input = curr_input;
    prev_output = curr_output;
}