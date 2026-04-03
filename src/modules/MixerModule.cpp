#include "modules/MixerModule.h"

int16_t MixerModule::process(int16_t &ch1, int16_t &ch2, int16_t &ch3, int16_t &ch4) {
    int32_t sum =
        signed_saturate_rshift(ch1 * channelGain[0], 16, 8) +
        signed_saturate_rshift(ch2 * channelGain[1], 16, 8) +
        signed_saturate_rshift(ch3 * channelGain[2], 16, 8) +
        signed_saturate_rshift(ch4 * channelGain[3], 16, 8);
    
    return saturate16(sum >> 2);
} 

int16_t MixerModule::process(int16_t &ch1, int16_t &ch2) {
    int32_t sum =
        signed_saturate_rshift(ch1 * channelGain[0], 16, 8) +
        signed_saturate_rshift(ch2 * channelGain[1], 16, 8);
    
    return saturate16(sum >> 1);
} 