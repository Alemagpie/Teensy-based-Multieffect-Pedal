#include <Arduino.h>
#include <dspinst.h>
#include <AudioStream.h> 

class QuantizerModule {
    public:
        QuantizerModule() { bitResolution = 14; intervalLenght = 1; }
        QuantizerModule(uint8_t b) { setResolution(b); }

        inline void setResolution(uint8_t r) { 
            bitResolution = r; 
            intervalLenght = 1 << (15 - bitResolution);
        }
        
        void process(int16_t &value);

    private:
        uint8_t bitResolution;
        uint16_t intervalLenght;
};