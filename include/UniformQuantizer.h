#ifndef QUANTIZER_H
#define QUANTIZER_H

#include <Arduino.h>
#include <dspinst.h>
#include <AudioStream.h> 

class UniformQuantizer {
    public:
        UniformQuantizer() { setResolution(1); }

        void setResolution(uint8_t b);
        void quantize(int16_t &x);

    private:
        uint8_t bitResolution = 1;
        uint16_t intervalLenght;
};

#endif