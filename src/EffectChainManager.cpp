#include "EffectChainManager.h"

void EffectChainManager::initChain() {
    AudioMemory(20);
    sgtl5000.enable();
    sgtl5000.volume(0.8);      
    sgtl5000.lineInLevel(8);
    sgtl5000.lineOutLevel(8); 
}

void EffectChainManager::updateChain(Save s) {
    s.effectIDs[currentSlot] = availableEffects[effectCandidateIndex]->getEffectID();
    isOn[currentSlot] = false;
}

void EffectChainManager::saveEffects() {
    availableEffects.push_back(effects[currentSlot]);
    effects[currentSlot] = availableEffects[effectCandidateIndex];
    availableEffects.erase(availableEffects.begin() + effectCandidateIndex);
    connectEffects();
    effectCandidateIndex = 0;
}