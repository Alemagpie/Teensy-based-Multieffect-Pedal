#include "UIManager.h"

void UIManager::start() {
    scrMan.start();
}

void UIManager::drawNewEffect(uint8_t index, const char* name, std::vector<const char *> * params, std::vector<bool> status) {
    scrMan.drawUI(index, name, *params, status);
}

void UIManager::drawCandidateEffect(const char* name) {
    scrMan.drawSelection(name);
}