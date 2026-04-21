#include "modules/BiquadFilterModule.h"

void BiquadFilterModule::setCoeff(float f) {
	w0 = 2.0f * PI * f / AUDIO_SAMPLE_RATE;
	alpha = sin(w0) / (2.0f * Q);	//this is wrong for shelf filters
}

void BiquadFilterModule::process(int16_t& sample) {
	//y[n] = b0*x[n] + b1 * x[n-1] + b2* x[n-2] - a1 * y[n-1] - a2 * y[n-2]
    int16_t out = 0;
	int16_t curr_input = sample;

	out += signed_saturate_rshift(curr_input * b0, 16, 15);
	out += signed_saturate_rshift(prevInput * b1, 16, 15);
	out += signed_saturate_rshift(prevPrevInput * b2, 16, 15);
	out += signed_saturate_rshift(prevOutput * a1, 16, 15);
	out += signed_saturate_rshift(prevPrevOutput * a2, 16, 15);

	prevPrevInput = prevInput;
	prevInput = sample;
	prevPrevOutput = prevOutput;
	prevOutput = out;

	sample = out;
}

