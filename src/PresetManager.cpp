#include "PresetManager.h"

void PresetManager::loadEffects() {
    __disable_irq();
    svMan.loadSettings(saveIDs);

    //Assign effects from saved IDs
    for(int i = 0; i < 5; i++) {
        uint8_t id = saveIDs.effectIDs[i];
        for(int j = 0; j < chain.allEffects.size(); j++) {
        if(chain.allEffects[j]->getEffectID() == id) {
            chain.effects[i] = chain.allEffects[j];
            break;
        }
        }
    }

    for(int j = 0; j < chain.availableEffects.size(); j++)
    Serial.println(chain.availableEffects[j]->getEffectName());

    //Leaves only available effects 
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < chain.availableEffects.size(); j++) {
        if(chain.availableEffects[j] == chain.effects[i]) {
            chain.availableEffects.erase(chain.availableEffects.begin() + j);
            break;
        }
        }
    }

    //for(int j = 0; j < availableEffects.size(); j++)
    //Serial.println(availableEffects[j]->getEffectName());

    __enable_irq();
}

void PresetManager::saveEffects() {
    chain.updateChain(saveIDs);
    svMan.storeSettings(saveIDs);
    chain.saveEffects();
}