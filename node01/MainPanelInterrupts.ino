#define ACTIVE HIGH
#include "Server.h"

#define CLIENT_ID "Mp-client01"
#define DEBUG 1

#define ACTIVATE_BREAKER LOW
#define RESET_BREAKER HIGH 



 int powerAvailableAcbIncomer = 0;
 int previousStateAcbIncomer = 0;
 
 int overAndUnderVoltageState = 0;
 int previousOverAndUnderVoltageState = 0;
 
 int phaseSequenceState = 0;
 int previousphaseSequenceState = 0;
 
 int acbAuxState = 0;
 int previousAcbAuxState = 0;
 
 int powerAvailableOnUtilityGridState = 0;
 int previousPowerAvailableOnUtilityGridState = 0;
 
 int interlockUtilityBreakerToStabAuxState = 0;
 int previousInterlockUtilityBreakerToStabAuxState = 0;
 
 int genGridDistributionSideBreakerAuxState = 0;
 int previousGenGridDistributionSideBreakerAuxState = 0;
 
 int interlockGenBreakerToStabAuxState = 0;
 int previousInterlockGenBreakerToStabAuxState = 0;

  void acbAuxStateRoutine(){
  char data2[750];

    acbAuxState = digitalRead(ACB_BREAKER_AUX);
    if (acbAuxState != previousAcbAuxState) {
        // if the state has changed, increment the counter
        if (acbAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)ACB_BREAKER_AUX+ "state"+ (String)acbAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)ACB_BREAKER_AUX+ "state"+ (String)acbAuxState);
            #endif
        }
         String instruction = "acbAux"+(String)acbAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousAcbAuxState = acbAuxState;
  }

  void powerAvailableOnUtilityGridStateRoutine(){
  char data2[750];

    powerAvailableOnUtilityGridState = digitalRead(POWER_AVAILABLE_ON_UTILITY_GRID);
     if (powerAvailableOnUtilityGridState != previousPowerAvailableOnUtilityGridState) {
        if (acbAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_ON_UTILITY_GRID+ "state"+ (String)powerAvailableOnUtilityGridState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)POWER_AVAILABLE_ON_UTILITY_GRID+ "state"+ (String)powerAvailableOnUtilityGridState);
            #endif
        }
         String instruction = "pwAUG"+(String)powerAvailableOnUtilityGridState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);
    previousPowerAvailableOnUtilityGridState = powerAvailableOnUtilityGridState;
  }

  void interlockUtilityBreakerToStabAuxStateRoutine(){
    char data2[750];
    
    interlockUtilityBreakerToStabAuxState = digitalRead(INTERLOCK_UTILITY_BREAKER_TO_STAB_AUX);
    if (interlockUtilityBreakerToStabAuxState != previousInterlockUtilityBreakerToStabAuxState) {
        // if the state has changed, increment the counter
        if (acbAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)INTERLOCK_UTILITY_BREAKER_TO_STAB_AUX+ "state"+ (String)interlockUtilityBreakerToStabAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)INTERLOCK_UTILITY_BREAKER_TO_STAB_AUX+ "state"+ (String)interlockUtilityBreakerToStabAuxState);
            #endif
        }
         String instruction = "iubAux"+(String)interlockUtilityBreakerToStabAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2); 
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousInterlockUtilityBreakerToStabAuxState = interlockUtilityBreakerToStabAuxState;
  }

  void genGridDistributionSideBreakerAuxStateRoutine(){
    char data2[750];
    genGridDistributionSideBreakerAuxState = digitalRead(GEN_GRID_DISRTIBUTION_SIDE_BREAKER_AUX);
    if (genGridDistributionSideBreakerAuxState != previousGenGridDistributionSideBreakerAuxState) {
        // if the state has changed, increment the counter
        if (acbAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_GRID_DISRTIBUTION_SIDE_BREAKER_AUX+ "state"+ (String)genGridDistributionSideBreakerAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)GEN_GRID_DISRTIBUTION_SIDE_BREAKER_AUX+ "state"+ (String)genGridDistributionSideBreakerAuxState);
            #endif
        }
         String instruction = "genGDBAux"+(String)genGridDistributionSideBreakerAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2);  
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousGenGridDistributionSideBreakerAuxState = genGridDistributionSideBreakerAuxState;
  }

  void interlockGenBreakerToStabAuxStateRoutine(){
    char data2[750];
    
    interlockGenBreakerToStabAuxState = digitalRead(INTERLOCK_GENERATOR_BREAKER_TO_STAB_AUX);
    if (interlockGenBreakerToStabAuxState != previousInterlockGenBreakerToStabAuxState) {
        // if the state has changed, increment the counter
        if (interlockGenBreakerToStabAuxState == ACTIVE) {
            currentState = ENABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)INTERLOCK_GENERATOR_BREAKER_TO_STAB_AUX+ "state"+ (String)interlockGenBreakerToStabAuxState);
            #endif
        } else {
            currentState = DISABLE;
            #if DEBUG
            Serial.println("pinNumber"+ (String)INTERLOCK_GENERATOR_BREAKER_TO_STAB_AUX+ "state"+ (String)interlockGenBreakerToStabAuxState);
            #endif
//            restAllGenLoadBreakers.enable();
        }
         String instruction = "iGenBAux"+(String)interlockGenBreakerToStabAuxState;
         instruction.toCharArray(data2, (instruction.length() + 1));
         client.publish("genTopic", data2, 2);  
    }
    vTaskDelay( 100 / portTICK_PERIOD_MS);    
    previousInterlockGenBreakerToStabAuxState = interlockGenBreakerToStabAuxState;
  }
