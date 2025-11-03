#include "EffectHandler.h"

EffectHandler::EffectHandler(std::initializer_list<CustomRange> r) : ranges(r) {}

float EffectHandler::getParamLevel(int index) {
    if(index < 0 || index > parameterCount - 1) {
        return 0;
    }

    return levels[index];
}