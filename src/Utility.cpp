#include "Utility.h"

float Utility::calculateParamValue(CustomRange& r, float& l) {
  return r.getMin() + l * r.getDelta();
}