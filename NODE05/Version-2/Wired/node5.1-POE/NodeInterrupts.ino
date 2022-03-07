


#define ACTIVE HIGH

#define CLIENT_ID "Node05"
#define DEBUG 1


   int bypassBreakerPowerAvailableState = 0;
   int previousBypassBreakerPowerAvailableState = 0;
   
   int bypassBreakerAuxState = 0;
   int previousBypassBreakerAuxState = 0;

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
           mqttClient.publish("genTopic", data2, 2); 
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
         mqttClient.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif 
    }
    previousBypassBreakerAuxState = bypassBreakerAuxState;
  }
