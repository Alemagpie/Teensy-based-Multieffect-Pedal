#ifndef EFFECT_HANDLER_H
#define EFFECT_HANDLER_H

#include <string>
#include <vector>
#include "Utility.h"
#include "CustomRange.h"

class EffectHandler {
    public:
    EffectHandler();
    EffectHandler(std::initializer_list<CustomRange> r);

    float getParamLevel(int index);
    virtual void setParamLevel(int index, float level) = 0;

    protected:
    std::vector<CustomRange> ranges = {CustomRange(), CustomRange()};
    std::vector<float> levels = {0, 0};
    std::string name;
    static const int parameterCount = 2;
};

#endif