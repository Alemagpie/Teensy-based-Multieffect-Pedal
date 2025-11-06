#ifndef EFFECT_ADAPTER_H
#define EFFECT_ADAPTER_H

#include <string>
#include <vector>
#include "Utility.h"
#include "CustomRange.h"
#include <initializer_list>

class EffectAdapter {
    public:
    //EffectAdapter();
    //EffectAdapter(std::initializer_list<CustomRange> r) : ranges(r) {};
    virtual ~EffectAdapter() = default;

    virtual float getParamLevel(int index) const = 0; //{return (index < 0 || index > parameterCount - 1) ? 0 : levels[index];}
    virtual void setParamLevel(int index, float level) = 0;
    virtual void init(float p1, float p2, float p3, float p4) = 0;

    //protected:
    //std::vector<CustomRange> ranges;
    //std::vector<float> levels;
    //std::string name;

    static const int parameterCount = 2;
};

#endif