

#define ACTIVE HIGH

#define CLIENT_ID "Node05"
#define DEBUG 1


   int bypassBreakerPowerAvailableState = 0;
   int previousBypassBreakerPowerAvailableState = 0;
   
   int bypassBreakerAuxState = 0;
   int previousBypassBreakerAuxState = 0;
   
   int utilityInputToStabPowerAvailableState = 0;
   int previousUtilityInputToStabPowerAvailableState = 0;
   
   int mainBusBarPowerAvailableState = 0;
   int previousMainBusBarPowerAvailableState = 0;
   
   int utilityOrGenContactorAuxState = 0;
   int previousUtilityOrGenContactorAuxState = 0;

 
  void bypassBreakerPowerAvailableRoutine(){ 
    char data2[750];
    bypassBreakerPowerAvailableState = digitalRead(BYPASS_BREAKER_POWER_AVAILABLE_AUX);
    if (bypassBreakerPowerAvailableState != previousBypassBreakerPowerAvailableState) {
          if (bypassBreakerPowerAvailableState == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("bypassBreakerPowerAvailableState"+ (String)BYPASS_BREAKER_POWER_AVAILABLE_AUX+ "state"+ (String)bypassBreakerPowerAvailableState);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("bypassBreakerPowerAvailableState"+ (String)BYPASS_BREAKER_POWER_AVAILABLE_AUX+ "state"+ (String)bypassBreakerPowerAvailableState);
               #endif
          }
           String instruction = "byBPwA"+(String)bypassBreakerPowerAvailableState;
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("genTopic", data2, 2); 
           #if DEBUG
           Serial.println(instruction);
           #endif
    }
          // save the current state as the last state,
    //for next time through the loop
    previousBypassBreakerPowerAvailableState = bypassBreakerPowerAvailableState;
    
  }


  void bypassBreakerAuxRoutine(){
    char data2[750];
    bypassBreakerAuxState = digitalRead(BYPASS_BREAKER_AUX);
    if (bypassBreakerAuxState != previousBypassBreakerAuxState) {
        // if the state has changed, increment the counter
        if (bypassBreakerAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("bypassBreakerAuxState"+ (String)BYPASS_BREAKER_AUX+ "state"+ (String)bypassBreakerAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("bypassBreakerAuxState"+ (String)BYPASS_BREAKER_AUX+ "state"+ (String)bypassBreakerAuxState);
            #endif
        }
         String instruction = "byBPAux"+(String)bypassBreakerAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif 
    }
    previousBypassBreakerAuxState = bypassBreakerAuxState;
  }
  
  void utilityInputToStabPowerAvailableRoutine(){
  char data2[750];
    utilityInputToStabPowerAvailableState = digitalRead(UTILITY_INPUT_TO_STAB_POWER_AVAILABLE); 
    if (utilityInputToStabPowerAvailableState != previousUtilityInputToStabPowerAvailableState) {
        // if the state has changed, increment the counter
        if (utilityInputToStabPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("utilityInputToStabPowerAvailableState"+ (String)UTILITY_INPUT_TO_STAB_POWER_AVAILABLE+ "state"+ (String)utilityInputToStabPowerAvailableState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("utilityInputToStabPowerAvailableState"+ (String)UTILITY_INPUT_TO_STAB_POWER_AVAILABLE+ "state"+ (String)utilityInputToStabPowerAvailableState);
           #endif
        }
         String instruction = "uInToSP"+(String)utilityInputToStabPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif  
    }
    previousUtilityInputToStabPowerAvailableState = utilityInputToStabPowerAvailableState;
  }

  void mainBusBarPowerAvailableRoutine(){
    char data2[750];
    mainBusBarPowerAvailableState = digitalRead(MAIN_BUS_BAR_POWER_AVAILABLE);
    if (mainBusBarPowerAvailableState != previousMainBusBarPowerAvailableState) {
        // if the state has changed, increment the counter
        if (mainBusBarPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("mainBusBarPowerAvailableState"+ (String)MAIN_BUS_BAR_POWER_AVAILABLE+ "state"+ (String)mainBusBarPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("mainBusBarPowerAvailableState"+ (String)MAIN_BUS_BAR_POWER_AVAILABLE+ "state"+ (String)mainBusBarPowerAvailableState);
            #endif
        }
         String instruction = "mainBPw"+(String)mainBusBarPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousMainBusBarPowerAvailableState = mainBusBarPowerAvailableState;
  }

  void utilityOrGenContactorAuxRoutine(){
   char data2[750];

    utilityOrGenContactorAuxState = digitalRead(UTILITY_OR_GEN_CONTACTOR_AUX);
     if (utilityOrGenContactorAuxState != previousUtilityOrGenContactorAuxState) {
        // if the state has changed, increment the counter
        if (utilityOrGenContactorAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("utilityOrGenContactorAuxState"+ (String)UTILITY_OR_GEN_CONTACTOR_AUX+ "state"+ (String)utilityOrGenContactorAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("utilityOrGenContactorAuxState"+ (String)UTILITY_OR_GEN_CONTACTOR_AUX+ "state"+ (String)utilityOrGenContactorAuxState);
            #endif
        }
         String instruction = "uOrGCAux"+(String)utilityOrGenContactorAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
      }
    previousUtilityOrGenContactorAuxState = utilityOrGenContactorAuxState;
  }
