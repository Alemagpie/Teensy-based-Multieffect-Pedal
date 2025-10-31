#ifndef RANGE_H
#define RANGE_H

#include <Arduino.h>

class Range {
    public:
    Range();
    Range(float min, float max);

    float getMin();
    float getMax();
    float getDelta();

    private:
    float start;
    float end;
    float delta;
};

#endif