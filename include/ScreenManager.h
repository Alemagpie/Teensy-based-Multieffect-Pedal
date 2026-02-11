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

int effectNumber_padding_x = 10;
int effectNumber_padding_y = 20;
int effectName_padding_x = 40;
int effectName_padding_y = 20;
int paramName_padding_x = 6;
int paramName_padding_y = 50;
int paramNameSize = 21;

class ScreenManager {
    public:
        void start() {
            Wire.begin();          // Uses SDA=18, SCL=19 on Teensy 4.1
            u8g2.begin();

            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_artosserif8_8r); //u8g2_font_ncenB08_tr
            u8g2.drawStr(10, 20, "Multieffect on");
            u8g2.drawStr(10, 40, "Loading...");
            u8g2.sendBuffer();
        }

        void setEffectNumber(int index) {
            const char c = ('0' + index);
            u8g2.drawStr(effectNumber_padding_x, effectNumber_padding_y, &c);
        }

        void setEffectName(const char *name) {
            u8g2.drawStr(effectName_padding_x, effectName_padding_y, name);
        }

        void setParamNames(std::vector<const char*> &names) {
            for(int i = 0; i < names.size(); i++) {
                u8g2.drawStr(paramName_padding_x * (i+1) + paramNameSize * i , paramName_padding_y, names[i]);
            }
        }

        void drawEffect(int index, const char *name, std::vector<const char*> &names){
            u8g2.clearBuffer();
            setEffectNumber(index);
            setEffectName(name);
            setParamNames(names);
            u8g2.sendBuffer();
        }
};

#endif