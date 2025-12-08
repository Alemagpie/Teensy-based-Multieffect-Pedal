#include "CustomRange.h"

CustomRange::CustomRange() {
    start = end = delta = 0.0;
}

CustomRange::CustomRange(float min, float max) {
    if(min >= max) {
        Serial.println("Invalid argument!");
        return;
    }

    start = min;
    end = max;
    delta = std::abs(max-min);
}

float CustomRange::getMin() {return start;}
float CustomRange::getMax() {return end;}
float CustomRange::getDelta() {return delta;}

bool CustomRange::isValid() {return start == end;}