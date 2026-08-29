#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <vector>

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64
#endif

extern U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2;

extern unsigned char wireIcon[];
extern unsigned char wireIconL[];
extern unsigned char wireIconR[];
extern unsigned char onIcon[];
extern unsigned char offIcon[];
extern unsigned char leftArrowIcon[];
extern unsigned char rightArrowIcon[];

extern std::vector<int8_t> wirePositions;
extern std::vector<int8_t> externalWiresPositions;
extern std::vector<int8_t> squarePositions;
extern uint8_t spriteSize;

extern uint8_t effectNumber_padding_x;
extern uint8_t effectNumber_padding_y;
extern uint8_t effectName_padding_x;
extern uint8_t effectName_padding_y;
extern uint8_t chain_padding_y;
extern uint8_t paramName_padding_x;
extern uint8_t paramName_padding_y;
extern uint8_t paramNameSize;
extern uint8_t effectSelect_padding_x;
extern uint8_t effectSelect_padding_y;
extern uint8_t effectSelectArrows_padding_y;
extern const char* selectText;

class ScreenManager {
    public:
        void start();
        void setEffectNumber(uint8_t index);
        void setEffectName(const char *name);
        void setParamNames(std::vector<const char*> &names);
        void drawChainUI(std::vector<bool> &effectsState);
        void drawUI(uint8_t index, const char *name, std::vector<const char*> &names, std::vector<bool> &states);
        void drawSelection(const char *effectName);
};

#endif