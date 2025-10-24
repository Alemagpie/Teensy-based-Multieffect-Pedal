#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

AudioInputI2S input;
AudioOutputI2S output;
AudioControlSGTL5000 sgtl5000;

AudioMixer4 mixer;
AudioConnection patchcord1(input, 0, mixer, 0);
AudioConneciton patchcord2(mixer, 0, output, 0);

  void setup() {
    _init();
  }

  void loop() {

  }



  private void _init() {
    AudioMemory(20);
    sgtl5000_1.enable();
    sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);  // use line in
    sgtl5000_1.volume(1);                      // master volume
    sgtl5000_1.lineInLevel(5);                   // increase sensitivity (0–15)
    sgtl5000_1.lineOutLevel(13);     

    mixer.gain(0, 1.0);
  }