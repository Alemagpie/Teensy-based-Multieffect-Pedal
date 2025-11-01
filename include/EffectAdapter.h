#ifndef EFFECT_ADAPT_H
#define EFFECT_ADAPT_H

#include <Range.h>
#include <string>

class EffectAdapter {
    public:
    EffectAdapter();
    EffectAdapter(std::vector<Range> r);

    static const int parameterCount = 4;

    //virtual float getParamLevel(int index) = 0;
    virtual void setParamLevel(int index, float level) = 0;

    protected:
    std::vector<Range> ranges = {Range(), Range(), Range(), Range()};
    std::vector<float> levels = {0, 0, 0, 0};
    std::string name;
};

#endif