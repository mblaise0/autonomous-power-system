
 enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 State currentState;

int powerAvailableGen02State = 0;
int previousPowerAvailableGen02State = 0;

void powerAvailabilityGenertor02(){
     char data2[750];
   powerAvailableGen02State = digitalRead(POWER_AVAILABLE_GEN02);
    if (powerAvailableGen02State != previousPowerAvailableGen02State) {
          if (powerAvailableGen02State == HIGH) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("powerAvailableGen02"+ (String)POWER_AVAILABLE_GEN02+ "state"+ (String)powerAvailableGen02State);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("powerAvailableGen02"+ (String)POWER_AVAILABLE_GEN02+ "state"+ (String)powerAvailableGen02State);
               #endif
          }  
         String report = "pwrGen2"+(String)powerAvailableGen02State;
         report.toCharArray(data2, (report.length() + 1));
         mqttClient.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(report);
         #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    previousPowerAvailableGen02State = powerAvailableGen02State;
}
