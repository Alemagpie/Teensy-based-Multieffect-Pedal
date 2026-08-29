#ifndef EFFECTS_H
#define EFFECTS_H

#include "effects/distortion_effect.h"
#include "effects/tremolo_effect.h"
#include "effects/bitcrusher_effect.h"
#include "effects/vibrato_effect.h"
#include "effects/chorus_effect.h"
#include "effects/volume_effect.h"
#include "effects/delay_effect.h"
#include "effects/biquad_effect.h"
#include "effects/phaser_effect.h"
#include "effects/env_filter_effect.h"
#include "effects/squarer_effect.h"
#include "effects/ring_mod_effect.h"

extern DistortionEffect dist;
extern TremoloEffect trem;
extern BitCrusherEffect bitcrush;
extern VibratoEffect vib;
extern ChorusEffect ch;
extern VolumeEffect vl;
extern DelayEffect de;
extern BiquadEffect bq;
extern PhaserEffect ph;
extern EnvelopeFilterEffect envf;
extern SquarerEffect sqr;
extern RingModulatorEffect rngMd;

#endif