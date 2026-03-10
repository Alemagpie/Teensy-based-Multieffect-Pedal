#ifndef BQ_FILTER_MODULE
#define BQ_FILTER_MODULE

#include <dspinst.h>
#include "Module.h"

class BiquadFilterModule : Module {
    public:
	BiquadFilterModule() { reset(); }

	void setCoefficients(uint32_t stage, const int *coefficients);
	void setCoefficients(uint32_t stage, const double *coefficients) {
		int coef[5];
		coef[0] = coefficients[0] * 1073741824.0;
		coef[1] = coefficients[1] * 1073741824.0;
		coef[2] = coefficients[2] * 1073741824.0;
		coef[3] = coefficients[3] * 1073741824.0;
		coef[4] = coefficients[4] * 1073741824.0;
		setCoefficients(stage, coef);
	}

	void setLowpass(uint32_t stage, float frequency, float q = 0.7071f) {
		int coef[5];
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		double alpha = sinW0 / ((double)q * 2.0);
		double cosW0 = cos(w0);
		double scale = 1073741824.0 / (1.0 + alpha);
		/* b0 */ coef[0] = ((1.0 - cosW0) / 2.0) * scale;
		/* b1 */ coef[1] = (1.0 - cosW0) * scale;
		/* b2 */ coef[2] = coef[0];
		/* a1 */ coef[3] = (-2.0 * cosW0) * scale;
		/* a2 */ coef[4] = (1.0 - alpha) * scale;
		setCoefficients(stage, coef);
	}
	void setHighpass(uint32_t stage, float frequency, float q = 0.7071) {
		int coef[5];
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		double alpha = sinW0 / ((double)q * 2.0);
		double cosW0 = cos(w0);
		double scale = 1073741824.0 / (1.0 + alpha);
		/* b0 */ coef[0] = ((1.0 + cosW0) / 2.0) * scale;
		/* b1 */ coef[1] = -(1.0 + cosW0) * scale;
		/* b2 */ coef[2] = coef[0];
		/* a1 */ coef[3] = (-2.0 * cosW0) * scale;
		/* a2 */ coef[4] = (1.0 - alpha) * scale;
		setCoefficients(stage, coef);
	}
	void setBandpass(uint32_t stage, float frequency, float q = 1.0) {
		int coef[5];
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		double alpha = sinW0 / ((double)q * 2.0);
		double cosW0 = cos(w0);
		double scale = 1073741824.0 / (1.0 + alpha);
		/* b0 */ coef[0] = alpha * scale;
		/* b1 */ coef[1] = 0;
		/* b2 */ coef[2] = (-alpha) * scale;
		/* a1 */ coef[3] = (-2.0 * cosW0) * scale;
		/* a2 */ coef[4] = (1.0 - alpha) * scale;
		setCoefficients(stage, coef);
	}
	void setNotch(uint32_t stage, float frequency, float q = 1.0) {
		int coef[5];
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		double alpha = sinW0 / ((double)q * 2.0);
		double cosW0 = cos(w0);
		double scale = 1073741824.0 / (1.0 + alpha);
		/* b0 */ coef[0] = scale;
		/* b1 */ coef[1] = (-2.0 * cosW0) * scale;
		/* b2 */ coef[2] = coef[0];
		/* a1 */ coef[3] = (-2.0 * cosW0) * scale;
		/* a2 */ coef[4] = (1.0 - alpha) * scale;
		setCoefficients(stage, coef);
	}
	void setLowShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
		int coef[5];
		double a = pow(10.0, gain/40.0f);
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		//double alpha = (sinW0 * sqrt((a+1/a)*(1/slope-1)+2) ) / 2.0;
		double cosW0 = cos(w0);
		//generate three helper-values (intermediate results):
		double sinsq = sinW0 * sqrt( (pow(a,2.0)+1.0)*(1.0/(double)slope-1.0)+2.0*a );
		double aMinus = (a-1.0)*cosW0;
		double aPlus = (a+1.0)*cosW0;
		double scale = 1073741824.0 / ( (a+1.0) + aMinus + sinsq);
		/* b0 */ coef[0] =		a *	( (a+1.0) - aMinus + sinsq	) * scale;
		/* b1 */ coef[1] =  2.0*a * ( (a-1.0) - aPlus  			) * scale;
		/* b2 */ coef[2] =		a * ( (a+1.0) - aMinus - sinsq 	) * scale;
		/* a1 */ coef[3] = -2.0*	( (a-1.0) + aPlus			) * scale;
		/* a2 */ coef[4] =  		( (a+1.0) + aMinus - sinsq	) * scale;
		setCoefficients(stage, coef);
	}
	void setHighShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
		int coef[5];
		double a = pow(10.0, gain/40.0f);
		double w0 = frequency * (2.0f * 3.141592654f / AUDIO_SAMPLE_RATE_EXACT);
		double sinW0 = sin(w0);
		//double alpha = (sinW0 * sqrt((a+1/a)*(1/slope-1)+2) ) / 2.0;
		double cosW0 = cos(w0);
		//generate three helper-values (intermediate results):
		double sinsq = sinW0 * sqrt( (pow(a,2.0)+1.0)*(1.0/(double)slope-1.0)+2.0*a );
		double aMinus = (a-1.0)*cosW0;
		double aPlus = (a+1.0)*cosW0;
		double scale = 1073741824.0 / ( (a+1.0) - aMinus + sinsq);
		/* b0 */ coef[0] =		a *	( (a+1.0) + aMinus + sinsq	) * scale;
		/* b1 */ coef[1] = -2.0*a * ( (a-1.0) + aPlus  			) * scale;
		/* b2 */ coef[2] =		a * ( (a+1.0) + aMinus - sinsq 	) * scale;
		/* a1 */ coef[3] =  2.0*	( (a-1.0) - aPlus			) * scale;
		/* a2 */ coef[4] =  		( (a+1.0) - aMinus - sinsq	) * scale;
		setCoefficients(stage, coef);
	}

    void process(audio_block_t *block);

    inline void reset() override { for (int i=0; i<32; i++) definition[i] = 0; }

    private:
	    int32_t definition[32];
};

#endif