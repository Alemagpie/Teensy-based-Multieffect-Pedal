#ifndef EFFECT_CHAIN_MANAGER_H
#define EFFECT_CHAIN_MANAGER_H

#include <Audio.h>
#include "InputState.h"
#include "SaveManager.h"

#include "Effects.h"

class EffectChainManager {
    public:
    //Sets up codec as well as chain's input and output
    void setup(AudioInputI2S* in, AudioOutputI2S* out, AudioControlSGTL5000* ctrl);
    //Advances in the current effect chain
    void nextChainEffect();
    //Goes back in the current effect chain
    void previousChainEffect();
    //Turns on/off effect at the current slot
    void toggleChainEffect();

    //Gets next possible selection
    void nextAvailableEffect();
    //Gets previous possible selection
    void previousAvailableEffect();
    //Confirms new effect selection
    void selectAvailableEffect();

    void setParameters(InputState& in);
    uint8_t getSlotPosition() { return currentSlot; }
    uint8_t getSelectionIndex() { return effectCandidateIndex; }

    //Updates the IDs in effect vector
    void updateChain(Save s);
    //Updates the actual available effects and reconnects effects
    void saveEffects();

    //Triggers effect reconnection
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
    AudioInputI2S* input = nullptr;
    AudioOutputI2S* output = nullptr;
    AudioControlSGTL5000* sgtl5000 = nullptr;

    const int effectCount = 5;
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