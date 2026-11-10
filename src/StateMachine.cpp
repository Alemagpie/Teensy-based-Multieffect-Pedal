#include "StateMachine.h"

void StateMachine::handleInput() {
    switch(currentState) {
        case PLAY:
            //Left press
            if(input->switches[0] == PressType::SHORT) {
                chain->previousChainEffect();
                onEffectChange();
                //When switching effects, disable mod
                isModifying = false;
            }
            //Right press
            if(input->switches[2] == PressType::SHORT) {
                chain->nextChainEffect();
                onEffectChange();
                //When switching effects, disable mod
                isModifying = false;
            }

            //Center short press (toggle effect)
            if(input->switches[1] == PressType::SHORT) {
                chain->toggleChainEffect();
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position + 1, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
            }
            //Center long press (enter edit mode)
            if(input->switches[1] == PressType::LONG) {
                currentState = EDIT;
                ui->drawCandidateEffect(chain->availableEffects[chain->getSelectionIndex()]->getEffectName());
            }

            //Toggle mod with short rot enc pb press
            if(input->switches[5] == PressType::SHORT){
                currentState = PARAM_MODIFY;
            }

            //Display parameter abbreviatinos in groups of four
            if(input->encoderDelta != 0) {
                //display new set
                paramDisplayGroupIndex += input->encoderDelta;
                if(paramDisplayGroupIndex < 0) paramDisplayGroupIndex = 0;
                if(paramDisplayGroupIndex > maxParamDisplayIndex - 1) paramDisplayGroupIndex = maxParamDisplayIndex - 1;

                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position + 1, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    paramDisplayGroupIndex,
                    chain->isOn
                );
            }
        break;

        case EDIT:
            //Left press
            if(input->switches[0] == PressType::SHORT) {
                chain->previousAvailableEffect();
                uint8_t index = chain->getSelectionIndex();
                ui->drawCandidateEffect(chain->availableEffects[index]->getEffectName());
            }
            //Right press
            if(input->switches[2] == PressType::SHORT) {
                chain->nextAvailableEffect();
                uint8_t index = chain->getSelectionIndex();
                ui->drawCandidateEffect(chain->availableEffects[index]->getEffectName());
            }

            //Center short press (confirm effect selection)
            if(input->switches[1] == PressType::SHORT) {
                preset->saveEffects();
                preset->loadEffects();
                currentState = PLAY;
                onEffectChange();
            }
            //Center long press (exit edit mode)
            if(input->switches[1] == PressType::LONG) {
                currentState = PLAY;
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position + 1, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
            }
        break;

        case PARAM_MODIFY:
            //Once entered, rot enc scrolls through the effects, pb starts modifying the current parameter -> short press to go back to scrolling, long press exits modifying
            
            paramIndex = 4 * paramDisplayGroupIndex;

            //ui->drawParam(...)     //add another parameter (bool isModifying), to draw something to tell that it's actually modifying (ex. bar above level slider)

            //rot enc short press
            if(input->switches[5] == PressType::SHORT) {
                if(!isModifying) {
                    //paramLevel = (float)get parameter / 65536.0f 
                    //ui->drawParam(...)
                }

                isModifying = !isModifying; 
            }

            //rot enc long press to stop editing parameters
            if(input->switches[5] == PressType::LONG && !isModifying) {
                //No param is being directly modified
                paramIndex = -1;
                currentState = PLAY;
            }

            //Expression button pressed
            if(input->switches[4] == PressType::SHORT) {
                //use assignExprToParam function
            }

            //Allow param updates every 100ms
            if(input->encoderDelta != 0) {
                //Change parameter value
                if(isModifying) {
                    //Increase/decrease parameter value
                    unsigned long now = millis();
                    if(now - lastUpdate >= PARAM_UPDATE_TIMER_MS) {
                        lastUpdate = now;

                        //paramLevel += (input->encoderDelta > 0) ? paramDelta : -paramDelta;  
                        //if(paramLevel > 1.0f) paramLevel = 1.0f;
                        //if(paramLevel < 0.0f) paramLevel = 0.0f;

                        //set effect's parameter level to (uint16_t)paramLevel*65536.0f

                        /*for(int i = 0; i < 4; i++) {
                            uint8_t position = chain->getSlotPosition();
                            chain->effects[position] -> setParamLevel(i, input->encoderDelta);
                            ui->drawParam(chain->effects[position]->getParamNames()->at(i), chain->effects[position]->getParamLevel(i));
                        }*/
                    }
                } else {
                    //Go to next/previous paramter, within [0, maxParamIndex - 1]
                    paramIndex += input->encoderDelta;
                    if(paramIndex < 0) paramIndex = 0;
                    if(paramIndex > maxParamIndex - 1) paramIndex = maxParamIndex - 1;
                }
            } 
            
        break;
    }

}