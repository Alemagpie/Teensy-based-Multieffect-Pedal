#ifndef BQ_FILTER_MODULE
#define BQ_FILTER_MODULE

#include <dspinst.h>
#include "Module.h"

//y[n] = b0*x[n] + b1 * x[n-1] + b2* x[n-2] - a1 * y[n-1] - a2 * y[n-2]
class BiquadFilterModule : Module {
    public:
	BiquadFilterModule() { reset(); }

	void setCutoff(float f);
	void setCoeff(float f);
	void setHighPass(float f);
	void setLowPass(float f);
	void setNotch(float f);
	void setBandPass(float f);
	void setLowShelf(float f);
	void setHighShelf(float f);

	void setMode(uint8_t m);

    void process(int16_t& sample);
    inline void reset() override {  }

    private:
	int16_t prevPrevInput, prevInput, prevPrevOutput, prevOutput;
	int32_t b0, b1, b2, a1, a2;
	float w0, sinw0, cosw0, alpha, Q, S, dBGain, a0;
	const float SCALE = 536870912.0f; 	//To go from float to Q29

	enum mode {
		LOWPASS = 0,
		HIGHPASS = 1,
		NOTCH = 2,
		BANDPASS = 3,
		LOWSHELF = 4,
		HIGHSHELF = 5
	};
	mode status = LOWPASS;
};

#endif