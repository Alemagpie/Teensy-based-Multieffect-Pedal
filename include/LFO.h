#ifndef LFO_H
#define LFO_H

#include <Arduino.h>
#include <AudioStream.h> 
#include "CustomRange.h"
#include <dspinst.h>
#include "Utility.h"

class LFO {
    public:
    //LFO(float f, float a, short s) { setFrequency(f); setAmplitude(a); setShape(s); }

    enum shape {
		SINE    = 0,
		SQUARE   = 1,
		TRIANG   = 2,
		SAW = 3,
        RAMP = 4
	};

    enum mode {
        BIPOLAR = 0,
        UNIPOLAR = 1
    };

    audio_block_t* getReadOnly();
    void setFrequency(float f);
    void setAmplitude(float a);
    void setPhase(float angle);
    void setShape(short s);
    void setMode(short m);
    void setDepth(int16_t d);

    private:
    audio_block_t block;

    uint16_t amplitude;
    short shapeSelect = 0;
    short modeSelect = 0;
    int16_t depth = 32767;
    uint32_t phase;
    uint32_t phaseStep;
    uint16_t ph; //ph = phase >> 17;
    
    int16_t sine();
    int16_t square();
    int16_t triang();
    int16_t saw();
    int16_t ramp();
};

#endif