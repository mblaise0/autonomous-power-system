
#define START_GEN LOW
#define STOP_GEN LOW
#define RESET HIGH
#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

int reportGen02StartError = 0;
int reportGen02StartWaitingInterval = 6;

/****************************************************************************************/
void startGen02() {
  char data2[750], data1[10];
  reportGen02StartError++;
  digitalWrite(START_OR_STOP_GEN02, START_GEN);
  mqttClient.publish("outTopic", "Gen02 recieved signal", 2); 

  #if DEBUG
  Serial.println("Gen02 Cranking in progress!....");
  #endif
  //IF the cranking failed to start the Gen restart the process of cranking the Gen
  if ((digitalRead(POWER_AVAILABLE_GEN02)) == POWER_AVAILABLE) {
    digitalWrite(START_OR_STOP_GEN02, RESET);
    #if DEBUG
    Serial.println("Success! Gen02 has Started succesfully.");
    #endif
     String report = "Gen02 has started";
     String signalCount = (String)startGenSignal;
     
     report.toCharArray(data2, (report.length() + 1));
     signalCount.toCharArray(data1, (signalCount.length() + 1));

     mqttClient.publish("outTopic", data2, 2);
     mqttClient.publish("g02StrtTopic", data1, true); 
     //inform the node controller that gen01 is running
     for (int i = 0; i < 3; i++){
        mqttClient.publish("genReport", "g02-running", 2); 
        delayTime = 500;
        start_time_micros = millis();
        while ((millis() - start_time_micros) < delayTime){
        ;
       }
     }
     stopGenSignal = 0;
     reportGen02StartError = 0; 
     startGeneratorTwo.disable();  
  }else{
     if (reportGen02StartError == reportGen02StartWaitingInterval){
       digitalWrite(START_OR_STOP_GEN02,  RESET);
       Serial.println("Gen01 process has been enabled due to failure of Gen02...");
       reportGen02StartError = 0;
       startGenSignal = 0;
       stopGenSignal = 0;
       startGeneratorTwo.disable();
       stopGeneratorTwo.disable();    
     }
  }
}


void stopGen02() {
  char data2[750], data1[10];
  digitalWrite(START_OR_STOP_GEN02, STOP_GEN);
  #if DEBUG
  Serial.println("Gen02 Stopping in progress!....");
  #endif
  //IF the cranking failed to start the Gen restart the process of cranking the Gen
  if ((digitalRead(POWER_AVAILABLE_GEN02)) == POWER_UNAVAILABLE) {
    digitalWrite(START_OR_STOP_GEN02, RESET);
    #if DEBUG
    Serial.println("Success! Gen02 has ShutDown succesfully.");
    #endif
   String report = "Gen02 has stopped";
   report.toCharArray(data2, (report.length() + 1));
   mqttClient.publish("outTopic", data2, 2); 
   for (int i = 0; i < 3; i++){
     mqttClient.publish("genReport", "g02-stpd", 2); 
   }
   startGenSignal = 0;
   stopGeneratorTwo.disable();  
  }
}
