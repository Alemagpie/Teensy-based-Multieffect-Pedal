#include "Utility.h"

float Utility::fastTanh(int16_t x) {
  /*
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
  */

  bool isNeg = x < 0;
  uint16_t absValue = (x < 0) ? -x : x;

  uint32_t index = ((uint32_t)absValue * (TANH_ENTRIES - 1)) >> 15;
  uint16_t index_int  = index >> 15;                          
  uint16_t index_frac = index & 0x7FFF;    

  int32_t delta = (int32_t)tanhLUT[index_int + 1] - (int32_t)tanhLUT[index_int];
  int32_t interp = (delta * index_frac) >> 15;
  int16_t y = tanhLUT[index_int] + interp;

  return isNeg ? -y : y;
}