

#define numberOfIteration 6

#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVATE LOW
#define RESET HIGH


/************** Contactors ************/
   /*
    * MGset Contactor
    * N.B : At this piont make sure a signal is been sent to other nodes(clients) i.e
    * to disengage Gen and Utility contactors 
    */
    void engageMgSetContactorCoil() {
      digitalWrite(MG_SET_CONTACTOR_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("MgSetContactorCoil: Engaged"); // + (String)ret); 
      #endif
      activateMgSetContactorCoil.disable();
      client.publish("outTopic","MgSetContactorCoil: Closed");
    }
    void disengageMgSetContactorCoil() {
      digitalWrite(MG_SET_CONTACTOR_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("MgSetContactorCoil: Disengaged"); // + (String)ret); 
      #endif
      deactivateMgSetContactorCoil.disable();
    }

    void engageContactor335Coil() {
      digitalWrite(CONTACTOR_335_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("contactor335Coil: Engaged");
      #endif
      activateContactor335Coil.disable();
      client.publish("outTopic","contactor335Coil: Closed");
    }
    void disengageContactor335Coil() {
      digitalWrite(CONTACTOR_335_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("contactor335Coil: Disengaged");
      #endif
      deactivateContactor335Coil.disable();
    }
    
/************** Breakers ************/

    void closeStabBreakerToDc() {
      digitalWrite(STABILIZER_BREAKER_TO_DC_GRID_CLOSE,  ACTIVATE);
      vTaskDelay(2500 / portTICK_PERIOD_MS);
      digitalWrite(STABILIZER_BREAKER_TO_DC_GRID_CLOSE,  RESET);
      #if DEBUG
      Serial.println("stabBreakerToDc: ACTIVATED"); // + (String)ret); 
      #endif
      activateStabBreakerToDc.disable();
      client.publish("outTopic","stabBreakerToDc: Closed");
    }
    void openStabBreakerToDc() {
      digitalWrite(STABILIZER_BREAKER_TO_DC_GRID_OPEN,  ACTIVATE);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(STABILIZER_BREAKER_TO_DC_GRID_OPEN,  RESET);
      #if DEBUG
      Serial.println("stabBreakerToDc: RESETED"); 
      #endif
      deactivateStabBreakerToDc.disable();
    }


    void closeOutBreakerToDatacenter() { 
      digitalWrite(OUTPUT_BREAKER_TO_DATA_CENTER_CLOSE, ACTIVATE);
      vTaskDelay(2500 / portTICK_PERIOD_MS);
      digitalWrite(OUTPUT_BREAKER_TO_DATA_CENTER_CLOSE, RESET);
      #if DEBUG
      Serial.println("OutBreakerToDatacenter: ACTIVATED"); 
      #endif
      activateOutBreakerToDatacenter.disable();
      client.publish("outTopic","OutBreakerToDatacenter: Closed");
    }
    
    void openOutBreakerToDatacenter() {
      digitalWrite(OUTPUT_BREAKER_TO_DATA_CENTER_OPEN,  ACTIVATE);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(OUTPUT_BREAKER_TO_DATA_CENTER_OPEN,  RESET);
      #if DEBUG
      Serial.println("OutBreakerToDatacenter: RESETED"); 
      #endif
      deactivateOutBreakerToDatacenter.disable();
    }


    void resetAllLoadActuators(){
      int contactors[] = {MG_SET_CONTACTOR_COIL, CONTACTOR_335_COIL};
      for (int i=0; i < 2; i++){
        digitalWrite(contactors[i], DISENGAGE);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
      int breakers[] = {STABILIZER_BREAKER_TO_DC_GRID_OPEN, OUTPUT_BREAKER_TO_DATA_CENTER_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite(breakers[i], ACTIVATE);
      }
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      for (int i = 0; i < 2; i++){
        digitalWrite(breakers[i], RESET);
      }
      #if DEBUG
      Serial.println("All Load Actuators: Disengaged"); 
      #endif
      resetAllLoadActuator.disable();
    }
    
