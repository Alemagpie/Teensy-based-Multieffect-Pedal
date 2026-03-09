#include "modules/SampleAndHoldModule.h"

void SampleAndHoldModule::process(int16_t &value) {
    if(holdSampleCount > 1) {
        if(hold == 0) {
            hold = holdSampleCount;
            holdSample = value;
        }

        value = holdSample;
        hold--;
    }
}