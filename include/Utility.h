#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include <Audio.h>
#include "CustomRange.h"

constexpr float TANH_MAX = 3.0f;
constexpr float INV_TANH_STEP = 1/0.05f;
constexpr int TANH_ENTRIES = int(TANH_MAX * INV_TANH_STEP) + 1;
constexpr float INV_TANH_SCALE = 1.0f/32767.0f;
PROGMEM
static const int16_t tanhLUT[TANH_ENTRIES] = {
    0, 1636, 3265, 4878, 6467, 8025, 9545, 11022, 12449, 13824, 15142, 16400, 17597, 18731, 
    19803, 20811, 21758, 22644, 23470, 24240, 24955, 25617, 26229, 26795, 27316, 27795, 28236, 
    28640, 29010, 29349, 29658, 29942, 30200, 30435, 30650, 30846, 31023, 31185, 31333, 31466, 
    31588, 31698, 31798, 31889, 31972, 32046, 32114, 32176, 32232, 32282, 32328, 32369, 32407, 
    32441, 32472, 32500, 32525, 32548, 32569, 32587, 32604, 
};

class Utility {
    public:
    static float calculateParamValue(CustomRange& r, float& l);
    static float fastTanh(float x);
};

#endif