#ifndef HP_FILTER_MODULE
#define HP_FILTER_MODULE

#include "UnipolarFilterModule.h"

class HighPassFilterModule : UnipolarFilterModule {
    public:
        HighPassFilterModule() { reset(); }
        HighPassFilterModule(float f) { setCutoff(f); }

        void setCutoff(float freq) override;
        void reset() override;
        void process(int16_t &sample) override;

    private:
        int16_t beta;
};

#endif