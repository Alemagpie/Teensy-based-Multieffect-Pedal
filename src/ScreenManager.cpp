#include "ScreenManager.h"

U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(
    U8G2_R0,
    U8X8_PIN_NONE
);

PROGMEM unsigned char wireIcon[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM unsigned char wireIconL[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM unsigned char wireIconR[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM unsigned char onIcon[] = {0x00, 0x00, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0x00, 0x00};
PROGMEM unsigned char offIcon[] = {0x00, 0x00, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x03, 0xc0, 0x03, 0xc0, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00};
PROGMEM unsigned char leftArrowIcon[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x06, 0x00, 0x03, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0x80, 0x01, 0x00, 0x03, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
PROGMEM unsigned char rightArrowIcon[] = {0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x60, 0x00, 0xc0, 0x00, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x80, 0x01, 0xc0, 0x00, 0x60, 0x00, 0x30, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00};

std::vector<int8_t> wirePositions = {20, 44, 68, 92};
std::vector<int8_t> externalWiresPositions = {0, 112};
std::vector<int8_t> squarePositions = {8, 32, 56, 80, 104};
uint8_t spriteSize = 16;

uint8_t effectNumber_padding_x = 10;
uint8_t effectNumber_padding_y = 10;
uint8_t effectName_padding_x = 40;
uint8_t effectName_padding_y = 10;
uint8_t chain_padding_y = 20;
uint8_t paramName_padding_x = 8;
uint8_t paramName_padding_y = 60;
uint8_t paramNameSize = 21;
uint8_t effectSelect_padding_x = 13;
uint8_t effectSelect_padding_y = 16;
uint8_t effectSelectArrows_padding_y = 32;
const char* selectText = "Change effect to:";

void ScreenManager::start() {
    Wire.begin();
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont12_mr);
    u8g2.setBitmapMode(1);
    u8g2.drawStr(10, 20, "Multieffect on");
    u8g2.drawStr(10, 40, "Loading...");
    u8g2.sendBuffer();
}

void ScreenManager::setEffectNumber(uint8_t index) {
    char c[2];
    c[0] = '0' + index;
    c[1] = '\0';
    u8g2.setFont(u8g2_font_profont15_mr);
    u8g2.drawStr(effectNumber_padding_x, effectNumber_padding_y, c);
    u8g2.setFont(u8g2_font_profont12_mr);
}

void ScreenManager::setEffectName(const char *name) {
    u8g2.drawStr(effectName_padding_x, effectName_padding_y, name);
}

void ScreenManager::setParamNames(std::vector<const char*> &names) {
    for (uint8_t i = 0; i < names.size(); i++) {
        u8g2.drawStr(paramName_padding_x * (i + 1) + paramNameSize * i, paramName_padding_y, names[i]);
    }
}

void ScreenManager::drawChainUI(std::vector<bool> &effectsState) {
    for (uint8_t i = 0; i < squarePositions.size(); i++) {
        const uint8_t *icon = effectsState[i] ? onIcon : offIcon;
        u8g2.drawXBM(squarePositions[i], chain_padding_y, spriteSize, spriteSize, icon);
    }
    u8g2.drawXBM(externalWiresPositions[0], chain_padding_y, spriteSize, spriteSize, wireIconL);
    u8g2.drawXBM(externalWiresPositions[1], chain_padding_y, spriteSize, spriteSize, wireIconR);
    for (uint8_t i = 0; i < wirePositions.size(); i++) {
        u8g2.drawXBM(wirePositions[i], chain_padding_y, spriteSize, spriteSize, wireIcon);
    }
}

void ScreenManager::drawUI(uint8_t index, const char *name, std::vector<const char*> &names, std::vector<bool> &states) {
    u8g2.clearBuffer();
    setEffectNumber(index);
    setEffectName(name);
    drawChainUI(states);
    setParamNames(names);
    u8g2.sendBuffer();
}

void ScreenManager::drawSelection(const char *effectName) {
    u8g2.clearBuffer();
    u8g2.drawStr(effectSelect_padding_x, effectSelect_padding_y, selectText);
    u8g2.drawBox(10, 20, 108, 2);
    u8g2.drawXBM(0, effectSelectArrows_padding_y, spriteSize, spriteSize, leftArrowIcon);
    u8g2.drawXBM(112, effectSelectArrows_padding_y, spriteSize, spriteSize, rightArrowIcon);
    u8g2.drawStr(2 * effectSelect_padding_x, 12 + effectSelectArrows_padding_y, effectName);
    u8g2.sendBuffer();
}