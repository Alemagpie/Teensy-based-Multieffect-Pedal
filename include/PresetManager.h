#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include <Arduino.h>
#include <Audio.h>
#include "SaveManager.h"
#include "EffectAdapter.h"
#include "EffectChainManager.h"

class PresetManager {
    public:
    PresetManager(EffectChainManager& e) : chain(e) {};

    void loadEffects();
    void saveEffects();

    private:
    EffectChainManager& chain;
    SaveManager svMan;
    Save saveIDs;
};

#endif