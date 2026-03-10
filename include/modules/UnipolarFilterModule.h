#ifndef UNI_FILTER_MODULE
#define UNI_FILTER_MODULE

#include <dspinst.h>
#include "Module.h"

class UnipolarFilterModule : Module {
    public:
        virtual void setCutoff(float freq) = 0;
        virtual inline void reset() = 0;
        virtual inline void process(int16_t &sample) = 0; 

    protected:
        int16_t alpha;
        int16_t prev_input = 0; 
        int16_t prev_output = 0;
};

#endif