

#define numberOfIteration 6

#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVE HIGH


#define ACTIVATE LOW
#define RESET HIGH



/************** Contactors ************/

    void CloseUtilityOrGenContactorRoutine() {
      digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE,  ENGAGE);
      #if DEBUG
      Serial.println("InputFromStabilizerOpenAndClose: Engaged"); // + (String)ret); 
      #endif
      client.publish("outTopic","InputFromStabilizerOpenAndClose: Engaged");
      closeUtilityOrGenContactor.disable();
    }
    void OpenUtilityOrGenContactorRoutine() {
      digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE,  DISENGAGE);
      #if DEBUG
      Serial.println("InputFromStabilizerOpenAndClose: Disengaged"); // + (String)ret); 
      #endif
      openUtilityOrGenContactor.disable();
    }

    
    void CloseBypassBreakerRoutine() {
      digitalWrite(BYPASS_BREAKER_CLOSE,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      digitalWrite(BYPASS_BREAKER_CLOSE,  RESET);
      #if DEBUG
      Serial.println("GenInputToStabilizer: Closed"); // + (String)ret); 
      #endif
      client.publish("outTopic","GenInputToStabilizer: Engaged");
      closeBypassBreaker.disable();
    }
    void OpenBypassBreakerRoutine() {
      digitalWrite(BYPASS_BREAKER_OPEN,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      digitalWrite(BYPASS_BREAKER_OPEN,  RESET);
      #if DEBUG
      Serial.println("GenInputToStabilizer: Closed"); // + (String)ret); 
      #endif
      openBypassBreaker.disable();
    }

    
    void hardResetAllLoadBreakers() {
        digitalWrite(BYPASS_BREAKER_OPEN,  ACTIVATE);
        digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE, DISENGAGE);
        vTaskDelay(1500/ portTICK_PERIOD_MS);
        //reset breaker
        digitalWrite(BYPASS_BREAKER_OPEN,  RESET);
        #if DEBUG
        Serial.println("All Stabilizer-Breakers: Resetted"); 
        #endif
        resetAllLoadBreakers.disable();
    }
