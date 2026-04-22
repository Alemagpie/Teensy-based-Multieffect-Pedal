#include "modules/BiquadFilterModule.h"

void BiquadFilterModule::setMode(uint8_t m) {
	if(m < 6) {
		switch(m) {
			case 0:
				status = LOWPASS;
			break;

			case 1:
				status = HIGHPASS;
			break;

			case 2:
				status = NOTCH;
			break;

			case 3:
				status = BANDPASS;
			break;

			case 4:
				status = LOWSHELF;
			break;

			case 5:
				status = HIGHSHELF;
			break;
		}
	}
}

void BiquadFilterModule::setCutoff(float f) {
	setCoeff(f);

	switch(status) {
		case 0:
			setLowPass(f);
		break;

		case 1:
			setHighPass(f);
		break;

		case 2:
			setNotch(f);
		break;

		case 3:
			setBandPass(f);
		break;

		case 4:
			setLowShelf(f);
		break;

		case 5:
			setHighShelf(f);
		break;

		default:
		break;
	}
}

void BiquadFilterModule::setCoeff(float f) {
	w0 = 2.0f * PI * f / AUDIO_SAMPLE_RATE;
	sinw0 = sin(w0);
	cosw0 = cos(w0);
	alpha = sin(w0) / (2.0f * Q);	//this is wrong for shelf filters
	a0 = 1.0f + alpha;
}

void BiquadFilterModule::setHighPass(float f) {
	b0 = (int32_t)(((1.0f + cosw0) / 2.0f) / a0 * SCALE);
	b1 = (int32_t)(-(1.0f + cosw0) / a0 * SCALE);
	b2 = b0;
	a1 = (int32_t)((-2.0f * cosw0) / a0 * SCALE);
	a2 = (int32_t)(( 1.0f - alpha) / a0 * SCALE);
}

void BiquadFilterModule::setLowPass(float f) {
	b0 = (int32_t)(((1.0f - cosw0) / 2.0f) / a0 * SCALE);
	b1 = (int32_t)(( 1.0f - cosw0) / a0 * SCALE);
	b2 = b0;
	a1 = (int32_t)((-2.0f * cosw0) / a0 * SCALE);
	a2 = (int32_t)(( 1.0f - alpha) / a0 * SCALE);
}

void BiquadFilterModule::setNotch(float f) {
	b0 = (int32_t)(1.0f/ a0 * SCALE);
	b1 = (int32_t)((-2.0f * cosw0) / a0 * SCALE);
	b2 = b0;
	a1 = (int32_t)((-2.0f * cosw0) / a0 * SCALE);
	a2 = (int32_t)(( 1.0f - alpha) / a0 * SCALE);
}

void BiquadFilterModule::setBandPass(float f) {
	b0 = (int32_t)(( sinw0 / 2.0f) / a0 * SCALE);
	b1 = 0;
	b2 = (int32_t)((-sinw0 / 2.0f) / a0 * SCALE);
	a1 = (int32_t)((-2.0f * cosw0) / a0 * SCALE);
	a2 = (int32_t)(( 1.0f - alpha) / a0 * SCALE);
}

void BiquadFilterModule::setLowShelf(float f) {
	//Not yet implemented, need more calculations as well as shelf slope (s)
}

void BiquadFilterModule::setHighShelf(float f) {
	//Not yet implemented, need more calculations as well as shelf slope (s)
}

void BiquadFilterModule::process(int16_t& sample) {
	//y[n] = b0*x[n] + b1 * x[n-1] + b2* x[n-2] - a1 * y[n-1] - a2 * y[n-2]
    int64_t sum = 0;
	int16_t out;
	int16_t curr_input = sample;

	sum += (int64_t) b0 * curr_input;
	sum += (int64_t) b1 * prevInput;
	sum += (int64_t) b2 * prevPrevInput;
	sum -= (int64_t) a1 * prevOutput;
	sum -= (int64_t) a2 * prevPrevOutput;

	out = signed_saturate_rshift(sum, 16, 29);

	prevPrevInput = prevInput;
	prevInput = sample;
	prevPrevOutput = prevOutput;
	prevOutput = out;

	sample = out;
}

