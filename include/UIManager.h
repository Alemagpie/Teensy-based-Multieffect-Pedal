#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ScreenManager.h"

class UIManager {
    public:
    //Calls for screen manager setup
    void start();
    //Draws new effect in the chain
    void drawNewEffect(uint8_t index, const char* name, std::vector<const char *> * params, std::vector<bool> status);
    //Draws parameter and slider
    void drawParam(const char* fullName, uint8_t level);
    //Draws selection for new effect to insert in the chain
    void drawCandidateEffect(const char* name);

    private:
    ScreenManager scrMan;
};

#endif