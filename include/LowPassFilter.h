#ifndef LPF_H
#define LPF_H

#include <Arduino.h>
#include <AudioStream.h> 

//y(nT) = y((n-1)T) + α*[x(nT)-y((n-1)T)]

// α = 1 - e^(-2*pi*fc/Fs) -> fc = cutoff freq, Fs = sample rate
#define x AUDIO_SAMPLE_RATE

class LowPassFilter {
    public:
        void setCutoff(float freq);
        void reset();
        void filter();

    private:
        float aplha;
        bool hasPreviousSample;
        int16_t prev_input;
        int16_t prev_output;
};

#endif