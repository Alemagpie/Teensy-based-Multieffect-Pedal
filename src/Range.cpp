#include "Range.h"

Range::Range() {
    start = end = NULL;
}

Range::Range(float min, float max) {
    if(min >= max) {
        Serial.println("Invalid argument!");
        return;
    }

    start = min;
    end = max;
    delta = std::abs(max-min);
}

float Range::getMin() {return start;}
float Range::getMax() {return end;}
float Range::getDelta() {return delta;}