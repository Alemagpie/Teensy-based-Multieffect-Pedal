#include "EffectAdapter.h"

EffectAdapter::EffectAdapter(std::initializer_list<Range> r) : ranges(r) {}

float EffectAdapter::getParamLevel(int index) {
    if(index < 0 || index > parameterCount - 1) {
        return 0;
    }

    return levels[index];
}