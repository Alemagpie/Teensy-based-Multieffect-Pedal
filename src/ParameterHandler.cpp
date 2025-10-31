#include "ParameterAdapter.h"

ParameterAdapter::ParameterAdapter(std::vector<float>& p, std::vector<Range>& r) : param(p), range(r) {}

float ParameterAdapter::getParamLevel(int index) {
    if(index < 0 || index > parameterCount - 1) {
        Serial.println("Invalid argument");
        return NULL;
    }

    return (std::abs(param[index] - range[index].getMin()))/range[index].getDelta();
}

void ParameterAdapter::setParamLevel(int index, float level) {
    if(level > 1 || level < 0 || index < 0 || index > parameterCount - 1) {
        Serial.println("Invalid argument");
        return;
    }

    //needs receiving function (ex. the number of chorus voices goes from 1 to inf, so setting it to a float between 0 and 1 won't work)
    param[index] = range[index].getMin() + level * range[index].getDelta();         
}