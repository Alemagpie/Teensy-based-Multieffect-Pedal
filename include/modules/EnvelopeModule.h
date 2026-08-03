#ifndef ENV_MODULE
#define ENV_MODULE

#include "Module.h"
#include "LowPassFilterModule.h"

class EnvelopeModule : Module {
    public:
    EnvelopeModule() { }
    EnvelopeModule(uint16_t a, uint16_t r) {
        setAttack(a);
        setRelease(r);
        setGain(10);
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
    void setThreshold(uint16_t t) {
        if(t > maxThreshold) t = maxThreshold;
        threshold = t;
    }
    void setGain(uint8_t g) {
        if(g > maxGain) g = maxGain;
        gain = g;
    }

    int16_t getEnvelope(int16_t &value) {
        int16_t x = saturate16(gain * ((value < 0) ? -value : value));
        
        if(x > env) {
            env += signed_saturate_rshift(attack * (x - env), 16, 15);
        } else {
            env += signed_saturate_rshift(release * (x - env), 16, 15);
        }

        //if(env < threshold) { env = 0; }

        return env;
    }

    private:
    int16_t env = 0;

    uint8_t gain = 1;
    uint8_t maxGain = 30;

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
    uint16_t maxThreshold = 1000;

    int16_t timeToQ15(float time_ms) {
        float tau = time_ms / 1000.0f;
        float coeff = (1.0f - expf(-1.0f / (AUDIO_SAMPLE_RATE_EXACT * tau))) * 32768.0f;
        if(coeff < 0.0f) return 0;
        if(coeff > 32767.0f) return 32767;
        return (int16_t) coeff;
    }
};

#endif