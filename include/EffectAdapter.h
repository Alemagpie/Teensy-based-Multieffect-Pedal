#ifndef EFFECT_ADAPT_H
#define EFFECT_ADAPT_H

#include <Range.h>
#include <string>
#include "Utility.h"
#include "Range.h"

class EffectAdapter {
    public:
    EffectAdapter();
    EffectAdapter(std::initializer_list<Range> r);

    float getParamLevel(int index);
    virtual void setParamLevel(int index, float level) = 0;

    protected:
    std::vector<Range> ranges = {Range(), Range()};
    std::vector<float> levels = {0, 0};
    std::string name;
    static const int parameterCount = 2;
};

#endif