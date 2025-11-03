#ifndef EFFECT_HANDLER_H
#define EFFECT_HANDLER_H

#include <string>
#include <vector>
#include "Utility.h"
#include "CustomRange.h"
#include <initializer_list>

class Adapter_EffectHandler {
    public:
    Adapter_EffectHandler();
    Adapter_EffectHandler(std::initializer_list<CustomRange> r);

    float getParamLevel(int index);
    virtual void setParamLevel(int index, float level) = 0;
    virtual void init(float p1, float p2, float p3, float p4) = 0;

    protected:
    std::vector<CustomRange> ranges = {CustomRange(), CustomRange()};
    std::vector<float> levels = {0, 0};
    std::string name;
    static const int parameterCount = 2;
};

#endif