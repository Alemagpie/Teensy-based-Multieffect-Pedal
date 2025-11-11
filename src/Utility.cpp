#include "Utility.h"

float Utility::calculateParamValue(CustomRange& r, float& l) {
  return r.getMin() + l * r.getDelta();
}

float Utility::fastTanh(float x) {
  //clamp results between 0 and 3.0f
  bool isNegative = x < 0.0f;
  float value = fabsf(x);
  if(value >= TANH_MAX) {
    return isNegative? -1.0f : 1.0f;
  }
  
  //interpolate
  float index = value * INV_TANH_STEP;  //saves 1 division per call
  int roundedIndex = (int) index;
  float delta = index - roundedIndex;
  float prevValue = tanhLUT[roundedIndex] * INV_TANH_SCALE;     //saves 2 divisions per call
  float nextValue = tanhLUT[roundedIndex + 1] * INV_TANH_SCALE;
  float y = prevValue + delta * (nextValue - prevValue);

  return isNegative? -y : y;
}