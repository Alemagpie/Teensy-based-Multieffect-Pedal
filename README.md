#DESCRIPTION:
A simple, microcontroller-based, multieffect pedal for guitar. The processing is done with a Teensy 4.1 microcontroller paired with an Audio Shield extension card.

#CURRENT PROGRESS:
-> Elemental DSP modules that can be combined to create all sorts of effects (gain, mixer, filters, lfo, drive, delay line, quantizer, sample and hold, etc.).

- Distortion effect with 4 parameters (gain, bias, treble, volume)
- Tremolo effect with 3 parameters (frequency, shape, depth)
- Bitcrusher effect with 4 parameters (resolution, down-sampling, mix, low pass)
- Vibrato effect with 3 parameters (speed, depth, tone)
- Chorus effect with 4 parameters (speed, depth, voices, mix)
- Delay with 4 parameters (time, feedback, dry, wet)
- Envelope filter (QTron style) with 4 parameters (sensibility, resonance, attack/release, filter type)
- Ring modulator with 4 parameters (frequency, shape, depth, mix)
- Square wave octaver with 4 parameters (dry, same octave, down octave, low-pass filter)

- Ability to chain multiple effects simultaneously
- Ability to change parameters' values in real time
- Ability to change effects in the chain
- simple UI

#TO-DO:
- Fix phaser
- Add makeup gain to all effects

#ROADMAP:
- Allpass filter module
- Replace pots with rotary encoder
- Add expression pedal input and parameter assign button
- Use one of the main switches as a momentary hold switch
- Add tap tempo 

#FUTURE EFFECTS:
- Ducking delay
- LFO lp
- Phaser
- Reverb
- Ambient drone (pads)
- ADSR envelope follower
- EQ
- Compressor
- Flanger
- Octaver
- Shimmer
- Wavefolder
- Granular