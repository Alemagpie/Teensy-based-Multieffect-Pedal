#include "UniformQuantizer.h"

void UniformQuantizer::setResolution(uint8_t b) {
    if(b < 1 || b > 15) {
        bitResolution = 1;
    }

    intervalLenght = 1 << (15 - bitResolution); //32768 / 2^bit = 2^(15-bit)
}

void UniformQuantizer::quantize(int16_t &x) {
    bool isNeg = x < 0 ? true : false;
    int16_t absValue = isNeg ? -x : x;
    uint16_t index = absValue / intervalLenght;

    int16_t q = (index * intervalLenght) + (intervalLenght >> 1);   //no need for saturation functions since index * intervalLenght can never exceed the Q15 max values

    if(isNeg) {
        q *= -1;    //quantizer is uniform thus symmetrical
    }

    x = q;
}