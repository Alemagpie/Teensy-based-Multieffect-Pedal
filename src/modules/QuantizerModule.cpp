#include "modules/QuantizerModule.h"

void QuantizerModule::process(int16_t &value) {
    bool isNeg = value < 0 ? true : false;
    int16_t absValue = isNeg ? -value : value;
    uint16_t index = absValue / intervalLenght;

    int16_t q = saturate16(index * intervalLenght + (intervalLenght >> 1));

    if(isNeg) {
        q *= -1;    //quantizer is uniform thus symmetrical
    }
    value = q;
}