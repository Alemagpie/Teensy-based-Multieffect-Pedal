#ifndef EFFECT_ADAPT_H
#define EFFECT_ADAPT_H

#include <Range.h>
#include <string>
#include "Utility.h"
#include "Range.h"

class EffectAdapter {
    public:
    EffectAdapter();
    EffectAdapter(std::vector<Range> r);

    static const int parameterCount = 2;

    float getParamLevel(int index);
    virtual void setParamLevel(int index, float level) = 0;

    protected:
    std::vector<Range> ranges = {Range(), Range()};
    std::vector<float> levels = {0, 0};
    std::string name;
};

#endif