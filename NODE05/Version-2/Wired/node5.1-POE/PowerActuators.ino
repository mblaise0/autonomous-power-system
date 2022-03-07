
#define numberOfIteration 6

#define ENGAGE HIGH
#define DISENGAGE LOW

#define ACTIVE HIGH


#define ACTIVATE LOW
#define RESET HIGH


void CloseBypassBreakerRoutine() {
  digitalWrite(BYPASS_BREAKER_CLOSE,  ACTIVATE);
  //reset trigger signal
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  digitalWrite(BYPASS_BREAKER_CLOSE,  RESET);
  #if DEBUG
  Serial.println("By-pass Breaker: Closed"); // + (String)ret); 
  #endif
  mqttClient.publish("outTopic","GenInputToStabilizer: Engaged");
  closeBypassBreaker.disable();
}

void OpenBypassBreakerRoutine() {
  digitalWrite(BYPASS_BREAKER_OPEN,  ACTIVATE);
  //reset trigger signal
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  digitalWrite(BYPASS_BREAKER_OPEN,  RESET);
  #if DEBUG
  Serial.println("By-pass Breaker: Opened"); // + (String)ret); 
  #endif
  openBypassBreaker.disable();
}

    
void hardResetAllLoadBreakers() {
  digitalWrite(BYPASS_BREAKER_OPEN,  ACTIVATE);
  vTaskDelay(2000/ portTICK_PERIOD_MS);
  //reset breaker
  digitalWrite(BYPASS_BREAKER_OPEN,  RESET);
  #if DEBUG
  Serial.println("All Stabilizer-Breakers: Resetted"); 
  #endif
  resetAllLoadBreakers.disable();
}
