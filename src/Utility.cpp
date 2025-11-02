#include "Utility.h"

float Utility::calculateParamValue(Range& r, float& l) {
  return r.getMin() + l * r.getDelta();
}