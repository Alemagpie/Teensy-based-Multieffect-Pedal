#ifndef LPF_H
#define LPF_H

#include "Filter.h"

//one-pole low pass filter
class LowPassFilter : public Filter {
    public:
        virtual void setCutoff(float freq) override {
            // α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
            alpha = 1 - expf(-2 * PI * freq / AUDIO_SAMPLE_RATE);
        }

        virtual void reset() override {
            alpha = 0.96f;  //value of a filter with cutoff freq of half the sample rate (max limit before artifacts start appearing)
            prev_input = prev_output = 0;
        }

        virtual void filter(int16_t *sample) override {
            //y(nT) = y((n-1)T) + α*[x(nT)-y((n-1)T)]
            int16_t curr_input = *sample;
            int16_t curr_output = prev_output + alpha * (curr_input - prev_input);
            *sample = curr_output;

            prev_input = curr_input;
            prev_output = curr_output;
        }
};

#endif