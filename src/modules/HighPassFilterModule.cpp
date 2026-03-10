#include "modules/HighPassFilterModule.h"

void HighPassFilterModule::setCutoff(float freq) {
    //α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
    alpha = (int16_t) ((1 - expf(-2 * PI * freq / AUDIO_SAMPLE_RATE)) * 32767.0f);
    beta = 32767 - alpha;
}

void HighPassFilterModule::reset() {
    alpha = 0;  //value of a filter with cutoff freq of 0 (lets full frequencies through)
    beta = 32767;
    prev_input = prev_output = 0;
}

void HighPassFilterModule::process(int16_t &sample) {
    int16_t curr_input = sample;
    int16_t curr_output = signed_saturate_rshift(
        ((int32_t)prev_output + (int32_t)curr_input - (int32_t)prev_input) * beta, 16, 15
    );
    sample = curr_output;

    prev_input = curr_input; 
    prev_output = curr_output;
}