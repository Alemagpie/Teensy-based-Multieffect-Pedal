#include "modules/LowPassFilterModule.h"

void LowPassFilterModule::setCutoff(float freq) {
    // α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
    alpha = (int16_t) ((1 - expf(-2 * PI * freq / AUDIO_SAMPLE_RATE)) * 32767.0f);
}

void LowPassFilterModule::reset() {
    alpha = 31456;  //value of a filter with cutoff freq of around 22kHz (full frequency spectrum)
    prev_input = prev_output = 0;
} 

void LowPassFilterModule::process(int16_t &sample) {
    //y(nT) = y((n-1)T) + α*[x(nT)-y((n-1)T)]
    int16_t curr_input = sample;
    int16_t curr_output = saturate16(prev_output + signed_saturate_rshift(
        (int32_t)(curr_input - prev_output) * alpha, 16, 15
    ));

    sample = curr_output; 

    prev_input = curr_input;
    prev_output = curr_output;
}