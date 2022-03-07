
#define numberOfIteration 6

#define ENGAGE LOW
#define DISENGAGE HIGH

#define ACTIVATE LOW
#define RESET HIGH

//  activateUtilityContactor01Coil.enable(); 
//  deactivateUtilityContactor01Coil.enable();
//  activateUtilityContactor02Coil.enable(); 
//  deactivateUtilityContactor02Coil.enable();
//  activateUtilityContactor03Coil.enable(); 
//  deactivateUtilityContactor03Coil.enable();
//  activateUtilityContactor04Coil.enable();   
//  deactivateUtilityContactor04Coil.enable();
//  activateUtilityContactor05Coil.enable(); 
//  activateUtilityContactor05Coil.enable(); 
  
/************** Contactors ************/

    void disengageAllUtilityContactorsRoutine() {
      int utilityContactors[] = {UTILITY_CONTACTOR_01_COIL, UTILITY_CONTACTOR_02_COIL, UTILITY_CONTACTOR_03_COIL, UTILITY_CONTACTOR_04_COIL, UTILITY_CONTACTOR_05_COIL};
      for (int i = 0; i < 5; i++){
        digitalWrite(utilityContactors[i],  DISENGAGE);
        vTaskDelay(800/ portTICK_PERIOD_MS);
      }
      #if DEBUG
      Serial.println("All UtilityContactorsCoil: Disengaged"); 
      #endif
      disengageAllUtilityContactors.disable(); 
    }

   void engageAllUtilityContactorsRoutine() {

      int utilityContactors[] = {UTILITY_CONTACTOR_01_COIL, UTILITY_CONTACTOR_02_COIL, UTILITY_CONTACTOR_03_COIL, UTILITY_CONTACTOR_04_COIL, UTILITY_CONTACTOR_05_COIL};
      for (int i = 0; i < 5; i++){
        digitalWrite(utilityContactors[i],  ENGAGE);
        vTaskDelay(500/ portTICK_PERIOD_MS);
      }
      #if DEBUG
      Serial.println("All UtilityContactorsCoil: Engaged"); 
      #endif
      client.publish("outTopic","UtilityContactorsCoil: All-Closed");
      engageAllUtilityContactors.disable(); 
    }

/************************************************************/

    void enableUtilityContactor01CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_01_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor01oil: Engaged"); // + (String)ret); 
      #endif
      activateUtilityContactor01Coil.disable(); 
    }
    void disableUtilityContactor01CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_01_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor01oil: Disengaged"); // + (String)ret); 
      #endif
      deactivateUtilityContactor01Coil.disable();
    }

    
    void enableUtilityContactor02CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_02_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor02Coil: Engaged"); // + (String)ret); 
      #endif
      activateUtilityContactor02Coil.disable(); 
    }
    void disableUtilityContactor02CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_02_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor02Coil: Disengaged"); // + (String)ret); 
      #endif
      deactivateUtilityContactor02Coil.disable();
    }

    void enableUtilityContactor03CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_03_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor03Coil: Engaged");
      #endif
      activateUtilityContactor03Coil.disable(); 
    }
    void disableUtilityContactor03CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_03_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor03Coil: Disengaged");
      #endif
      deactivateUtilityContactor03Coil.disable();
    }
    
    void enableUtilityContactor04CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_04_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor04Coil: Engaged");
      #endif
      activateUtilityContactor04Coil.disable(); 
    }
    void disableUtilityContactor04CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_04_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor04Coil: Disengaged");
      #endif
      deactivateUtilityContactor04Coil.disable();
    }

    void enableUtilityContactor05CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_05_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor05Coil: Engaged");
      #endif
      activateUtilityContactor05Coil.disable(); 
    }
    void disableUtilityContactor05CoilRoutine() {
      digitalWrite(UTILITY_CONTACTOR_05_COIL,  ACTIVATE);
      #if DEBUG
      Serial.println("UtilityContactor05Coil: Disengaged"); 
      #endif
      deactivateUtilityContactor05Coil.disable();
    }
