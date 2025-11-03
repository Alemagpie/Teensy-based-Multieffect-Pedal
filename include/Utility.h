#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include <Audio.h>
#include "CustomRange.h"

class Utility {
    public:
    static float calculateParamValue(CustomRange& r, float& l);
};

#endif