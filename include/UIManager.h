#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ScreenManager.h"

class UIManager {
    public:
    void start();
    //Draws new effect in the chain
    void drawNewEffect(uint8_t index, const char* name, std::vector<const char *> * params, std::vector<bool> status);
    //Draws selection for new effect to insert in the chain
    void drawCandidateEffect(const char* name);

    private:
    ScreenManager scrMan;
};

#endif