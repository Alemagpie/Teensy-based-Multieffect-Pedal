#ifndef HPF_H
#define HPF_H

#include "Filter.h"

//one-pole high pass filter
class HighPassFilter : public Filter {
    public:
        HighPassFilter() { reset(); }
        virtual void setCutoff(float freq) override;
        virtual void reset() override;
        virtual void filter(int16_t *sample) override;

    private:
    int16_t beta;
};

#endif