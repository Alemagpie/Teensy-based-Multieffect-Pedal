#ifndef EFFECT_CHAIN_MANAGER_H
#define EFFECT_CHAIN_MANAGER_H

#include <Audio.h>
#include "InputState.h"
#include "SaveManager.h"

#include "effects/distortion_effect.h"
#include "effects/tremolo_effect.h"
#include "effects/bitcrusher_effect.h"
#include "effects/vibrato_effect.h"
#include "effects/chorus_effect.h"
#include "effects/volume_effect.h"
#include "effects/delay_effect.h"
#include "effects/biquad_effect.h"
#include "effects/phaser_effect.h"
#include "effects/env_filter_effect.h"
#include "effects/squarer_effect.h"
#include "effects/ring_mod_effect.h"


//Effects
DistortionEffect dist;
TremoloEffect trem;
BitCrusherEffect bitcrush;
VibratoEffect vib;
ChorusEffect ch;
VolumeEffect vl;
DelayEffect de;
BiquadEffect bq;
PhaserEffect ph;
EnvelopeFilterEffect envf;
SquarerEffect sqr;
RingModulatorEffect rngMd;



class EffectChainManager {
    public:
    EffectChainManager(AudioInputI2S in, AudioOutputI2S out, AudioControlSGTL5000 ctrl) : input(in), output(out), sgtl5000(ctrl) {
        AudioMemory(20);
        sgtl5000.enable();
        sgtl5000.volume(0.8);      
        sgtl5000.lineInLevel(8);
        sgtl5000.lineOutLevel(8); 
    };

    void nextChainEffect();
    void previousChainEffect();
    void toggleChainEffect();

    void nextAvailableEffect();
    void previousAvailableEffect();
    void selectAvailableEffect();

    void setParameters(InputState& in);

    void initChain();
    void updateChain(Save s);
    void saveEffects();

    void connectEffects();

    EffectAdapter* effects[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::vector<bool> isOn = {false, false, false, false, false};
    std::vector<EffectAdapter*> allEffects = {
        &dist, &trem, &bitcrush, &vib, &ch, &vl, &de, &bq, &ph, &envf, &sqr, &rngMd
    };
    std::vector<EffectAdapter*> availableEffects = {
        &dist, &trem, &bitcrush, &vib, &ch, &vl, &de, &bq, &ph, &envf, &sqr, &rngMd
    };

    private:
    AudioInputI2S& input;
    AudioOutputI2S& output;
    AudioControlSGTL5000& sgtl5000;

    int effectCount = 5;
    uint8_t currentSlot = 0;
    uint8_t effectCandidateIndex = 0;

    //Connections
    AudioConnection* ie1 = nullptr;
    AudioConnection* e1e2 = nullptr;
    AudioConnection* e2e3 = nullptr;
    AudioConnection* e3e4 = nullptr;
    AudioConnection* e4e5 = nullptr;
    AudioConnection* e5o = nullptr;
};

#endif