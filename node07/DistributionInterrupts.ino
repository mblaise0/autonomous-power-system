

#ifndef _MAIN_PANEL_INT
#define _MAIN_PANEL_INT

#define ACTIVE HIGH

#define CLIENT_ID "Node07"
#define DEBUG 1

#define ENGAGE LOW
#define DISENGAGE HIGH

   int genContactor01AuxState = 0;
   int previousGenContactor01AuxState = 0;
   
   int genContactor02AuxState = 0;
   int previousGenContactor02AuxState = 0;
   
   int genContactor03AuxState = 0;
   int previousGenContactor03AuxState = 0;
   
   int genContactor04AuxState = 0;
   int previousGenContactor04AuxState = 0;
   
   int genContactor05AuxState = 0;
   int previousGenContactor05AuxState = 0;
   
   int genPowerAvailableState = 0;
   int previousGenPowerAvailableState = 0;
   
   int distribution04PowerAvailableState = 0;
   int previousDistribution04PowerAvailableState = 0;
   
   int distribution05PowerAvailableState = 0;
   int previousDistribution05PowerAvailableState = 0;

  
  void genContactor01AuxRoutine(){ 
    char data2[750];
    genContactor01AuxState = digitalRead(GEN_CONTACTOR_01_AUX);
    if (genContactor01AuxState != previousGenContactor01AuxState) {
          if (genContactor01AuxState == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("pinNumber"+ (String)GEN_CONTACTOR_01_AUX+ "state"+ (String)genContactor01AuxState);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("pinNumber"+ (String)GEN_CONTACTOR_01_AUX+ "state"+ (String)genContactor01AuxState);
               #endif
          }
         String instruction = "genC1Aux"+(String)genContactor01AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    previousGenContactor01AuxState = genContactor01AuxState;
    
  }


  void genContactor02AuxRoutine(){
    char data2[750];
    genContactor02AuxState = digitalRead(GEN_CONTACTOR_02_AUX);
    if (genContactor02AuxState != previousGenContactor02AuxState) {
        // if the state has changed, increment the counter
        if (genContactor02AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_02_AUX+ "state"+ (String)genContactor02AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_02_AUX+ "state"+ (String)genContactor02AuxState);
            #endif
        }
         String instruction = "genC2Aux"+(String)genContactor02AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
  }
    previousGenContactor02AuxState = genContactor02AuxState;
  }
  
  void genContactor03AuxRoutine(){
    char data2[750];
    genContactor03AuxState = digitalRead(GEN_CONTACTOR_03_AUX); 
    if (genContactor03AuxState != previousGenContactor03AuxState) {
        // if the state has changed, increment the counter
        if (genContactor03AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_03_AUX+ "state"+ (String)genContactor03AuxState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("pinNumber"+ (String)GEN_CONTACTOR_03_AUX+ "state"+ (String)genContactor03AuxState);
           #endif
        }
         String instruction = "genC3Aux"+(String)genContactor03AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenContactor03AuxState = genContactor03AuxState;
  }

  void genContactor04AuxRoutine(){
    char data2[750];
    genContactor04AuxState = digitalRead(GEN_CONTACTOR_04_AUX);
    if (genContactor04AuxState != previousGenContactor04AuxState) {
        // if the state has changed, increment the counter
        if (genContactor04AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_04_AUX+ "state"+ (String)genContactor04AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_04_AUX+ "state"+ (String)genContactor04AuxState);
            #endif
        }
         String instruction = "genC4Aux"+(String)genContactor04AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenContactor04AuxState = genContactor04AuxState;
  }

  void genContactor05AuxRoutine(){
    char data2[750];

    genContactor05AuxState = digitalRead(GEN_CONTACTOR_05_AUX);
     if (genContactor05AuxState != previousGenContactor05AuxState) {
        // if the state has changed, increment the counter
        if (genContactor05AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_05_AUX+ "state"+ (String)genContactor05AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_CONTACTOR_05_AUX+ "state"+ (String)genContactor05AuxState);
            #endif
        }
         String instruction = "genC5Aux"+(String)genContactor04AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenContactor05AuxState = genContactor05AuxState;
  }

  void genPowerAvailableRoutine(){
    char data2[750];
    genPowerAvailableState = digitalRead(GEN_POWER_AVAILABLE);
    if (genPowerAvailableState != previousGenPowerAvailableState) {
        // if the state has changed, increment the counter
        if (genPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_POWER_AVAILABLE+ "state"+ (String)genPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_POWER_AVAILABLE+ "state"+ (String)genPowerAvailableState);
            #endif
            //when theres Gen failure to distribution disengage all utility contactors to distribution
//            int genContactors[] = {GEN_CONTACTOR_01_COIL, GEN_CONTACTOR_02_COIL, GEN_CONTACTOR_03_COIL, GEN_CONTACTOR_04_COIL, GEN_CONTACTOR_05_COIL};
//            for (int i = 0; i < 5; i++){
//              digitalWrite(genContactors[i],  DISENGAGE);
//              vTaskDelay(700/ portTICK_PERIOD_MS);
//            }
        }
         String instruction = "genPw"+(String)genPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousGenPowerAvailableState = genPowerAvailableState;
  }

  void distribution04PowerAvailableRoutine(){
    char data2[750];
    distribution04PowerAvailableState = digitalRead(DISTRIBUTION_04_POWER_AVAILABLE);
    if (distribution04PowerAvailableState != previousDistribution04PowerAvailableState) {
        // if the state has changed, increment the counter
        if (distribution04PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_04_POWER_AVAILABLE+ "state"+ (String)distribution04PowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_04_POWER_AVAILABLE+ "state"+ (String)distribution04PowerAvailableState);
            #endif
        }
         String instruction = "dis04"+(String)distribution04PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousDistribution04PowerAvailableState = distribution04PowerAvailableState;
  }


  void distribution05PowerAvailableRoutine(){
    char data2[750];
    distribution05PowerAvailableState = digitalRead(DISTRIBUTION_05_POWER_AVAILABLE);
    if (distribution05PowerAvailableState != previousDistribution05PowerAvailableState) {
        // if the state has changed, increment the counter
        if (distribution05PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_05_POWER_AVAILABLE+ "state"+ (String)distribution05PowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)DISTRIBUTION_05_POWER_AVAILABLE+ "state"+ (String)distribution05PowerAvailableState);
            #endif
        }
         String instruction = "dis05"+(String)distribution05PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
    }
    previousDistribution05PowerAvailableState = distribution05PowerAvailableState;
  }
