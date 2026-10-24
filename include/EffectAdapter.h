#ifndef EFFECT_ADAPTER_H
#define EFFECT_ADAPTER_H

#include <string>
#include <vector>
#include "Utility.h"
#include "CustomRange.h"
#include <initializer_list>

/**
 * IMPORTANT:
 * 1) parameterCount, ranges, levels, paramName, paramFullName and isParamLog MUST always match size
 * 2) paramName MUST have a number of entries that is a multiple of 4. If there aren't enough parameters, the empty spots must be filled with "---"
 */

class EffectAdapter {
    public:
    EffectAdapter();
    EffectAdapter(std::initializer_list<CustomRange> r) : ranges(r) {};

    uint16_t getParamLevel(uint8_t index) const {return (index > parameterCount - 1) ? 0 : levels[index];}
    virtual void setParamLevel(int index, uint16_t level) = 0;

    /*
    //The parameter influenced by the tap tempo has to be set internally by the developer, not asssigned by the user
    virtual void setParamFromTapTempo(float tapTempo) {};
    //The parameter influenced by the expr pedal has to be asssigned by the user
    virtual void assignExprToParam(uint8_t paramIndex) {};
    //The parameter influenced by the hold press has to be set internally by the developer, not asssigned by the user
    virtual void setParamAsHold() {}; 
    */

    bool toggleEnable(){
        enabled = !enabled;
        return enabled;
    }
    //Needed to build the audio chain
    virtual AudioStream* getAudioStreamComponent() = 0;

    const char* getEffectName() { return effectName; }
    const uint8_t getEffectID() { return ID; }
    std::vector<const char*> *getParamNames() { return &paramName; }
    std::vector<const char*> *getParamFullName() { return &paramFullName; }
    uint8_t getParamCount() { return parameterCount; }

    virtual void assignExprToParam(uint8_t index);  //if not enabled, enable it. If already enabled, disable for old and enable for new
    virtual void disableExpParam();

    protected:
    uint8_t ID;
    std::vector<CustomRange> ranges;
    std::vector<uint8_t> levels = {0, 0, 0, 0};
    const char* effectName;
    std::vector<const char*> paramName;
    std::vector<const char*> paramFullName;
    std::vector<bool> isParamLog = {0, 0, 0, 0};
    bool enabled = false;
    uint8_t parameterCount = 4;
    int expParamIndex = -1;
};

#endif