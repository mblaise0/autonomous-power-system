
#define ACTIVATE LOW
#define DEACTIVATE HIGH 

#define CLOSE LOW
#define OPEN HIGH

#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

#define START_GEN LOW
#define STOP_GEN LOW

#define RESET_SIGNAL LOW
#define RESET HIGH

    void startGen01() {
      char data2[750];
      digitalWrite(GENERATOR_01_START_AND_STOP,  START_GEN);
      Serial.println("335Gen01 Cranking in progress!....");
      if ((digitalRead(POWER_AVAILABLE_ON_GENERATOR_01)) == POWER_AVAILABLE) {
         digitalWrite(GENERATOR_01_START_AND_STOP,  RESET);
         #if DEBUG
         Serial.println("Success! 335Gen01: Started"); // + (String)ret); 
         #endif
         String report = "335Gen01 has started";
         report.toCharArray(data2, (report.length() + 1));
         client.publish("outTopic", data2, 2); 
         client.publish("genTopic", "33kva-running", 2); 
         activateStartGen01.disable();
      }
    }
    
    void stopGen01() {
      char data2[750];
      digitalWrite(GENERATOR_01_START_AND_STOP,  STOP_GEN);
      Serial.println("335Gen01 Stopping in progress!....");
      if ((digitalRead(POWER_AVAILABLE_ON_GENERATOR_01)) == POWER_UNAVAILABLE) {
         digitalWrite(GENERATOR_01_START_AND_STOP,  RESET);
         #if DEBUG
         Serial.println("Success! 335Gen01: Stopped"); // + (String)ret); 
         #endif
         String report = "335Gen01 has stopped";
         report.toCharArray(data2, (report.length() + 1));
         client.publish("outTopic", data2, 2); 
         activateStopGen01.disable();
      }
    }
/********************************************************************/
    void engage335Gen01ContactorCoil() {
      char data2[750];
      digitalWrite(GENERATOR_01_COIL_OPEN_AND_CLOSE,  ACTIVATE);
      #if DEBUG
      Serial.println("MgSetContactorCoil: stopped");  
      #endif
      String report = "335Contactor has Engaged";
      report.toCharArray(data2, (report.length() + 1));
      client.publish("outTopic", data2, 2); 
      activate335Gen01ContactorCoil.disable();
    }

    void disengage335Gen01ContactorCoil() {
      digitalWrite(GENERATOR_01_COIL_OPEN_AND_CLOSE,  DEACTIVATE);
      #if DEBUG
      Serial.println("MgSetContactorCoil: stopped");  
      #endif
      deactivate335Gen01ContactorCoil.disable();
    }
/********************************************************************/

    void closeOutBreaker() {
      char data2[750];
      digitalWrite(OUTPUT_BREAKER_CLOSE,  ACTIVATE);
      #if DEBUG
      Serial.println("outBreaker: Closed"); 
      #endif
      //reset the coil of the breaker
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      digitalWrite(OUTPUT_BREAKER_CLOSE, RESET); 
      #if DEBUG
      Serial.println("outBreakerSignal: Resetted"); 
      #endif
      String report = "335OutputBreaker has Closed";
      report.toCharArray(data2, (report.length() + 1));
      client.publish("outTopic", data2, 2);
      activateOutBreaker.disable();
    }

    void openOutputBreaker(){
      digitalWrite(OUTPUT_BREAKER_OPEN, ACTIVATE);
      //reset the breaker coil
      digitalWrite(OUTPUT_BREAKER_OPEN, RESET);
      #if DEBUG
      Serial.println("OutBreaker: Opened");
      #endif
      deactivateOutBreaker.disable();
    }
/********************************************************************/

    void startGen02() {
      char data2[750];
      digitalWrite(GENERATOR_02_START_AND_STOP,  START_GEN);
      Serial.println("335Gen02 Cranking in progress!....");

      if ((digitalRead(POWER_AVAILABLE_ON_GENERATOR_02)) == POWER_AVAILABLE) {
        digitalWrite(GENERATOR_02_START_AND_STOP,  RESET);
        #if DEBUG
        Serial.println("startGen02: Activated"); // + (String)ret); 
        #endif
        String report = "335Gen02 has Started";
        report.toCharArray(data2, (report.length() + 1));
        client.publish("outTopic", data2, 2);
        client.publish("genTopic", "33kva-running", 2); 
        activateStartGen02.disable();
      }
      startGenSignal = 0;
      startGenSignal02 = 0;
    }
    
    void stopGen02() {
      char data2[750];
      digitalWrite(GENERATOR_02_START_AND_STOP,  STOP_GEN);
      Serial.println("335Gen02 Stopping in progress!....");
      if ((digitalRead(POWER_AVAILABLE_ON_GENERATOR_02)) == POWER_UNAVAILABLE) {
         digitalWrite(GENERATOR_02_START_AND_STOP,  RESET);
         #if DEBUG
         Serial.println("Success! 335Gen02: Stopped"); // + (String)ret); 
         #endif
    
         String report = "335Gen02 has stopped";
         report.toCharArray(data2, (report.length() + 1));
         client.publish("outTopic", data2, 2); 
         activateStopGen02.disable();
      }
      stopGenSignal = 0;
      stopGenSignal2 = 0;
    }
 /********************************************************************/
   
    void engage335Gen02ContactorCoil() {
      char data2[750];
      digitalWrite(GENERATOR_02_COIL_OPEN_AND_CLOSE,  ACTIVATE);
      #if DEBUG
      Serial.println("MgSetContactorCoil: stopped"); // + (String)ret); 
      #endif
      String report = "335Gen02Contactor has Started";
      report.toCharArray(data2, (report.length() + 1));
      client.publish("outTopic", data2, 2);
      activate335Gen02ContactorCoil.disable();
    }

/********************************************************************/

  void resetAllLoadActuators() {
      char data2[750];
      int contactorsPins[] = {GENERATOR_01_COIL_OPEN_AND_CLOSE, GENERATOR_02_COIL_OPEN_AND_CLOSE};
      for (int i = 0; i < 2; i++){
        digitalWrite(contactorsPins[i], HIGH);
        vTaskDelay(700 / portTICK_PERIOD_MS);
      }
    
      int breaker[] = {OUTPUT_BREAKER_OPEN};
      for (int i = 0; i < 1; i++){
        digitalWrite(breaker[i], HIGH);
        vTaskDelay(700 / portTICK_PERIOD_MS);
      }

      #if DEBUG
      Serial.println("Success! All 335kva Load Actuators has been Resetted");
      #endif
      String report = "335Panel ready for power-change";
      report.toCharArray(data2, (report.length() + 1));
      client.publish("outTopic", data2, 2);
      activateResetForAllLoadActuators.disable();
  }
