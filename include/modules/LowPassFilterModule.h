#ifndef LP_FILTER_MODULE
#define LP_FILTER_MODULE

#include "UnipolarFilterModule.h"

class LowPassFilterModule : UnipolarFilterModule {
    public:
        LowPassFilterModule() { reset(); }
        LowPassFilterModule(float f) { setCutoff(f); }

        void setCutoff(float freq) override;
        void reset() override;
        void process(int16_t &sample) override;
};

#endif