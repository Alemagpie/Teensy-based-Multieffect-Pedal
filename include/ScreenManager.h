#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(
    U8G2_R0,        // No rotation
    U8X8_PIN_NONE   // No reset pin
);

PROGMEM static unsigned char wireIcon[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM static unsigned char wireIconL[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM static unsigned char wireIconR[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
PROGMEM static unsigned char onIcon[] = {0x00, 0x00, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0x00, 0x00};
PROGMEM static unsigned char offIcon[] = {0x00, 0x00, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x03, 0xc0, 0x03, 0xc0, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00};

static std::vector<int8_t> wirePositions = {20, 44, 68, 92};
static std::vector<int8_t> externalWiresPositions = {0, 112};
static std::vector<int8_t> squarePositions = {8, 32, 56, 80, 104};
uint8_t spriteSize = 16;

uint8_t effectNumber_padding_x = 10;
uint8_t effectNumber_padding_y = 10;
uint8_t effectName_padding_x = 40;
uint8_t effectName_padding_y = 10;
uint8_t chain_padding_y = 20;
uint8_t paramName_padding_x = 8;
uint8_t paramName_padding_y = 60;
uint8_t paramNameSize = 21;

class ScreenManager {
    public:
        void start() {
            Wire.begin();          // Uses SDA=18, SCL=19 on Teensy 4.1
            u8g2.begin();

            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_profont12_mr); //u8g2_font_ncenB08_tr
            u8g2.setBitmapMode(1);
            u8g2.drawStr(10, 20, "Multieffect on");
            u8g2.drawStr(10, 40, "Loading...");
            u8g2.sendBuffer();
        }

        void setEffectNumber(uint8_t index) {
            char c[2];
            c[0] = '0' + index;
            c[1] = '\0';  //a null-terminated string is needed
            u8g2.setFont(u8g2_font_profont15_mr);
            u8g2.drawStr(effectNumber_padding_x, effectNumber_padding_y, c);
            u8g2.setFont(u8g2_font_profont12_mr);
        }

        void setEffectName(const char *name) {
            u8g2.drawStr(effectName_padding_x, effectName_padding_y, name);
        }

        void setParamNames(std::vector<const char*> &names) {
            for(uint8_t i = 0; i < names.size(); i++) {
                u8g2.drawStr(paramName_padding_x * (i+1) + paramNameSize * i , paramName_padding_y, names[i]);
            }
        }

        void drawChainUI(std::vector<bool> &effectsState) {  
            //draw boxes
            for(uint8_t i = 0; i <  squarePositions.size(); i++) {
                const uint8_t *icon = effectsState[i] ? onIcon : offIcon;
                u8g2.drawXBM(squarePositions[i], chain_padding_y, spriteSize, spriteSize, icon);
            }

            //draw external wires
            u8g2.drawXBM(externalWiresPositions[0], chain_padding_y, spriteSize, spriteSize, wireIconL);
            u8g2.drawXBM(externalWiresPositions[1], chain_padding_y, spriteSize, spriteSize, wireIconR);

            //draw interal wires
            for(uint8_t i = 0; i < wirePositions.size(); i++) {
                u8g2.drawXBM(wirePositions[i], chain_padding_y, spriteSize, spriteSize, wireIcon);
            }
        }

        void drawUI (uint8_t index, const char *name, std::vector<const char*> &names, std::vector<bool> &states){
            u8g2.clearBuffer();
            setEffectNumber(index);
            setEffectName(name);
            drawChainUI(states);
            setParamNames(names);
            u8g2.sendBuffer();
        }
};

#endif