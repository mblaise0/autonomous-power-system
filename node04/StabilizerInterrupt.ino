
#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVATE LOW
#define RESET HIGH

#define ACTIVE HIGH

#define CLIENT_ID "Node5"
#define DEBUG 1


   int inputFromStabilizerContactorAvailableState = 0;
   int previousInputFromStabilizerContactorAvailableState = 0;
   
   int outputFromStabilizerContactorAvailableState = 0;
   int previousOutputFromStabilizerContactorAvailableState = 0;
   
   int stabilizerContactorAuxState = 0;
   int previousStabilizerContactorAuxState = 0;
   
   int genInputAvailableState = 0;
   int previousGenInputAvailableState = 0;
   
   int genOutputAvailableState = 0;
   int previousGenOutputAvailableState = 0;
   
   int genAuxState = 0;
   int previousGenAuxState = 0;
   
   int utilityToStabilizerPowerAvailableState = 0;
   int previousUtilityToStabilizerPowerAvailableState = 0;
   
   int utilityToStabilizerBreakerAuxState = 0;
   int previousUtilityToStabilizerBreakerAuxState = 0;


  void inputFromStabilizerContactorAvailableRoutine(){ 
  char data2[750];
    inputFromStabilizerContactorAvailableState = digitalRead(INPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE);
    if (inputFromStabilizerContactorAvailableState != previousInputFromStabilizerContactorAvailableState) {
          if (inputFromStabilizerContactorAvailableState == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("pinNumber"+ (String)INPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE+ "state"+ (String)inputFromStabilizerContactorAvailableState);
             #endif
          } else {
             currentState = DISABLE;
             #if DEBUG
             Serial.println("pinNumber"+ (String)INPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE+ "state"+ (String)inputFromStabilizerContactorAvailableState);
             #endif
          }
           String instruction = "inFrmStb"+(String)inputFromStabilizerContactorAvailableState;
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("genTopic", data2, 2); 
           #if DEBUG
           Serial.println(instruction);
           #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    previousInputFromStabilizerContactorAvailableState = inputFromStabilizerContactorAvailableState;
    
  }


  void outputFromStabilizerContactorAvailableRoutine(){
  char data2[750];
    outputFromStabilizerContactorAvailableState = digitalRead(OUTPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE);
    if (outputFromStabilizerContactorAvailableState != previousOutputFromStabilizerContactorAvailableState) {
        // if the state has changed, increment the counter
        if (outputFromStabilizerContactorAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)OUTPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE+ "state"+ (String)outputFromStabilizerContactorAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)OUTPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE+ "state"+ (String)outputFromStabilizerContactorAvailableState);
            #endif
        }
         String instruction = "OfrmStb"+(String)outputFromStabilizerContactorAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousOutputFromStabilizerContactorAvailableState = outputFromStabilizerContactorAvailableState;
  }
  
  void stabilizerContactorAuxRoutine(){
  char data2[750];
    stabilizerContactorAuxState = digitalRead(STABILIZER_CONTACTOR_AUX); 
    if (stabilizerContactorAuxState != previousStabilizerContactorAuxState) {
        // if the state has changed, increment the counter
        if (stabilizerContactorAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)STABILIZER_CONTACTOR_AUX+ "state"+ (String)stabilizerContactorAuxState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("pinNumber"+ (String)STABILIZER_CONTACTOR_AUX+ "state"+ (String)stabilizerContactorAuxState);
           #endif
        }
         String instruction = "stabCAux"+(String)stabilizerContactorAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousStabilizerContactorAuxState = stabilizerContactorAuxState;
  }

  void genInputAvailableRoutine(){
  char data2[750];
    genInputAvailableState = digitalRead(GEN_INPUT_AVAILABLE);
    if (genInputAvailableState != previousGenInputAvailableState) {
        // if the state has changed, increment the counter
        if (genInputAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_INPUT_AVAILABLE+ "state"+ (String)genInputAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_INPUT_AVAILABLE+ "state"+ (String)genInputAvailableState);
            #endif
        }
         String instruction = "genIn"+(String)genInputAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenInputAvailableState = genInputAvailableState;
  }

  void genOutputAvailableRoutine(){
  char data2[750];
    genOutputAvailableState = digitalRead(GEN_OUTPUT_AVAILABLE);
     if (genOutputAvailableState != previousGenOutputAvailableState) {
        // if the state has changed, increment the counter
        if (genOutputAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_OUTPUT_AVAILABLE+ "state"+ (String)genOutputAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_OUTPUT_AVAILABLE+ "state"+ (String)genOutputAvailableState);
            #endif
        }
         String instruction = "genOut"+(String)genOutputAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenOutputAvailableState = genOutputAvailableState;
  }

  void genAuxRoutine(){
    char data2[750];
    genAuxState = digitalRead(GEN_AUX);
    if (genAuxState != previousGenAuxState) {
        // if the state has changed, increment the counter
        if (genAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_AUX+ "state"+ (String)genAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_AUX+ "state"+ (String)genAuxState);
            #endif
        }
         String instruction = "genAux"+(String)genAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif 
    }
    previousGenAuxState = genAuxState;
  }

  void utilityToStabilizerPowerAvailableRoutine(){
    char data2[750];
    utilityToStabilizerPowerAvailableState = digitalRead(UTILITY_TO_STABILIZER_POWER_AVAILABLE);
    if (utilityToStabilizerPowerAvailableState != previousUtilityToStabilizerPowerAvailableState) {
        // if the state has changed, increment the counter
        if (utilityToStabilizerPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_TO_STABILIZER_POWER_AVAILABLE+ "state"+ (String)utilityToStabilizerPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_TO_STABILIZER_POWER_AVAILABLE+ "state"+ (String)utilityToStabilizerPowerAvailableState);
            #endif
        }
         String instruction = "utilToStabPA"+(String)utilityToStabilizerPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
     }
    previousUtilityToStabilizerPowerAvailableState = utilityToStabilizerPowerAvailableState;
  }


  void utilityToStabilizerBreakerAuxRoutine(){
    char data2[750];
    utilityToStabilizerBreakerAuxState = digitalRead(UTILITY_TO_STABILIZER_BREAKER_AUX);
    if (utilityToStabilizerBreakerAuxState != previousUtilityToStabilizerBreakerAuxState) {
        // if the state has changed, increment the counter
        if (utilityToStabilizerBreakerAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_TO_STABILIZER_BREAKER_AUX+ "state"+ (String)utilityToStabilizerBreakerAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_TO_STABILIZER_BREAKER_AUX+ "state"+ (String)utilityToStabilizerBreakerAuxState);
            #endif
        }
         String instruction = "utilToStabAux"+(String)utilityToStabilizerBreakerAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousUtilityToStabilizerBreakerAuxState = utilityToStabilizerBreakerAuxState;
  }
