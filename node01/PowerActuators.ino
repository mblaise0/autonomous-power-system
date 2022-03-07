

#define ACTIVATE_BREAKER LOW
#define RESET_BREAKER HIGH 

#define ENGAGE HIGH
#define DISENGAGE LOW

#include "Server.h"

#define DEBUG 1





//
//    deactivateAcbBreaker.enable();
//    activateAcbBreaker.enable();
//    activateInterlockUtilityBreakerToStab.enable();
//    deactivateInterlockUtilityBreakerToStab.enable();
//    activateGenGridDistributionSideBreaker.enable();
//    deactivateGenGridDistributionSideBreaker.enable();
//    activateInterlockGenBreakerToStab.enable();
//    deactivateInterlockGenBreakerToStab.enable();
    
/************** Breakers ************/
   //AcbBreaker
   void openAcbBreaker() {
      digitalWrite(ACB_DRIVE_OPEN,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("ACB Opened"); // + (String)ret); 
      #endif
      //set back the pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(ACB_DRIVE_OPEN,  RESET_BREAKER);
      deactivateAcbBreaker.disable();
    }

   void closeAcbBreaker() {
      digitalWrite(ACB_DRIVE_CLOSE,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("ACB Closed"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(ACB_DRIVE_CLOSE,  RESET_BREAKER);
      client.publish("outTopic","ACB: Closed");
      activateAcbBreaker.disable();
    }
    
    //interlockUtilityBreakerToStab
    void closeinterlockUtilityBreakerToStab() {

      digitalWrite(INTERLOCK_UTILITY_BREAKER_TO_STAB_CLOSE,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("interlockUtilityBreakerToStab Closed"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(INTERLOCK_UTILITY_BREAKER_TO_STAB_CLOSE,  RESET_BREAKER);
      client.publish("outTopic","interlockUtilityBreakerToStab: Closed");
      activateInterlockUtilityBreakerToStab.disable();
    }

    void openinterlockUtilityBreakerToStab() {
      digitalWrite(INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("interlockUtilityBreakerToStab Opened"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN,  RESET_BREAKER);
      deactivateInterlockUtilityBreakerToStab.disable();
    }

    //GenGridDistributionSideBreaker
    void closeGenGridDistributionSideBreaker() {
      digitalWrite(GEN_GRID_DISRTIBUTION_SIDE_BREAKER_CLOSE,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("GenGridDistributionSideBreaker Closed"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(GEN_GRID_DISRTIBUTION_SIDE_BREAKER_CLOSE,  RESET_BREAKER);
      client.publish("outTopic","GenGridDistributionSideBreaker: Closed");
      activateGenGridDistributionSideBreaker.disable();
    }

    void openGenGridDistributionSideBreaker() {
      digitalWrite(GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("GenGridDistributionSideBreaker Opened"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1500 / portTICK_PERIOD_MS);
      digitalWrite(GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN,  RESET_BREAKER);
      deactivateGenGridDistributionSideBreaker.disable();
    }

    //interlockGenBreakerToStab
    void closeInterlockGenBreakerToStab() {

      digitalWrite(INTERLOCK_GENERATOR_BREAKER_TO_STAB_CLOSE,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("interlockGenBreakerToStab Closed"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(INTERLOCK_GENERATOR_BREAKER_TO_STAB_CLOSE,  RESET_BREAKER);
      client.publish("outTopic","interlockGenBreakerToStab: Closed");
      activateInterlockGenBreakerToStab.disable();
    }

    void openInterlockGenBreakerToStab() {
      digitalWrite(INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN,  ACTIVATE_BREAKER);
      #if DEBUG
      Serial.println("interlockGenBreakerToStab Opened"); 
      #endif
      //set back th pin to be high to reset the trigger signal
      vTaskDelay(1200 / portTICK_PERIOD_MS);
      digitalWrite(INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN,  RESET_BREAKER);
      deactivateInterlockGenBreakerToStab.disable();
    }

    void deactivateAllLoadBreakers(){
      int OpenBreakers[] = {INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN, ACB_DRIVE_OPEN, INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN};
      for (int i = 0; i < 4; i++){
        digitalWrite((OpenBreakers[i]), ACTIVATE_BREAKER);
        vTaskDelay(1200 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All load Breakers are opened");
       #endif
      int resetBreakers[] = {INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN, ACB_DRIVE_OPEN, INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN};
      for (int i = 0; i < 4; i++){
        digitalWrite((resetBreakers[i]), RESET_BREAKER);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All load Breakers open pins has been resetted");
       #endif
       restAllLoadBreakers.disable();
    }


    void deactivateAllUtilityLoadBreakers(){
      int OpenBreakers[] = {ACB_DRIVE_OPEN, INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite((OpenBreakers[i]), ACTIVATE_BREAKER);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All Utility load Breakers are opened");
       #endif
      int resetBreakers[] = {ACB_DRIVE_OPEN, INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite((resetBreakers[i]), RESET_BREAKER);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All Utility load Breakers open pins has been resetted");
       #endif
       restAllUtilityLoadBreakers.disable();
    }


     void deactivateAllGenLoadBreakers(){
      int OpenBreakers[] = {INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite((OpenBreakers[i]), ACTIVATE_BREAKER);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All Gen load Breakers are opened");
       #endif
      int resetBreakers[] = {INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN};
      for (int i = 0; i < 2; i++){
        digitalWrite((resetBreakers[i]), RESET_BREAKER);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
       #if DEBUG
       Serial.println("All load Gen Breakers open pins has been resetted");
       #endif
       restAllGenLoadBreakers.disable();
    }
