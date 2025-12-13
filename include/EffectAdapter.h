#ifndef EFFECT_ADAPTER_H
#define EFFECT_ADAPTER_H

#include <string>
#include <vector>
#include "Utility.h"
#include "CustomRange.h"
#include <initializer_list>

class EffectAdapter {
    public:
    EffectAdapter();
    EffectAdapter(std::initializer_list<CustomRange> r) : ranges(r) {};

    float getParamLevel(int index) const {return (index < 0 || index > parameterCount - 1) ? 0 : levels[index];}
    virtual void setParamLevel(int index, uint16_t level) = 0;

    bool toggleEnable(){
        enabled = !enabled;
        return enabled;
    }
    virtual AudioStream* getAudioStreamComponent() = 0;

    protected:
    std::vector<CustomRange> ranges;
    std::vector<uint16_t> levels = {0, 0, 0, 0};
    std::string name;
    bool enabled = false;

    static const int parameterCount = 4;
};

#endif