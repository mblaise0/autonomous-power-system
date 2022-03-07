

#define START_GEN LOW
#define STOP_GEN LOW
#define RESET HIGH
#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

int reportGen01StartError = 0;
int reportGen01StartWaitingInterval = 5;

void startGen01() {
  char data2[1024];
  reportGen01StartError++; 
  digitalWrite(START_OR_STOP_GEN01, START_GEN);
  mqttClient.publish("outTopic", "Gen01 recieved signal", 2); 
  #if DEBUG
  Serial.println("Gen01 Cranking in progress!....");
  #endif
  //IF the cranking failed to start the Gen restart the process of cranking the Gen
  if ((digitalRead(POWER_AVAILABLE_GEN01)) == POWER_AVAILABLE) {
    digitalWrite(START_OR_STOP_GEN01, RESET);
    #if DEBUG
    Serial.println("Success! Gen01 has Started succesfully.");
    Serial.println("Present startGenSignalCount: "+(String)startGenSignal);
    #endif
     String report = "Gen01 has started";
     String signalCount = (String)startGenSignal;
     
     report.toCharArray(data2, (report.length() + 1));

     mqttClient.publish("outTopic", data2, 2); 
     //inform the node controller that gen01 is running
     for (int i = 0; i < 3; i++){
        mqttClient.publish("genReport", "g01-running", 2); 
        delayTime = 500;
        start_time_micros = millis();
        while ((millis() - start_time_micros) < delayTime){
        ;
       }
     }
     stopGenSignal = 0;  
     reportGen01StartError = 0;
     startGeneratorOne.disable();  
  }else{
    if (reportGen01StartError == reportGen01StartWaitingInterval){
       digitalWrite(START_OR_STOP_GEN01, RESET);
       Serial.println("Gen02 process has been enabled due to failure of Gen01...");
        reportGen01StartError = 0;
        startGenSignal = 0;
        stopGenSignal = 0;
        startGeneratorOne.disable();
        stopGeneratorOne.disable();  
    }  
  }
}


void stopGen01() {
  char data2[750];
  char data1[10];
  digitalWrite(START_OR_STOP_GEN01, STOP_GEN);
  #if DEBUG
  Serial.println("Gen01 Stopping in progress!....");
  #endif
  //IF the cranking failed to start the Gen restart the process of cranking the Gen
  if ((digitalRead(POWER_AVAILABLE_GEN01)) == POWER_UNAVAILABLE) {
    digitalWrite(START_OR_STOP_GEN01, RESET);
    #if DEBUG
    Serial.println("Success! Gen01 has ShutDown succesfully.");
    #endif
     String report = "Gen01 has stopped";
     report.toCharArray(data2, (report.length() + 1));
     mqttClient.publish("outTopic", data2, 2); 
     for (int i = 0; i < 3; i++){
       mqttClient.publish("genReport", "g01-stpd", 2); 
     }
     startGenSignal = 0;  
     stopGeneratorOne.disable();  
  }
}



/******************************************************************************************/

void restAllLoadActuators(){
  digitalWrite(START_OR_STOP_GEN01, RESET);
  restGenStartStop.disable();

//  int generatorPins[] = {START_OR_STOP_GEN01, START_OR_STOP_GEN02, START_OR_STOP_GEN03};
//  for (int i = 0; i < 3; i++){
//    digitalWrite( generatorPins[i], RESET);
//  }
}
