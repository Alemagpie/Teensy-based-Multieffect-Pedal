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
	void setQ(float q) { Q = q; }

    void process(int16_t& sample);
    inline void reset() override { setQ(0.707f); }

    private:
	int16_t prevPrevInput, prevInput, prevPrevOutput, prevOutput;
	int32_t b0, b1, b2, a1, a2;
	float w0, sinw0, cosw0, alpha, Q, S, dBGain, a0;

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

static inline int32_t floatToQ29Sat(float val) {
	float scaled = val * 536870912.0f;

	//saturate32
	if(scaled > 2147483647.0f) return 2147483647;
	else if(scaled < -2147483648.0f) return -2147483648;
	return (int32_t) scaled;
}

#endif