#include "StateMachine.h"

void StateMachine::handleInput() {
    switch(currentState) {
        case PLAY:
            //Left press
            if(input->switches[0] == PressType::SHORT) {
                chain->previousChainEffect();
                //Redraw new effect
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
                //When switching effects, disable mod
                isModifying = false;
            }
            //Right press
            if(input->switches[2] == PressType::SHORT) {
                chain->nextChainEffect();
                //Redraw new effect
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
                //When switching effects, disable mod
                isModifying = false;
            }

            //Center short press (toggle effect)
            if(input->switches[1] == PressType::SHORT) {
                chain->toggleChainEffect();
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
            }
            //Center long press (enter edit mode)
            if(input->switches[1] == PressType::LONG) {
                currentState = EDIT;
                ui->drawCandidateEffect(chain->effects[chain->getSelectionIndex()]->getEffectName());
            }

            //Toggle mod
            if(input->switches[3] == PressType::SHORT){
                isModifying = !isModifying;
            }
        break;

        /*case MODIFY:
            break;*/

        case EDIT:
            //Left press
            if(input->switches[0] == PressType::SHORT) {
                chain->previousAvailableEffect();
                uint8_t index = chain->getSelectionIndex();
                ui->drawCandidateEffect(chain->effects[index]->getEffectName());
            }
            //Right press
            if(input->switches[2] == PressType::SHORT) {
                chain->nextAvailableEffect();
                uint8_t index = chain->getSelectionIndex();
                ui->drawCandidateEffect(chain->effects[index]->getEffectName());
            }

            //Center short press (confirm effect selection)
            if(input->switches[1] == PressType::SHORT) {
                preset->saveEffects();
            }
            //Center long press (exit edit mode)
            if(input->switches[1] == PressType::LONG) {
                currentState = PLAY;
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
            }
        break;
    }

    digitalWrite(EXTRA_LED_PIN, isModifying ? HIGH : LOW);

    //Allow param updates every 100ms
    if(isModifying) {
        unsigned long now = millis();
        if(now - lastUpdate >= PARAM_UPDATE_TIMER_MS) {
            lastUpdate = now;
            for(int i = 0; i < 4; i++) {
                uint8_t position = chain->getSlotPosition();
                chain->effects[position] -> setParamLevel(i, input->params[i]);
            }
        }
    }
}