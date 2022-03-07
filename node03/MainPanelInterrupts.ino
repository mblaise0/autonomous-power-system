
#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVE HIGH
#define CLIENT_ID "node03"

#define ACTIVATE LOW
#define RESET HIGH

   int powerAvailable335IncomerState = 0;
   int previousPowerAvailable335IncomerState = 0;
   
   int mgSetContactorAuxState = 0;
   int previousMgSetContactorAuxState = 0;
   
   int contator335CoilAuxState = 0;
   int previousContator335CoilAuxState = 0;
   
   int powerAvailableDCGridState = 0;
   int previousPowerAvailableDCGridState = 0;
   
   int mgSetPowerAvailableState = 0;
   int previousMgSetPowerAvailableState = 0;
   
   int powerAvailableDCOutputState = 0;
   int previousPowerAvailableDCOutputState = 0;
   
   int dcOutputBreakerAuxState = 0;
   int previousDcOutputBreakerAuxState = 0;
   
   int stabilizerBreakerToDCAuxState = 0;
   int previousStabilizerBreakerToDCAuxState = 0;
   
   int powerAvailableToStabState = 0;
   int previousPowerAvailableToStabState = 0;

   int powerAvailableFromStabState = 0;
   int previousPowerAvailableFromStabState = 0;

   int outputBreakerDataCenterAuxState = 0;
   int previousOutputBreakerDataCenterAuxState = 0;
   
    
  void powerAvailable335InRoutine(){ 
   char data2[750];
    powerAvailable335IncomerState = digitalRead(POWER_AVAILABLE_335_INCOMER);
    if (powerAvailable335IncomerState != previousPowerAvailable335IncomerState) {
          if (powerAvailable335IncomerState == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("pinNumber"+ (String)POWER_AVAILABLE_335_INCOMER+ "state"+ (String)powerAvailable335IncomerState);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("pinNumber"+ (String)POWER_AVAILABLE_335_INCOMER+ "state"+ (String)powerAvailable335IncomerState);
               #endif
          }
           String instruction = "pw335"+(String)powerAvailable335IncomerState;
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("genTopic", data2, 2); 
           #if DEBUG
           Serial.println(instruction);
           #endif
    }
          // save the current state as the last state,
    //for next time through the loop
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailable335IncomerState = powerAvailable335IncomerState;    
  }


  void mgSetContactorAuxRoutine(){
   char data2[750];
    mgSetContactorAuxState = digitalRead(MG_SET_CONTACTOR_AUX);
    if (mgSetContactorAuxState != previousMgSetContactorAuxState) {
        // if the state has changed, increment the counter
        if (mgSetContactorAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)MG_SET_CONTACTOR_AUX+ "state"+ (String)mgSetContactorAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)MG_SET_CONTACTOR_AUX+ "state"+ (String)mgSetContactorAuxState);
            #endif
        }
         String instruction = "mgCAux"+(String)mgSetContactorAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousMgSetContactorAuxState = mgSetContactorAuxState;
  }
  
  void contator335CoilAuxRoutine(){
  char data2[750];

    contator335CoilAuxState = digitalRead(CONTACTOR_335_COIL_AUX); 
    if (contator335CoilAuxState != previousContator335CoilAuxState) {
        // if the state has changed, increment the counter
        if (contator335CoilAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)CONTACTOR_335_COIL_AUX+ "state"+ (String)contator335CoilAuxState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("pinNumber"+ (String)CONTACTOR_335_COIL_AUX+ "state"+ (String)contator335CoilAuxState);
           #endif
        }
         String instruction = "c335CAux"+(String)contator335CoilAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousContator335CoilAuxState = contator335CoilAuxState;
  }

  void powerAvailableDCGridRoutine(){
  char data2[750];

    powerAvailableDCGridState = digitalRead(POWER_AVAILABLE_DATA_CENTER_GRID);
    if (powerAvailableDCGridState != previousPowerAvailableDCGridState) {
        // if the state has changed, increment the counter
        if (powerAvailableDCGridState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_DATA_CENTER_GRID+ "state"+ (String)powerAvailableDCGridState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_DATA_CENTER_GRID+ "state"+ (String)powerAvailableDCGridState);
            #endif
        }
         String instruction = "pwDCG"+(String)powerAvailableDCGridState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableDCGridState = powerAvailableDCGridState;
  }

  void mgSetPowerAvailableRoutine(){
  char data2[750];
    mgSetPowerAvailableState = digitalRead(MG_SET_POWER_AVAILABLE);
     if (mgSetPowerAvailableState != previousMgSetPowerAvailableState) {
        // if the state has changed, increment the counter
        if (mgSetPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)MG_SET_POWER_AVAILABLE+ "state"+ (String)mgSetPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)MG_SET_POWER_AVAILABLE+ "state"+ (String)mgSetPowerAvailableState);
            #endif
        }
         String instruction = "mgPw"+(String)mgSetPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousMgSetPowerAvailableState = mgSetPowerAvailableState;
  }

  void powerAvailableDCOutputRoutine(){
  char data2[750];
    powerAvailableDCOutputState = digitalRead(POWER_AVAILABLE_DATA_CENTER_OUTPUT);
    if (powerAvailableDCOutputState != previousPowerAvailableDCOutputState) {
        // if the state has changed, increment the counter
        if (powerAvailableDCOutputState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_DATA_CENTER_OUTPUT+ "state"+ (String)powerAvailableDCOutputState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_DATA_CENTER_OUTPUT+ "state"+ (String)powerAvailableDCOutputState);
            #endif
        }
         String instruction = "pwDcO"+(String)powerAvailableDCOutputState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableDCOutputState = powerAvailableDCOutputState;
  }

  void dcOutputBreakerAuxRoutine(){
  char data2[750];
    dcOutputBreakerAuxState = digitalRead(DATA_CENTER_OUTPUT_BREAKER_AUX);
    if (dcOutputBreakerAuxState != previousDcOutputBreakerAuxState) {
        // if the state has changed, increment the counter
        if (dcOutputBreakerAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DATA_CENTER_OUTPUT_BREAKER_AUX+ "state"+ (String)dcOutputBreakerAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DATA_CENTER_OUTPUT_BREAKER_AUX+ "state"+ (String)dcOutputBreakerAuxState);
            #endif
        }
         String instruction = "dcOutB"+(String)dcOutputBreakerAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousDcOutputBreakerAuxState = dcOutputBreakerAuxState;
  }

  void stabilizerBreakerToDCAuxRoutine(){
   char data2[750];
    stabilizerBreakerToDCAuxState = digitalRead(STABILIZER_BREAKER_TO_DC_GRID_AUX);
    if (stabilizerBreakerToDCAuxState != previousStabilizerBreakerToDCAuxState) {
        // if the state has changed, increment the counter
        if (stabilizerBreakerToDCAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)STABILIZER_BREAKER_TO_DC_GRID_AUX+ "state"+ (String)stabilizerBreakerToDCAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)STABILIZER_BREAKER_TO_DC_GRID_AUX+ "state"+ (String)stabilizerBreakerToDCAuxState);
            #endif
        }
         String instruction = "stbBDC"+(String)stabilizerBreakerToDCAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousStabilizerBreakerToDCAuxState = stabilizerBreakerToDCAuxState;
  }


  void powerAvailableToStabRoutine(){
  char data2[750];
    powerAvailableToStabState = digitalRead(POWER_AVAILABLE_TO_STABILIZER);
    if (powerAvailableToStabState != previousPowerAvailableToStabState) {
        // if the state has changed, increment the counter
        if (powerAvailableToStabState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_TO_STABILIZER+ "state"+ (String)powerAvailableToStabState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_TO_STABILIZER+ "state"+ (String)powerAvailableToStabState);
            #endif
        }
         String instruction = "pwToStab"+(String)powerAvailableToStabState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableToStabState = powerAvailableToStabState;
  }


 void powerAvailableFromStabRoutine(){
  char data2[750];
    powerAvailableFromStabState = digitalRead(POWER_AVAILABLE_FROM_STABILIZER);
    if (powerAvailableFromStabState != previousPowerAvailableFromStabState) {
        // if the state has changed, increment the counter
        if (powerAvailableFromStabState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_FROM_STABILIZER+ "state"+ (String)powerAvailableFromStabState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_FROM_STABILIZER+ "state"+ (String)powerAvailableFromStabState);
            #endif
        }
         String instruction = "pwFrmStab"+(String)powerAvailableFromStabState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableFromStabState = powerAvailableFromStabState;
  }


 void outputBreakerDataCenterAuxRoutine(){
  char data2[750];
    outputBreakerDataCenterAuxState = digitalRead(OUTPUT_BREAKER_TO_DATA_CENTER_AUX);
    if (outputBreakerDataCenterAuxState != previousOutputBreakerDataCenterAuxState) {
        // if the state has changed, increment the counter
        if (outputBreakerDataCenterAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)OUTPUT_BREAKER_TO_DATA_CENTER_AUX+ "state"+ (String)outputBreakerDataCenterAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)OUTPUT_BREAKER_TO_DATA_CENTER_AUX+ "state"+ (String)outputBreakerDataCenterAuxState);
            #endif
        }
         String instruction = "outBDCA"+(String)outputBreakerDataCenterAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousOutputBreakerDataCenterAuxState = outputBreakerDataCenterAuxState;
 }
