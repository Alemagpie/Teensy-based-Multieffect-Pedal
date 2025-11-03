#include "Adapter_EffectHandler.h"

Adapter_EffectHandler::Adapter_EffectHandler(std::initializer_list<CustomRange> r) : ranges(r) {}

float Adapter_EffectHandler::getParamLevel(int index) {
    if(index < 0 || index > parameterCount - 1) {
        return 0;
    }

    return levels[index];
}