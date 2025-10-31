#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;
AudioConnection patchCord(input, 0, output, 0);

/*
AudioSynthWaveformSine sine1;
AudioConnection patchCord1(sine1, 0, output, 0);
AudioConnection patchCord2(sine1, 0, output, 1);
*/
AudioEffectChorus chorus();

//---------------------------------------------------------------------------------------------
//Holds pointers to the 5 current effects
std::vector<AudioStream*> effects(5);

//to refactor
//1) make a pure virtual class in AudioStream (virtual type function(...) = 0) 
//called setParameter(int index, float level) that each effect implements differently       --> started in chorus
//2) create a vector of 4 floats to keep track of the parameters' values
//3) create a vector of 4 ranges internally set to each effect's parameter's needs

void setup() {
  AudioMemory(12);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8);  
  /*sine1.frequency(440);
  sine1.amplitude(0.8);*/
}

void loop() {}