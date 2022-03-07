

#define START_GEN LOW
#define STOP_GEN LOW
#define RESET HIGH
#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

int reportGen01StartError = 0;
int reportGen01StartWaitingInterval = 10;

void startGen01() {
  char data2[1024];
  reportGen01StartError++; 
  digitalWrite(START_OR_STOP_GEN01, START_GEN);
  client.publish("outTopic", "Gen01 recieved signal", 2); 
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

     client.publish("outTopic", data2, 2); 
     vTaskDelay(500);
     //inform the node controller that gen01 is running
     for (int i = 0; i < 3; i++){
        client.publish("genReport", "g01-running", 2); 
        vTaskDelay(500);
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
     client.publish("outTopic", data2, 2); 
     for (int i = 0; i < 3; i++){
       client.publish("genReport", "g01-stpd", 2); 
     }
     startGenSignal = 0;  
     stopGeneratorOne.disable();  
  }
}
