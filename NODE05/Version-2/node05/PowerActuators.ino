

#define numberOfIteration 6

#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVE HIGH


#define ACTIVATE LOW
#define RESET HIGH



/************** Contactors ************/

void CloseUtilityOrGenContactorRoutine() {
  digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE,  ENGAGE);
  #if DEBUG
  Serial.println("InputFromStabilizerOpenAndClose: Engaged"); // + (String)ret); 
  #endif
  client.publish("outTopic","InputFromStabilizerOpenAndClose: Engaged");
  closeUtilityOrGenContactor.disable();
}
 
void OpenUtilityOrGenContactorRoutine() {
  digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE,  DISENGAGE);
  #if DEBUG
  Serial.println("InputFromStabilizerOpenAndClose: Disengaged"); // + (String)ret); 
  #endif
  openUtilityOrGenContactor.disable();
}


void hardResetAllLoadBreakers() {
    digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE, DISENGAGE);
    vTaskDelay(1500/ portTICK_PERIOD_MS);
    //reset breaker
    #if DEBUG
    Serial.println("All Stabilizer-Breakers: Resetted"); 
    #endif
    resetAllLoadBreakers.disable();
}
