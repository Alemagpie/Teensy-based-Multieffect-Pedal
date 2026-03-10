#include "modules/BiquadFilterModule.h"

void BiquadFilterModule::process(audio_block_t *block) {
    if(!block) return;

    int32_t b0, b1, b2, a1, a2, sum;
	uint32_t in2, out2, bprev, aprev, flag;
	uint32_t *data, *end;
	int32_t *state;

	end = (uint32_t *)(block->data) + AUDIO_BLOCK_SAMPLES/2;
	state = (int32_t *)definition;

	do {
		b0 = *state++;
		b1 = *state++;
		b2 = *state++;
		a1 = *state++;
		a2 = *state++;
		bprev = *state++;
		aprev = *state++;
		sum = *state & 0x3FFF;
		data = end - AUDIO_BLOCK_SAMPLES/2;
		do {
			in2 = *data;
			sum = signed_multiply_accumulate_32x16b(sum, b0, in2);
			sum = signed_multiply_accumulate_32x16t(sum, b1, bprev);
			sum = signed_multiply_accumulate_32x16b(sum, b2, bprev);
			sum = signed_multiply_accumulate_32x16t(sum, a1, aprev);
			sum = signed_multiply_accumulate_32x16b(sum, a2, aprev);
			out2 = signed_saturate_rshift(sum, 16, 14);
			sum &= 0x3FFF;
			sum = signed_multiply_accumulate_32x16t(sum, b0, in2);
			sum = signed_multiply_accumulate_32x16b(sum, b1, in2);
			sum = signed_multiply_accumulate_32x16t(sum, b2, bprev);
			sum = signed_multiply_accumulate_32x16b(sum, a1, out2);
			sum = signed_multiply_accumulate_32x16t(sum, a2, aprev);
			bprev = in2;
			aprev = pack_16b_16b(
				signed_saturate_rshift(sum, 16, 14), out2);
			// retaining part of the sum is meant to implement the
			// "first order noise shaping" described in this article:
			// http://www.earlevel.com/main/2003/02/28/biquads/
			//  TODO: is logical AND really correct, or maybe it
			//        should really be signed_saturate_rshift() ???
			sum &= 0x3FFF;
			bprev = in2;
			*data++ = aprev;
		} while (data < end);
        
		flag = *state & 0x80000000;
		*state++ = sum | flag;
		*(state-2) = aprev;
		*(state-3) = bprev;
	} while (flag);
}

void BiquadFilterModule::setCoefficients(uint32_t stage, const int *coefficients)
{
	if (stage >= 4) return;
	int32_t *dest = definition + (stage << 3);
	__disable_irq();
	if (stage > 0) *(dest - 1) |= 0x80000000;
	*dest++ = *coefficients++;
	*dest++ = *coefficients++;
	*dest++ = *coefficients++;
	*dest++ = *coefficients++ * -1;
	*dest++ = *coefficients++ * -1;
	//*dest++ = 0;
	//*dest++ = 0;  // clearing filter state causes loud pop
	dest += 2;
	*dest   &= 0x80000000;
	__enable_irq();
}