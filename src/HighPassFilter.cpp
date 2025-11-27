#include "HighPassFilter.h"

void HighPassFilter::setCutoff(float freq) {
    //α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
    alpha = (1 - expf(-2 * PI * freq / AUDIO_SAMPLE_RATE))*32767.0f;
}

void HighPassFilter::reset() {
    alpha = 0.0f;  //value of a filter with cutoff freq of 0 (lets full frequencies through)
    prev_input = prev_output = 0;
}

void HighPassFilter::filter(int16_t *sample) {
    //y(nT) = (1 - α)*y((n-1)T) + (1-α)[x(nT) - x((n-1)T)]
    int16_t curr_input = *sample;
    int16_t beta = saturate16(32767 - (int32_t)alpha);
    int16_t curr_output = signed_saturate_rshift(
        beta * saturate16((int32_t)curr_input - (int32_t)prev_input + (int32_t)prev_output),    
    16, 15); //since alpha (or beta) is the float coefficient * 32767, should i also rshift by 15?

    *sample = curr_output;

    prev_input = curr_input;
    prev_output = curr_output;
}