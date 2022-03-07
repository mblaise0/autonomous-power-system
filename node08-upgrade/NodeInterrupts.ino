




int powerAvailableAcbIncomerState = 0;
int previousPowerAvailableAcbIncomerState = 0;

void utilityPowerAvailable() {
   char data2[750];
   powerAvailableAcbIncomerState = digitalRead(POWER_AVAILABLE_ACB_INCOMER);
    if (powerAvailableAcbIncomerState != previousPowerAvailableAcbIncomerState) {
          if (powerAvailableAcbIncomerState == HIGH) {
             currentState = ENABLE;        
             #if DEBUG
              Serial.println("powerAvailableAcbIncomerState"+ (String)POWER_AVAILABLE_ACB_INCOMER+ "state"+ (String)powerAvailableAcbIncomerState);
             #endif
          } else {
              currentState = DISABLE;
               #if DEBUG
               Serial.println("powerAvailableAcbIncomerState"+ (String)POWER_AVAILABLE_ACB_INCOMER+ "state"+ (String)powerAvailableAcbIncomerState);
               #endif
          }  
         String report = "utilPW"+(String)powerAvailableAcbIncomerState;
         report.toCharArray(data2, (report.length() + 1));
         client.publish("displayTopic", data2, 2); 
         #if DEBUG
         Serial.println(report);
         #endif
      }
          // save the current state as the last state,
    //for next time through the loop
    previousPowerAvailableAcbIncomerState = powerAvailableAcbIncomerState;
}
