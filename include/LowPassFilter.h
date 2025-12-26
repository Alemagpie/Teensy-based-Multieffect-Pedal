#ifndef LPF_H
#define LPF_H

#include "Filter.h"

//one-pole low pass filter
class LowPassFilter : public Filter {
    public:
    LowPassFilter() { reset(); }
    virtual void setCutoff(float freq) override;
    virtual void reset() override;
    virtual void filter(int16_t *sample) override;
};

#endif