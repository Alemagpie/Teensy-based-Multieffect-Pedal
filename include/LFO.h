#ifndef LFO_H
#define LFO_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include <dspinst.h>

class LFO {
    public:
    enum shape {
		SINE    = 0,
		SQUARE   = 1,
		TRIANG   = 2,
		SAW = 3,
        RAMP = 4
	};

    audio_block_t* sendReadOnly();
    void setFrequency(float f);
    void setAmplitude(float a);
    void setPhase(float angle);
    void setShape(short s);

    private:
    audio_block_t block;

    uint16_t amplitude;
    short shapeSelect = 0;
    uint32_t phase;
    uint32_t phaseStep;
    int16_t ph; //ph = phase >> 17;
    
    int16_t sine();
    int16_t square();
    int16_t triang();
    int16_t saw();
    int16_t ramp();
};

#endif