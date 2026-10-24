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
                    position + 1, 
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
                    position + 1, 
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

            //Toggle mod
            if(input->switches[5] == PressType::SHORT){
                currentState = PARAM_MODIFY;
            }

            //Display parameter abbreviatinos in groups of four
            if(input->encoderDelta != 0) {
                //display new set
                //when the effect changes: paramDisplayGroupIndex = 0;
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
                uint8_t position = chain->getSlotPosition();
                ui->drawNewEffect(
                    position + 1, 
                    chain->effects[position]->getEffectName(),
                    chain->effects[position]->getParamNames(),
                    chain->isOn
                );
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

            //ui->drawParam(...)     //add another parameter (bool isModifying), to draw something to tell that it's actually modifying (ex. bar above level slider)

            //rot enc short press
            if(input->switches[5] == PressType::SHORT) {
                if(!isModifying) {
                    //paramIndex = (paramIndex == -1) ? 0 : ...
                    //paramLevel = (float)get parameter / 65536.0f 
                }

                isModifying = !isModifying; 
            }

            //rot enc long press to stop editing parameters
            if(input->switches[5] == PressType::LONG && !isModifying) {
                //paramIndex = -1;
                currentState = PLAY;
            }

            //Expression button pressed
            if(input->switches[4] == PressType::SHORT) {
                //use assignExprToParam function

                //could this be moved just to use a long press when isModifying = true?
            }

            //Allow param updates every 100ms
            if(input->encoderDelta != 0) {
                //Change parameter value
                if(isModifying) {
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
                    //Go to next/previous paramter, within [0, parameterCount - 1]
                    //paramIndex = ...
                }
            } 
            
        break;
    }

}