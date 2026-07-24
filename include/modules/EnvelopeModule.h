#ifndef ENV_MODULE
#define ENV_MODULE

#include "Module.h"
#include "LowPassFilterModule.h"

class EnvelopeModule : Module {
    public:
    EnvelopeModule() { lp_m.setCutoff(signalCutoffFreq); }
    EnvelopeModule(uint16_t a, uint16_t r) {
        lp_m.setCutoff(signalCutoffFreq);
        setAttack(a);
        setRelease(r);
    }

    //If attack/release are out of bounds return a placeholder value
    void setAttack(float a) { 
        if(a < minAttack_ms) a = minAttack_ms;
        if(a > maxAttack_ms) a = maxAttack_ms;
        attack = timeToQ15(a);
    }
    void setRelease(float r) { 
        if(r < minRelease_ms) r = minRelease_ms;
        if(r > maxRelease_ms) r = maxRelease_ms;
        release = timeToQ15(r);
    }

    int16_t getEnvelope(int16_t &value) {
        int16_t sample = lp_m.process_not_in_place(value);
        int16_t pwr = signed_saturate_rshift(sample * sample, 16, 15);
        
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

    //uint16_t maxAttack = 15;    //50ms
    float maxAttack_ms = 50;
    //uint16_t minAttack = 3512;  //0.2ms
    float minAttack_ms = 0.2;
    uint16_t attack = 30;  //25ms
    //uint16_t maxRelease = 1;    //500ms
    float maxRelease_ms = 500;
    //uint16_t minRelease = 37;   //20ms
    float minRelease_ms = 20;
    uint16_t release = 3;  //260ms

    uint16_t threshold = 200;  //pure amplitude value in Q15

    LowPassFilterModule lp_m;
    float signalCutoffFreq = 1500;

    int16_t timeToQ15(float time_ms) {
        float tau = time_ms / 1000.0f;
        float coeff = (1.0f - expf(-1.0f / (AUDIO_SAMPLE_RATE_EXACT * tau))) * 32768.0f;
        if(coeff < 0.0f) return 0;
        if(coeff > 32767.0f) return 32767;
        return (int16_t) coeff;
    }
};

#endif