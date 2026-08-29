#include "EffectChainManager.h"

void EffectChainManager::setup(AudioInputI2S* in, AudioOutputI2S* out, AudioControlSGTL5000* ctrl) { 
    input = in; 
    output = out; 
    sgtl5000 = ctrl;
    
    AudioMemory(20);
    sgtl5000->enable();
    sgtl5000->volume(0.8);      
    sgtl5000->lineInLevel(8);
    sgtl5000->lineOutLevel(8); 
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

void EffectChainManager::connectEffects() {
    __disable_irq();
    delete(ie1);
    ie1 = new AudioConnection(*input, 0, *(effects[0]->getAudioStreamComponent()), 0);
    delete(e1e2);
    e1e2 = new AudioConnection(*(effects[0]->getAudioStreamComponent()), 0, *(effects[1]->getAudioStreamComponent()), 0);
    delete(e2e3);
    e2e3 = new AudioConnection(*(effects[1]->getAudioStreamComponent()), 0, *(effects[2]->getAudioStreamComponent()), 0);
    delete(e3e4);
    e3e4 = new AudioConnection(*(effects[2]->getAudioStreamComponent()), 0, *(effects[3]->getAudioStreamComponent()), 0);
    delete(e4e5);
    e4e5 = new AudioConnection(*(effects[3]->getAudioStreamComponent()), 0, *(effects[4]->getAudioStreamComponent()), 0);
    delete(e5o);
    e5o = new AudioConnection(*(effects[4]->getAudioStreamComponent()), 0, *output, 0);
    __enable_irq();
}

void EffectChainManager::toggleChainEffect() {
    isOn[currentSlot] = effects[currentSlot]->toggleEnable();
}

void EffectChainManager::nextChainEffect() {
    currentSlot++;
    if(currentSlot > effectCount - 1) currentSlot = 0;
}

void EffectChainManager::previousChainEffect() {
    currentSlot--;
    if(currentSlot < 0) currentSlot = effectCount - 1;
}

void EffectChainManager::nextAvailableEffect() {
    effectCandidateIndex++;
    if(effectCandidateIndex == availableEffects.size()) effectCandidateIndex = 0;
}

void EffectChainManager::previousAvailableEffect() {
    effectCandidateIndex--;
    if(effectCandidateIndex < 0) effectCandidateIndex = availableEffects.size() - 1;
}