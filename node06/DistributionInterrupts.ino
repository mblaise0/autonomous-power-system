


#define ACTIVE HIGH

#define CLIENT_ID "Node4"
#define DEBUG 1

#define ENGAGE LOW
#define DISENGAGE HIGH

   int utilityContactor01AuxState = 0;
   int previousUtilityContactor01AuxState = 0;
   
   int utilityContactor02AuxState = 0;
   int previousUtilityContactor02AuxState = 0;
   
   int utilityContactor03AuxState = 0;
   int previousUtilityContactor03AuxState = 0;
   
   int utilityContactor04AuxState = 0;
   int previousUtilityContactor04AuxState = 0;
   
   int utilityContactor05AuxState = 0;
   int previousUtilityContactor05AuxState = 0;
   
   int utilityPowerAvailableState = 0;
   int previousUtilityPowerAvailableState = 0;
   
   int distribution01PowerAvailableState = 0;
   int previousDistribution01PowerAvailableState = 0;
   
   int distribution02PowerAvailableState = 0;
   int previousDistribution02PowerAvailableState = 0;
   
   int distribution03PowerAvailableState = 0;
   int previousDistribution03PowerAvailableState = 0;
   
  
  void utilityContactor01AuxRoutine(){ 
    char data2[750];
    utilityContactor01AuxState = digitalRead(UTILITY_CONTACTOR_01_AUX);
    if (utilityContactor01AuxState != previousUtilityContactor01AuxState) {
          if (utilityContactor01AuxState == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_01_AUX+ "state"+ (String)utilityContactor01AuxState);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_01_AUX+ "state"+ (String)utilityContactor01AuxState);
               #endif
          }
           String instruction = "utilC1"+(String)utilityContactor01AuxState;
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("genTopic", data2, 2); 
           #if DEBUG
           Serial.println(instruction);
           #endif
     }
          // save the current state as the last state,
    //for next time through the loop
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousUtilityContactor01AuxState = utilityContactor01AuxState;    
  }


  void utilityContactor02AuxRoutine(){
    char data2[750];

    utilityContactor02AuxState = digitalRead(UTILITY_CONTACTOR_02_AUX);
    if (utilityContactor02AuxState != previousUtilityContactor02AuxState) {
        // if the state has changed, increment the counter
        if (utilityContactor02AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_02_AUX+ "state"+ (String)utilityContactor02AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_02_AUX+ "state"+ (String)utilityContactor02AuxState);
            #endif
        }
         String instruction = "utilC2"+(String)utilityContactor02AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousUtilityContactor02AuxState = utilityContactor02AuxState;
  }
  
  void utilityContactor03AuxRoutine(){
    char data2[750];
    utilityContactor03AuxState = digitalRead(UTILITY_CONTACTOR_03_AUX); 
    if (utilityContactor03AuxState != previousUtilityContactor03AuxState) {
        // if the state has changed, increment the counter
        if (utilityContactor03AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_03_AUX+ "state"+ (String)utilityContactor03AuxState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_03_AUX+ "state"+ (String)utilityContactor03AuxState);
           #endif
        }
         String instruction = "utilC3"+(String)utilityContactor03AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousUtilityContactor03AuxState = utilityContactor03AuxState;
  }

  void utilityContactor04AuxRoutine(){
    char data2[750];
    utilityContactor04AuxState = digitalRead(UTILITY_CONTACTOR_04_AUX);
    if (utilityContactor04AuxState != previousUtilityContactor04AuxState) {
        // if the state has changed, increment the counter
        if (utilityContactor04AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_04_AUX+ "state"+ (String)utilityContactor04AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_04_AUX+ "state"+ (String)utilityContactor04AuxState);
            #endif
        }
         String instruction = "utilC4"+(String)utilityContactor04AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousUtilityContactor04AuxState = utilityContactor04AuxState;
  }

  void utilityContactor05AuxRoutine(){
    char data2[750];
    utilityContactor05AuxState = digitalRead(UTILITY_CONTACTOR_05_AUX);
     if (utilityContactor05AuxState != previousUtilityContactor05AuxState) {
        // if the state has changed, increment the counter
        if (utilityContactor05AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_05_AUX+ "state"+ (String)utilityContactor05AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_CONTACTOR_05_AUX+ "state"+ (String)utilityContactor05AuxState);
            #endif
        }
         String instruction = "utilC5"+(String)utilityContactor05AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousUtilityContactor05AuxState = utilityContactor05AuxState;
  }

  void utilityPowerAvailableRoutine(){
    char data2[750];
    utilityPowerAvailableState = digitalRead(UTILITY_POWER_AVAILABLE);
    if (utilityPowerAvailableState != previousUtilityPowerAvailableState) {
        // if the state has changed, increment the counter
        if (utilityPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_POWER_AVAILABLE+ "state"+ (String)utilityPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)UTILITY_POWER_AVAILABLE+ "state"+ (String)utilityPowerAvailableState);
            #endif
            //when theres utility failure to distribution disengage all utility contactors to distribution
//            int utilityContactors[] = {UTILITY_CONTACTOR_01_COIL, UTILITY_CONTACTOR_02_COIL, UTILITY_CONTACTOR_03_COIL, UTILITY_CONTACTOR_04_COIL, UTILITY_CONTACTOR_05_COIL};
//            for (int i = 0; i < 5; i++){
//              digitalWrite(utilityContactors[i],  DISENGAGE);
//              vTaskDelay(800/ portTICK_PERIOD_MS);
//            }
        }
         String instruction = "utilPw"+(String)utilityPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousUtilityPowerAvailableState = utilityPowerAvailableState;
  }

  void distribution01PowerAvailableRoutine(){
    char data2[750];
    distribution01PowerAvailableState = digitalRead(DISTRIBUTION_01_POWER_AVAILABLE);
    if (distribution01PowerAvailableState != previousDistribution01PowerAvailableState) {
        // if the state has changed, increment the counter
        if (distribution01PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_01_POWER_AVAILABLE+ "state"+ (String)distribution01PowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_01_POWER_AVAILABLE+ "state"+ (String)distribution01PowerAvailableState);
            #endif
        }
         String instruction = "dis01"+(String)distribution01PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousDistribution01PowerAvailableState = distribution01PowerAvailableState;
  }


  void distribution02PowerAvailableRoutine(){
    char data2[750];
    distribution02PowerAvailableState = digitalRead(DISTRIBUTION_02_POWER_AVAILABLE);
    if (distribution02PowerAvailableState != previousDistribution02PowerAvailableState) {
        // if the state has changed, increment the counter
        if (distribution02PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_02_POWER_AVAILABLE+ "state"+ (String)distribution02PowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_02_POWER_AVAILABLE+ "state"+ (String)distribution02PowerAvailableState);
            #endif
        }
         String instruction = "dis02"+(String)distribution02PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousDistribution02PowerAvailableState = distribution02PowerAvailableState;
  }


 void distribution03PowerAvailableRoutine(){
    char data2[750];
    distribution03PowerAvailableState = digitalRead(DISTRIBUTION_03_POWER_AVAILABLE);
    if (distribution03PowerAvailableState != previousDistribution03PowerAvailableState) {
        // if the state has changed, increment the counter
        if (distribution03PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_03_POWER_AVAILABLE+ "state"+ (String)distribution03PowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_03_POWER_AVAILABLE+ "state"+ (String)distribution03PowerAvailableState);
            #endif
        }
         String instruction = "dis03"+(String)distribution03PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousDistribution03PowerAvailableState = distribution03PowerAvailableState;
  }
