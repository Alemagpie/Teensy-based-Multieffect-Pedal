#ifndef RANGE_H
#define RANGE_H

#include <Arduino.h>

//A class used to aid the process of parameter mapping parameter values
class CustomRange {
    public:
    CustomRange();
    CustomRange(float min, float max);

    float getMin();
    float getMax();
    float getDelta();
    bool isValid();

    private:
    float start;
    float end;
    float delta;
};

#endif