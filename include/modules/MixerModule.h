#ifndef MIXER_MODULE
#define MIXER_MODULE

#include "Module.h"

class MixerModule : Module {
    public:
    int16_t process(int16_t &ch1, int16_t &ch2, int16_t &ch3, int16_t &ch4);

    int16_t process(int16_t &ch1, int16_t &ch2);

    void setGain(uint8_t channel, uint8_t gain) {
        if(channel < 3) {
            channelGain[channel] = gain;
        }
    }

    inline void reset() override { channelGain = {0, 0, 0, 0}; }

    private:
    std::vector<uint8_t> channelGain = {0, 0, 0, 0};
};

#endif