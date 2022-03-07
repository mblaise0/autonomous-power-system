


int powerAvailableGen01State = 0;
int previousPowerAvailableGen01State = 0;

enum State {
DISABLE = 0,
ENABLE = 1  
};
State currentState;


void powerAvailabilityGenertor01() {
   char data2[750];
   powerAvailableGen01State = digitalRead(POWER_AVAILABLE_GEN01);
    if (powerAvailableGen01State != previousPowerAvailableGen01State) {
          if (powerAvailableGen01State == HIGH) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("powerAvailableGen01"+ (String)POWER_AVAILABLE_GEN01+ "state"+ (String)powerAvailableGen01State);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("powerAvailableGen01"+ (String)POWER_AVAILABLE_GEN01+ "state"+ (String)powerAvailableGen01State);
               #endif
          }  
         String report = "pwrGen"+(String)powerAvailableGen01State;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("genTopic", data2, 2); 
         #if DEBUG
         Serial.println(report);
         #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    previousPowerAvailableGen01State = powerAvailableGen01State;
}
