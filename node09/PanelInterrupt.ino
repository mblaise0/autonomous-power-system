
#define ACTIVE HIGH
#define CLIENT_ID "node03"


   int powerAvailableOnGenerator01 = 0;
   int previousPowerAvailableOnGenerator01 = 0;
   
   int generatot01AuxState = 0;
   int previousGeneratot01AuxState = 0;
   
   int generator02AuxState = 0;
   int previousGenerator02AuxState = 0;
   
   int powerAvailableGenerator02State = 0;
   int previousPowerAvailableGenerator02State = 0;

   int powerAvailableOnMainGridState = 0;
   int previousPowerAvailableOnMainGridState = 0;

   int powerAvailableOutgoingBreakerToDatacenterState = 0;
   int previousPowerAvailableOutgoingBreakerToDatacenterRoutine = 0;

  
   void powerAvailableOnGenerator01Routine(){ 
    char data2[750];
    powerAvailableOnGenerator01 = digitalRead(POWER_AVAILABLE_ON_GENERATOR_01);
    if (powerAvailableOnGenerator01 != previousPowerAvailableOnGenerator01) {
          if (powerAvailableOnGenerator01 == ACTIVE) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("powerAvailableOnGenerator01"+ (String)POWER_AVAILABLE_ON_GENERATOR_01+ "state"+ (String)powerAvailableOnGenerator01);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("powerAvailableOnGenerator01"+ (String)POWER_AVAILABLE_ON_GENERATOR_01+ "state"+ (String)powerAvailableOnGenerator01);
               #endif
          }
           String report = "335PWG01"+(String)powerAvailableOnGenerator01;
           report.toCharArray(data2, (report.length() + 1));
           client.publish("genTopic", data2, 2); 
      }
      vTaskDelay( 100 / portTICK_PERIOD_MS);
          // save the current state as the last state,
    //for next time through the loop
    previousPowerAvailableOnGenerator01 = powerAvailableOnGenerator01;    
  }


  void generatot01AuxRoutine(){
    char data2[750];
    generatot01AuxState = digitalRead(GENERATOR_01_AUX);
    if (generatot01AuxState != previousGeneratot01AuxState) {
        // if the state has changed, increment the counter
        if (generatot01AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("generatot01AuxState"+ (String)GENERATOR_01_AUX+ "state"+ (String)generatot01AuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("generatot01AuxState"+ (String)GENERATOR_01_AUX+ "state"+ (String)generatot01AuxState);
            #endif
        }
         String report = "335G01A"+(String)generatot01AuxState;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGeneratot01AuxState = generatot01AuxState;
  }
  

  void powerAvailableGenerator02Routine(){
    char data2[750];
    powerAvailableGenerator02State = digitalRead(POWER_AVAILABLE_ON_GENERATOR_02);
    if (powerAvailableGenerator02State != previousPowerAvailableGenerator02State) {
        // if the state has changed, increment the counter
        if (powerAvailableGenerator02State == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("powerAvailableGenerator02"+ (String)POWER_AVAILABLE_ON_GENERATOR_02+ "state"+ (String)powerAvailableGenerator02State);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("powerAvailableGenerator02"+ (String)POWER_AVAILABLE_ON_GENERATOR_02+ "state"+ (String)powerAvailableGenerator02State);
            #endif
        }
         String report = "335GPW02"+(String)powerAvailableGenerator02State;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableGenerator02State = powerAvailableGenerator02State;
  }


  void generator02AuxRoutine(){
    char data2[750];
    generator02AuxState = digitalRead(GENERATOR_02_AUX); 
    if (generator02AuxState != previousGenerator02AuxState) {
        // if the state has changed, increment the counter
        if (generator02AuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("generator02AuxState"+ (String)GENERATOR_02_AUX+ "state"+ (String)generator02AuxState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("generator02AuxState"+ (String)GENERATOR_02_AUX+ "state"+ (String)generator02AuxState);
           #endif
        }
         String report = "335G02A"+(String)generator02AuxState;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousGenerator02AuxState = generator02AuxState;
  }


  void powerAvailableOnMainGridRoutine(){
    char data2[750];
    powerAvailableOnMainGridState = digitalRead(POWER_AVAILABLE_ON_MAIN_GRID); 
    if (powerAvailableOnMainGridState != previousPowerAvailableOnMainGridState) {
        // if the state has changed, increment the counter
        if (powerAvailableOnMainGridState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("powerAvailableOnMainGrid"+ (String)POWER_AVAILABLE_ON_MAIN_GRID+ "state"+ (String)powerAvailableOnMainGridState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("powerAvailableOnMainGrid"+ (String)POWER_AVAILABLE_ON_MAIN_GRID+ "state"+ (String)powerAvailableOnMainGridState);
           #endif
        }
         String report = "PWAMG"+(String)powerAvailableOnMainGridState;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableOnMainGridState = powerAvailableOnMainGridState;
  }


  void powerAvailableOutgoingBreakerToDatacenterRoutine(){
    char data2[750];
    powerAvailableOutgoingBreakerToDatacenterState = digitalRead(POWER_AVAILABLE_OUTGOING_BREAKER_TO_DC); 
    if (powerAvailableOutgoingBreakerToDatacenterState != previousPowerAvailableOutgoingBreakerToDatacenterRoutine) {
        // if the state has changed, increment the counter
        if (powerAvailableOutgoingBreakerToDatacenterState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("powerAvailableOutgoingBreakerToDatacenter"+ (String)POWER_AVAILABLE_OUTGOING_BREAKER_TO_DC+ "state"+ (String)powerAvailableOnMainGridState);
            #endif
        } else {
           currentState = DISABLE;
           #if DEBUG
           Serial.println("powerAvailableOutgoingBreakerToDatacenter"+ (String)POWER_AVAILABLE_OUTGOING_BREAKER_TO_DC+ "state"+ (String)powerAvailableOnMainGridState);
           #endif
        }
         String report = "PWOBDC"+(String)powerAvailableOutgoingBreakerToDatacenterState;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableOutgoingBreakerToDatacenterRoutine = powerAvailableOutgoingBreakerToDatacenterState;
  }
  
