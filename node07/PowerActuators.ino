


#define numberOfIteration 6

#define ENGAGE LOW
#define DISENGAGE HIGH

#define ACTIVATE LOW
#define RESET HIGH

  
/************** Contactors ************/
   /*
    * MGset Contactor
    * N.B : At this piont make sure a signal is been sent to other nodes(clients) i.e
    * to disengage Gen and Utility contactors 
    */
    void enableGenContactor01CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_01_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor01oil: Engaged"); // + (String)ret); 
      #endif
      enableGenContactor01Coil.disable();
    }
    void disableGenContactor01CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_01_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("UtilityContactor01oil: Disengaged"); // + (String)ret); 
      #endif
      disableGenContactor01Coil.disable();
    }

    
    void enableGenContactor02CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_02_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("genContactor02Coil: Engaged"); // + (String)ret); 
      #endif
      enableGenContactor02Coil.disable();
    }
    void disableGenContactor02CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_02_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("genContactor02Coil: Disengaged"); // + (String)ret); 
      #endif
      disableGenContactor02Coil.disable();
    }

    void enableGenContactor03CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_03_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("genContactor03Coil: Engaged");
      #endif
      enableGenContactor03Coil.disable();
    }
    void disableGenContactor03CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_03_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("genContactor03Coil: Disengaged");
      #endif
      disableGenContactor03Coil.disable();
    }
    
    void enableGenContactor04CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_04_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("genContactor04Coil: Engaged");
      #endif
      enableGenContactor04Coil.disable();
    }
    void disableGenContactor04CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_04_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("genContactor04Coil: Disengaged");
      #endif
      disableGenContactor04Coil.disable();
    }

    void enableGenContactor05CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_05_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("genContactor05Coil: Engaged");
      #endif
      enableGenContactor05Coil.disable();
    }
    void disableGenContactor05CoilRoutine() {
      digitalWrite(GEN_CONTACTOR_05_COIL,  ACTIVATE);
      #if DEBUG
      Serial.println("genContactor05Coil: Disengaged"); 
      #endif
      disableGenContactor05Coil.disable();
    }

    void disengageAllGenContactorsRoutine() {
      int genContactors[] = {GEN_CONTACTOR_01_COIL, GEN_CONTACTOR_02_COIL, GEN_CONTACTOR_03_COIL, GEN_CONTACTOR_04_COIL, GEN_CONTACTOR_05_COIL};
      for (int i = 0; i < 5; i++){
        digitalWrite(genContactors[i],  DISENGAGE);
        vTaskDelay(700/ portTICK_PERIOD_MS);
      }
      #if DEBUG
      Serial.println("All GenContactorCoils: Disengaged"); 
      #endif
      disengageAllGenContactors.disable();
    }

   void engageAllGenContactorsRoutine() {
      int genContactors[] = {GEN_CONTACTOR_01_COIL, GEN_CONTACTOR_02_COIL, GEN_CONTACTOR_03_COIL, GEN_CONTACTOR_04_COIL, GEN_CONTACTOR_05_COIL};
      for (int i = 0; i < 5; i++){
        digitalWrite(genContactors[i],  ENGAGE);
        vTaskDelay(700/ portTICK_PERIOD_MS);
      }
      #if DEBUG
      Serial.println("All GenContactorCoils: Engaged"); 
      #endif
      client.publish("outTopic","All-GenContactorCoils: Closed");
      engageAllGenContactors.disable();
    }

#endif
