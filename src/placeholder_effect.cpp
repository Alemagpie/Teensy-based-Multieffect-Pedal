#include "placeholder_effect.h"

void PlaceholderEffect::update(void)
{
	audio_block_t *block = receiveWritable();
    if(!block) return;
    transmit(block);
    release(block);
}

void PlaceholderEffect::init(float p1, float p2, float p3, float p4) {}