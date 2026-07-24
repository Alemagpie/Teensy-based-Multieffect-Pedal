#ifndef AP_FILTER_MODULE
#define AP_FILTER_MODULE

#include "UnipolarFilterModule.h"

class AllPassFilterModule : UnipolarFilterModule {
    //y[n] = -a*x[n] + x[n-1] + a*y[n-1]
    public:
        AllPassFilterModule() { reset(); }
        AllPassFilterModule(float f) { setCutoff(f); }

        void setCutoff(float freq) override;
        void reset() override;
        void process(int16_t &sample) override;
};

#endif
