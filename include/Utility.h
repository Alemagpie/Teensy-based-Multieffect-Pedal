#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include <Audio.h>
#include "Range.h"

class Utility {
    public:
    static float calculateParamValue(Range& r, float& l);
};

#endif