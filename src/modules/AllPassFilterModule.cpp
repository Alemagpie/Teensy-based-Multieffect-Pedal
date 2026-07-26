#include "modules/AllPassFilterModule.h"

void AllPassFilterModule::setCutoff(float freq) {
    alpha = (int16_t) ((1 - tanf(PI * freq / 44100.0f)) / (1 + tanf(PI * freq / 44100.0f)) * 32767.0f);
}

void AllPassFilterModule::process(int16_t &sample) {
    //y[n] = -a*x[n] + x[n-1] + a*y[n-1]
    int16_t curr_input = sample;
    int16_t curr_output = saturate16(
        signed_saturate_rshift(-alpha * curr_input, 16, 15) +
        prev_input +
        signed_saturate_rshift(alpha * prev_output, 16, 15)
    );

    sample = curr_output;
    prev_input = curr_input;
    prev_output = curr_output;
}

void AllPassFilterModule::reset() {
    prev_input = prev_output = 0;
}