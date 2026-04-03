#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

struct Save {
    uint8_t effectIDs[5];
};

class SaveManager {
    public:
    void storeSettings(Save& s) {
        EEPROM.put(address, s);
    }

    void loadSettings(Save& s) {
        EEPROM.get(address, s);
    }

    private:
    uint8_t address = 0;
};

#endif