#ifndef ENV_MODULE
#define ENV_MODULE

#include "Module.h"
#include "LowPassFilterModule.h"

class EnvelopeModule : Module {
    public:
    EnvelopeModule() { lp_m.setCutoff(signalCutoffFreq); }
    EnvelopeModule(uint16_t a, uint16_t r) {
        lp_m.setCutoff(signalCutoffFreq);
        attack = a;
        release = r;
    }

    void setAttack(uint16_t a) { attack = a; }
    void setRelease(uint16_t r) { release = r; }

    int16_t getEnvelope(int16_t &value) {
        lp_m.process(value);
        int16_t pwr = signed_saturate_rshift(value * value, 16, 15);
        
        if(pwr > env) {
            env += signed_saturate_rshift(attack * (pwr - env), 16, 15);
        } else {
            env += signed_saturate_rshift(release * (pwr - env), 16, 15);
        }

        if(env < threshold) { env = 0; }

        return env;
    }

    private:
    int16_t env = 0;

    uint16_t attack = 250; 
    uint16_t release = 10;    
    uint16_t threshold = 200;

    LowPassFilterModule lp_m;
    float signalCutoffFreq = 1500;
};

#endif