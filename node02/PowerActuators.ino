



#define ENGAGE HIGH
#define DISENGAGE LOW

#define DEBUG 1


/************** Breakers ************/
   /*
    * Gen Contactor
    * N.B : At this piont make sure a signal is been sent to other nodes(clients) i.e
    * to disengage 335 and Utility contactors 
    */
   void engageGen01ContactorCoil() {

      //interlock the activity of gen contactors, disengage other contact at the point of engaging one
      int otherGencontactors[] = {GEN_02_CONTACTOR_01_COIL, GEN_02_CONTACTOR_02_COIL, GEN_03_CONTACTOR_01_COIL, GEN_03_CONTACTOR_02_COIL};
      for (int i = 0; i < 4; i++){
        digitalWrite(otherGencontactors[i], DISENGAGE);
      }
      vTaskDelay(500 / portTICK_PERIOD_MS);
      
      digitalWrite(GEN_01_CONTACTOR_01_COIL,  ENGAGE);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalWrite(GEN_01_CONTACTOR_02_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("Gen01ContactorCoils: Engaged"); // + (String)ret); 
      #endif
      activateGen01ContactorCoil.disable(); 
      client.publish("outTopic","Gen01ContactorCoils: Closed");
    }


   void engageGen02ContactorCoil() {
      //interlock the activity of gen contactors, disengage other contact at the point of engaging one
      int otherGencontactors[] = {GEN_01_CONTACTOR_01_COIL, GEN_01_CONTACTOR_02_COIL, GEN_03_CONTACTOR_01_COIL, GEN_03_CONTACTOR_02_COIL};
      for (int i = 0; i < 4; i++){
        digitalWrite(otherGencontactors[i], DISENGAGE);
      }
      vTaskDelay(500 / portTICK_PERIOD_MS);
      
      digitalWrite(GEN_02_CONTACTOR_01_COIL,  ENGAGE);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalWrite(GEN_02_CONTACTOR_02_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("Gen02ContactorCoils: Engaged"); // + (String)ret); 
      #endif
      client.publish("outTopic","Gen02ContactorCoils: Closed");
      activateGen02ContactorCoil.disable();   

    }


    void engageGen03ContactorCoil() {
      //interlock the activity of gen contactors, disengage other contact at the point of engaging one
      int otherGencontactors[] = {GEN_01_CONTACTOR_01_COIL, GEN_01_CONTACTOR_02_COIL, GEN_02_CONTACTOR_01_COIL, GEN_02_CONTACTOR_02_COIL};
      for (int i = 0; i < 4; i++){
        digitalWrite(otherGencontactors[i], DISENGAGE);
      }
      vTaskDelay(500 / portTICK_PERIOD_MS);
      
      digitalWrite(GEN_03_CONTACTOR_01_COIL,  ENGAGE);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalWrite(GEN_03_CONTACTOR_02_COIL,  ENGAGE);
      #if DEBUG
      Serial.println("Gen03ContactorCoils: Engaged"); // + (String)ret); 
      #endif
      client.publish("outTopic","Gen03ContactorCoils: Closed");
      activateGen03ContactorCoil.disable();   
    }

    
    void disengageGen01Contactors() {
      digitalWrite(GEN_01_CONTACTOR_01_COIL,  DISENGAGE);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(GEN_01_CONTACTOR_02_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("All Gen01Contactors are down"); 
      #endif
      deactivateGen01ContactorCoil.disable();
    }

    void disengageGen02Contactors() {
      digitalWrite(GEN_02_CONTACTOR_01_COIL,  DISENGAGE);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(GEN_02_CONTACTOR_02_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("All Gen02Contactors are down"); 
      #endif
      deactivateGen02ContactorCoil.disable();
    }

    void disengageGen03Contactors() {
      digitalWrite(GEN_03_CONTACTOR_01_COIL,  DISENGAGE);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(GEN_03_CONTACTOR_02_COIL,  DISENGAGE);
      #if DEBUG
      Serial.println("All Gen03Contactors are down"); 
      #endif
      deactivateGen03ContactorCoil.disable();
    }

    void ResetAllLoadContactors() {
      int otherGencontactors[] = {GEN_01_CONTACTOR_01_COIL, GEN_01_CONTACTOR_02_COIL,GEN_02_CONTACTOR_01_COIL, GEN_02_CONTACTOR_02_COIL, GEN_03_CONTACTOR_01_COIL, GEN_03_CONTACTOR_02_COIL};
      for (int i = 0; i < 4; i++){
        digitalWrite(otherGencontactors[i], DISENGAGE);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
      #if DEBUG
      Serial.println("All Load-Contactors are down"); 
      #endif
      deactivateAllLoadContactors.disable();
    }
