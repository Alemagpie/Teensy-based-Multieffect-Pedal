#ifndef ADSR_MODULE
#define ADSR_MODULE

#include "Module.h"
#include "EnvelopeModule.h"

class ADSRModule : Module {
    public:
    ADSRModule() { env_m.setAttack(295); env_m.setRelease(6); }

    int16_t getEnvelope(int16_t &value);

    void setAttack(uint16_t a) { attack = a; }
    void setDecay(uint16_t d) { decay = d; }
    void setSustain(uint16_t s) { sustain = s; sustainThreshold = s + sustainThresholdOffset;}
    void setRelease(uint16_t r) { release = r; }

    private: 

    void noteOn() { currentState = ATTACK; }
    void noteOff() { currentState = RELEASE; }

    enum State {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    State currentState = IDLE;

    int16_t env = 0;

    uint16_t attack;    //2-20ms
    uint16_t maxAttack = 37;
    uint16_t minAttack = 370;
    uint16_t decay; //50-200ms
    uint16_t maxDecay = 4;
    uint16_t minDecay = 15;
    uint16_t sustain;   //0.5-0.8
    uint16_t maxSustain = 26214;    //~0.8
    uint16_t minSustain = 16383;    //~0.5
    uint16_t release;   //200-800ms
    uint16_t maxRelease = 1;
    uint16_t minRelease = 15;

    int16_t decayThreshold = 32110; //about 0.98
    int16_t sustainThreshold;
    uint16_t sustainThresholdOffset = 812;  //about 0.025
    int16_t idleThreshold = 656;    //about 0.02

    int16_t triggerThreshold = 300; //if notes dont trigger lower the trigger threshold by 50
    int16_t releaseThreshold = 120; //if notes get cut too early lower the release threshold by 20
                                    //if envelope triggers on noise increase both thresholds by 50 and 20

    EnvelopeModule env_m;
    int16_t ctrlEnv;

    int16_t timeToQ15(float time_ms) {
        float tau = time_ms / 1000.0f;
        float coeff = 1.0f - expf(-1.0f / (AUDIO_SAMPLE_RATE_EXACT * tau));
        return (int16_t)(coeff * 32768.0f);
    }
};

#endif