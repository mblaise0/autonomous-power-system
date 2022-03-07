

#define numberOfIteration 6

#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVATE LOW
#define RESET HIGH


#define DEBUG 1

  
/************** Contactors ************/

    void enableStabilizerContactorOpenAndCloseRoutine() {
      digitalWrite(STABILIZER_CONTACTOR_OPEN_CLOSE,  ENGAGE);
      #if DEBUG
      Serial.println("StabilizerContactorOpenAndClose: Engaged"); // + (String)ret); 
      #endif
      client.publish("outTopic","StabilizerContactorOpenAndClose: Closed");
      enableStabilizerContactorOpenAndClose.disable();
    }
    void disableStabilizerContactorOpenAndCloseRoutine() {
      digitalWrite(STABILIZER_CONTACTOR_OPEN_CLOSE,  DISENGAGE);
      #if DEBUG
      Serial.println("StabilizerContactorOpenAndClose: Disengaged"); // + (String)ret); 
      #endif
      disableStabilizerContactorOpenAndClose.disable();
    }

    
    void openGenInputToStabilizerRoutine() {
      digitalWrite(GEN_INPUT_TO_STABILIZER_OPEN,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(GEN_INPUT_TO_STABILIZER_OPEN,  RESET);
      #if DEBUG
      Serial.println("GenInputToStabilizer: Opened"); // + (String)ret); 
      #endif
      openGenInputToStabilizer.disable();
    }
    void closeGenInputToStabilizerRoutine() {
      
      digitalWrite(GEN_INPUT_TO_STABILIZER_CLOSE,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(GEN_INPUT_TO_STABILIZER_CLOSE,  RESET);
      #if DEBUG
      Serial.println("GenInputToStabilizer: Closed"); // + (String)ret); 
      #endif
      client.publish("outTopic","GenInputToStabilizer: Closed");
      closeGenInputToStabilizer.disable();
    }

    void closeUtilityToStabilizerBreakerRoutine() {
      digitalWrite(UTILITY_TO_STABILIZER_BREAKER_CLOSE,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(UTILITY_TO_STABILIZER_BREAKER_CLOSE,  RESET);
      #if DEBUG
      Serial.println("UtilityToStabilizerBreaker: Closed");
      #endif
      client.publish("outTopic","UtilityToStabilizerBreaker: Closed");
      closeUtilityToStabilizerBreaker.disable();
    }
    void openUtilityToStabilizerBreakerRoutine() {
      digitalWrite(UTILITY_TO_STABILIZER_BREAKER_OPEN,  ACTIVATE);
      //reset trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(UTILITY_TO_STABILIZER_BREAKER_OPEN,  RESET);
      #if DEBUG
      Serial.println("UtilityToStabilizerBreaker: Opened");
      #endif
      openUtilityToStabilizerBreaker.disable();
    }
    

    void hardResetAllLoadBreakersRoutine() {
      int loadBreakers[] = {GEN_INPUT_TO_STABILIZER_OPEN, UTILITY_TO_STABILIZER_BREAKER_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite(loadBreakers[i],  ACTIVATE);
        vTaskDelay(800/ portTICK_PERIOD_MS);
      }
        vTaskDelay(500/ portTICK_PERIOD_MS);

      int resetLoadBreakers[] = {GEN_INPUT_TO_STABILIZER_OPEN, UTILITY_TO_STABILIZER_BREAKER_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite(resetLoadBreakers[i],  RESET);
        vTaskDelay(50/ portTICK_PERIOD_MS);
      }
      digitalWrite(STABILIZER_CONTACTOR_OPEN_CLOSE, DISENGAGE);
      #if DEBUG
      Serial.println("All Stabilizer-Breakers: Resetted"); 
      #endif
      hardResetAllLoadBreakers.disable();
    }
