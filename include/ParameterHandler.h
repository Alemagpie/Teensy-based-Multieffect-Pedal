#ifndef PARAM_ADAPT_H
#define PARAM_ADAPT_H

#include <Range.h>

class ParameterAdapter {
    public:
    ParameterAdapter(std::vector<float>& p, std::vector<Range>& r);

    static const int parameterCount = 4;

    float getParamLevel(int index);
    void setParamLevel(int index, float level);

    private:
    //References to effect specific variables
    std::vector<float>& param;
    //Mapping range for each parameter
    std::vector<Range>& range;
};

#endif