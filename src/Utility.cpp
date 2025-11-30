#include "Utility.h"

int16_t Utility::fastTanh(int16_t x) {
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

  /*
  bool isNeg = x < 0;
  uint16_t absValue = (x < 0) ? -x : x;

  uint32_t index = ((uint32_t)absValue * (TANH_ENTRIES - 1)) >> 15;
  uint16_t index_int  = index >> 15;                          
  uint16_t index_frac = index & 0x7FFF;    

  int32_t delta = (int32_t)tanhLUT[index_int + 1] - (int32_t)tanhLUT[index_int];
  int32_t interp = (delta * index_frac) >> 15;
  int16_t y = tanhLUT[index_int] + interp;

  return isNeg ? -y : y;
  */

  bool isNeg = x < 0;
uint16_t absValue = isNeg ? -x : x;

// Q15 fixed-point index
uint32_t idx = ((uint32_t)absValue * (TANH_ENTRIES - 1));

// Integer part: top bits
uint16_t index_int  = idx >> 15;

// Fractional part: low 15 bits
uint16_t index_frac = idx & 0x7FFF;

// Bounds check (avoid reading LUT[TANH_ENTRIES])
if (index_int >= TANH_ENTRIES - 1)
    index_int = TANH_ENTRIES - 2;

int32_t delta  = (int32_t)tanhLUT[index_int + 1] - (int32_t)tanhLUT[index_int];
int32_t interp = (delta * index_frac) >> 15;
int16_t y      = tanhLUT[index_int] + interp;

return isNeg ? -y : y;
}