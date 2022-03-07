

#define ACTIVE HIGH

#define CLIENT_ID "Mp-client02"
#define DEBUG 1

#define ENGAGE HIGH
#define DISENGAGE LOW
 

   int gen01PowerAvailableState = 0;
   int previousGen01PowerAvailableState = 0;
   
   int gen01Contactor01AuxState = 0;
   int previousGen01Contactor01AuxState = 0;
   
   int gen01Contactor02AuxState = 0;
   int previousGen01Contactor02AuxState = 0;
   
   int gen02PowerAvailableState = 0;
   int previousGen02PowerAvailableState = 0;
   
   int gen02Contactor01AuxState = 0;
   int previousGen02Contactor01AuxState = 0;
   
   int gen02Contactor02AuxState = 0;
   int previousGen02Contactor02AuxState = 0;
   
   int gen03PowerAvailableState = 0;
   int previousGen03PowerAvailableState = 0;
   
   int gen03Contactor01AuxState = 0;
   int previousGen03Contactor01AuxState = 0;
   
   int gen03Contactor02AuxState = 0;
   int previousGen03Contactor02AuxState = 0;

   int genGridPowerAvailableState = 0;
   int previousGenGridPowerAvailableState = 0;
   

  void gen01PowerAvailableRoutine(){ 
  char data2[750];

    gen01PowerAvailableState = digitalRead(GEN_01_POWER_AVAILABLE);
    if (gen01PowerAvailableState != previousGen01PowerAvailableState) {
          if (gen01PowerAvailableState == ACTIVE) {
             currentState = ENABLE;        
//             #if DEBUG
//              Serial.println("pinNumber"+ (String)GEN_01_POWER_AVAILABLE+ "state"+ (String)gen01PowerAvailableState);
//             #endif
          } else {
              currentState = DISABLE;
//               #if DEBUG
//               Serial.println("pinNumber"+ (String)GEN_01_POWER_AVAILABLE+ "state"+ (String)gen01PowerAvailableState);
//               #endif
          }

         String instruction = "gen01Av"+(String)gen01PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(instruction);
         #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGen01PowerAvailableState = gen01PowerAvailableState;
    
  }

  void gen01ContactorsAuxRoutine() {
     char data2[750];

      gen01Contactor01AuxState = digitalRead(GEN_01_CONTACTOR_01_AUX);
      if (gen01Contactor01AuxState != previousGen01Contactor01AuxState) {
          // if the state has changed, increment the counter
          if (gen01Contactor01AuxState == ACTIVE) {
              currentState = ENABLE;
          } else {
              currentState = DISABLE;
          } 
       }
      gen01Contactor02AuxState = digitalRead(GEN_01_CONTACTOR_02_AUX); 
      if (gen01Contactor02AuxState != previousGen01Contactor02AuxState) {
          // if the state has changed, increment the counter
          if (gen01Contactor02AuxState == ACTIVE) {
              currentState = ENABLE;
          } else {
             currentState = DISABLE;
          }
           String instruction = "g1aux"+(String)gen01Contactor01AuxState+"."+(String)gen01Contactor02AuxState;
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("genTopic", data2, 2); 
          #if DEBUG
          Serial.println(instruction);
          #endif
      }
      vTaskDelay( 100 / portTICK_PERIOD_MS);
      previousGen01Contactor01AuxState = gen01Contactor01AuxState;
      previousGen01Contactor02AuxState = gen01Contactor02AuxState;
  }

  void gen02PowerAvailableRoutine(){
     char data2[750];
     
    gen02PowerAvailableState = digitalRead(GEN_02_POWER_AVAILABLE);
    if (gen02PowerAvailableState != previousGen02PowerAvailableState) {
        // if the state has changed, increment the counter
        if (gen02PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
//            #if DEBUG
//            Serial.println("pinNumber"+ (String)GEN_02_POWER_AVAILABLE+ "state"+ (String)gen02PowerAvailableState);
//            #endif
        } else {
            currentState = DISABLE;
//            #if DEBUG
//            Serial.println("pinNumber"+ (String)GEN_02_POWER_AVAILABLE+ "state"+ (String)gen02PowerAvailableState);
//            #endif
        }
         String instruction = "gen02Av"+(String)gen02PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
          #if DEBUG
         Serial.println(instruction);
         #endif    
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGen02PowerAvailableState = gen02PowerAvailableState;
  }


  void gen02ContactorsAuxRoutine(){
     char data2[750];

     gen02Contactor01AuxState = digitalRead(GEN_02_CONTACTOR_01_AUX);
     if (gen02Contactor01AuxState != previousGen02Contactor01AuxState) {
        // if the state has changed, increment the counter
        if (gen02Contactor01AuxState == ACTIVE) {
            currentState = ENABLE;
        } else {
            currentState = DISABLE;
        }
    }

    gen02Contactor02AuxState = digitalRead(GEN_02_CONTACTOR_02_AUX);
    if (gen02Contactor02AuxState != previousGen02Contactor02AuxState) {
        // if the state has changed, increment the counter
        if (gen02Contactor02AuxState == ACTIVE) {
            currentState = ENABLE;
        } else {
            currentState = DISABLE;
        }
       String instruction = "g2aux"+(String)gen02Contactor01AuxState+"."+(String)gen02Contactor02AuxState;
       instruction.toCharArray(data2, (instruction.length() + 1));
       client.publish("genTopic", data2, 2); 
      #if DEBUG
      Serial.println(instruction);
      #endif

    }
     vTaskDelay( 100 / portTICK_PERIOD_MS);
     previousGen02Contactor01AuxState = gen02Contactor01AuxState;
     previousGen02Contactor02AuxState = gen02Contactor02AuxState;
  }
  
  void gen03PowerAvailableRoutine(){
     char data2[750];
     
    gen03PowerAvailableState = digitalRead(GEN_03_POWER_AVAILABLE);
    if (gen03PowerAvailableState != previousGen03PowerAvailableState) {
        // if the state has changed, increment the counter
        if (gen03PowerAvailableState == ACTIVE) {
            currentState = ENABLE;
//            #if DEBUG
//            Serial.println("pinNumber"+ (String)GEN_03_POWER_AVAILABLE+ "state"+ (String)gen03PowerAvailableState);
//            #endif
        } else {
            currentState = DISABLE;
//            #if DEBUG
//            Serial.println("pinNumber"+ (String)GEN_03_POWER_AVAILABLE+ "state"+ (String)gen03PowerAvailableState);
//            #endif
        }
         String instruction = "gen03Av"+(String)gen03PowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
          #if DEBUG
         Serial.println(instruction);
         #endif  
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGen03PowerAvailableState = gen03PowerAvailableState;
  }


  void gen03ContactorsAuxRoutine() {
     char data2[750];

    gen03Contactor01AuxState = digitalRead(GEN_03_CONTACTOR_01_AUX);
    if (gen03Contactor01AuxState != previousGen03Contactor01AuxState) {
        // if the state has changed, increment the counter
        if (gen03Contactor01AuxState == ACTIVE) {
            currentState = ENABLE;
        } else {
            currentState = DISABLE;
        }
    }
    
    gen03Contactor02AuxState = digitalRead(GEN_03_CONTACTOR_02_AUX);
    if (gen03Contactor02AuxState != previousGen03Contactor02AuxState) {
        // if the state has changed, increment the counter
        if (gen03Contactor02AuxState == ACTIVE) {
            currentState = ENABLE;
        } else {
            currentState = DISABLE;
        }
         String instruction = "g3aux"+(String)gen03Contactor01AuxState+"."+(String)gen03Contactor02AuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
        #if DEBUG
        Serial.println(instruction);
        #endif
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
     previousGen03Contactor02AuxState = gen03Contactor02AuxState;
     previousGen03Contactor01AuxState = gen03Contactor01AuxState;
  }

 void genGridPowerAvailableRoutine(){
     char data2[750];
    genGridPowerAvailableState = digitalRead(GEN_GRID_POWER_AVAILABLE);
    if (genGridPowerAvailableState != previousGenGridPowerAvailableState) {
        // if the state has changed, increment the counter
        if (genGridPowerAvailableState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_GRID_POWER_AVAILABLE+ "state"+ (String)genGridPowerAvailableState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_GRID_POWER_AVAILABLE+ "state"+ (String)genGridPowerAvailableState);
            #endif
            /*
             * Interlock all Load Actuators when theres powerfailure on Gen Grid
             */
//             int otherGencontactors[] = {GEN_01_CONTACTOR_01_COIL, GEN_01_CONTACTOR_02_COIL,GEN_02_CONTACTOR_01_COIL, GEN_02_CONTACTOR_02_COIL, GEN_03_CONTACTOR_01_COIL, GEN_03_CONTACTOR_02_COIL};
//              for (int i = 0; i < 4; i++){
//                digitalWrite(otherGencontactors[i], DISENGAGE);
//                vTaskDelay(500 / portTICK_PERIOD_MS);
//              }
        }
         String instruction = "ggpw"+(String)genGridPowerAvailableState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
          #if DEBUG
         Serial.println(instruction);
         #endif  
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGenGridPowerAvailableState = genGridPowerAvailableState;
  }
