#include "Module.h"

class QuantizerModule : Module {
    public:
        QuantizerModule() { setResolution(14); }
        QuantizerModule(uint8_t b) { setResolution(b); }

        void process(int16_t &value) override;

        inline void setResolution(uint8_t r) { 
            bitResolution = r; 
            intervalLenght = 1 << (15 - bitResolution);
        }

        inline void reset() override { setResolution(14); }

    private:
        uint8_t bitResolution;
        uint16_t intervalLenght;
};