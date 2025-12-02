#include "Utility.h"

int16_t Utility::fastTanh(int16_t x) {
  bool isNeg = x < 0;
  uint16_t absValue = isNeg ? -x : x;

  uint32_t index = ((uint32_t)absValue * (TANH_ENTRIES - 1));

  // Integer part (top bits)
  uint16_t index_int  = index >> 15;
  // Fractional part (low 15 bits)
  uint16_t index_frac = index & 0x7FFF;

  if (index_int >= TANH_ENTRIES - 1) {
      index_int = TANH_ENTRIES - 2;
  }

  int32_t delta  = (int32_t)tanhLUT[index_int + 1] - (int32_t)tanhLUT[index_int];
  int32_t interp = (delta * index_frac) >> 15;
  int16_t y = tanhLUT[index_int] + interp;

  return isNeg ? -y : y;
}