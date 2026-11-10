#include "UIManager.h"

void UIManager::start() {
    scrMan.start();
}

void UIManager::drawNewEffect(uint8_t index, const char* name, std::vector<const char *> * params, std::vector<bool> status) {
    scrMan.drawUI(index, name, params, status);
}

void UIManager::drawNewEffect(uint8_t index, const char* name, std::vector<const char *> * params, uint8_t paramSet, std::vector<bool> status) {
    scrMan.drawUI(index, name, params + paramSet, status);
}

void UIManager::drawParam(const char* fullName, uint8_t level, bool isMod) {
    scrMan.drawParam(fullName, level, isMod);
}

void UIManager::drawCandidateEffect(const char* name) {
    scrMan.drawSelection(name);
}