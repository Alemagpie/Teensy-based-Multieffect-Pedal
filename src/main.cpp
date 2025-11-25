#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <vector>
#include "EffectAdapter.h"

#include "distortion_effect.h"
#include "placeholder_effect.h"

AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;

DistortionEffect dist;
AudioMixer4 mixer;
AudioConnection im(input, 0, mixer, 0);
AudioConnection ie1(input, 0, dist, 0);
AudioConnection e1m(dist, 0, mixer, 1); //effects always on channel 1
AudioConnection mo(mixer, 0, output, 0);

void setup() {
    AudioMemory(12);
  sgtl5000.enable();
  sgtl5000.volume(0.8);      
  sgtl5000.lineInLevel(8);
  sgtl5000.lineOutLevel(8); 
  mixer.gain(0, 0.0f);
  mixer.gain(1, 1.0f);
}

void loop() {

}