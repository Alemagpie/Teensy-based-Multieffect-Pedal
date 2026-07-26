#include "effects/env_filter_effect.h"

void EnvelopeFilterEffect::update(void) {
    audio_block_t *block;
    int16_t sample, env;
    

    block = receiveWritable();

    if(!block) {
        return;
    }

    if(enabled) {
        inputSamplePtr = block->data; 

        for(int i = 0;i < AUDIO_BLOCK_SAMPLES;i++) {
            sample = *inputSamplePtr;
            env = env_m.getEnvelope(sample);
            float envNorm = sqrtf((float)env / 32767.0f);
            float exponent = (mode == 2) ? (1.0f - envNorm) * sens : envNorm * sens;

            //Get how much the filter is affected, if mode is 2 (lp down) make it the complement to 1
            freq_amount = powf(f_max/f_min, exponent);
            Serial.print(env);
            Serial.print(" - ");
            Serial.print(f_min * freq_amount);
            Serial.print("\n");
            bq_m.setCutoff(f_min * freq_amount);
            bq_m.process(sample);

            *inputSamplePtr = sample;
            inputSamplePtr++;
        }
    }
    
    transmit(block);
    release(block);
}

void EnvelopeFilterEffect::setParamLevel(int index, uint16_t level) {
    if(index < 0 || index > parameterCount - 1) {
        return;
    }

    //update parameters levels
    levels[index] = level;

    float valueLin = Utility::calculateParamValueLin(ranges[index], (float)level/65536.0f );
    float valueLog = Utility::calculateParamValueLog(ranges[index], (float)level/65536.0f);

    switch(index) {
        case 0:
            sens = valueLin;
        break;

        case 1:
            q = valueLog;
            bq_m.setQ(q);
        break;

        case 2:
            env_m.setAttack(atk_max - valueLin * (atk_max - atk_min));
            env_m.setRelease(rel_max - valueLin * (rel_max - rel_min));
        break;

        case 3:
            mode = (uint8_t) valueLin;
            bq_m.setMode( (mode == 0) ? 3 : 0 );
        break;
        
        default:
        break;
    }
}