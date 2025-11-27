#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>
#include <dspinst.h>
#include <AudioStream.h> 

class Filter {
    public:
        virtual void setCutoff(float freq) = 0;
        virtual void reset() = 0;
        virtual void filter(int16_t *sample) = 0; 

    protected:
        int16_t alpha;
        int16_t prev_input = 0;
        int16_t prev_output = 0;
};

#endif