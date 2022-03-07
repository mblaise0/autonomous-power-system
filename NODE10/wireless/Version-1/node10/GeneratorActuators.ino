

#define START_GEN LOW
#define STOP_GEN LOW
#define RESET HIGH
#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

int reportGen01StartError = 0;
int reportGen01StartWaitingInterval = 7;

void startGen01() {
  char data2[1024];
  char data1[10];
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
     signalCount.toCharArray(data1, (signalCount.length() + 1));

     client.publish("outTopic", data2, 2); 
     client.publish("g01StrtTopic", data1, true); 
     vTaskDelay(500);
     //inform the node controller that gen01 is running
     for (int i = 0; i < 3; i++){
        client.publish("genReport", "g01-running", 2); 
        vTaskDelay(500);
     }
     reportGen01StartError = 0;
     startGeneratorOne.disable();  
  }else{
    if (reportGen01StartError == reportGen01StartWaitingInterval){
       digitalWrite(START_OR_STOP_GEN01, RESET);
       Serial.println("Gen01 process has been enabled due to failure of Gen01...");
       for (int i = 0; i < 3; i++){
          client.publish("catGenTwo", "genTwoNxt", 2);    //informing node10 that gen01 is next to run( reset node10 Gen01 counter)
          vTaskDelay(500);
       }
       vTaskDelay(4000);
       for (int i = 0; i < 3; i++){
         client.publish("strtGen", "STRTGEN", 2);
//         client.publish("errMon", "335gen02", 2);
         vTaskDelay(600 / portTICK_PERIOD_MS);  
        }
        nxtGenSignal = 0;
        startGenSignal = 3;
        stopGenSignal = 3;
        reportGen01StartError = 0;
        
        String signalCount = (String)startGenSignal;
        String stopSignalCount = (String)stopGenSignal;

        signalCount.toCharArray(data1, (signalCount.length() + 1));
        stopSignalCount.toCharArray(data1, (stopSignalCount.length() + 1));

        client.publish("g01StrtTopic", data1, true);
        client.publish("g01StpTopic", data1, true); 
        
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
       client.publish("catGenTwo", "genTwoNxt", 2);    //informing node09(assuming it is online) that its generators are next to run
       vTaskDelay(500);
     }
     nxtGenSignal = 0;
     startGenSignal = 3;
     stopGenSignal = 3;
     
     String stopSignalCount = (String)stopGenSignal;
     stopSignalCount.toCharArray(data1, (stopSignalCount.length() + 1));
     client.publish("g01StpTopic", data1, true); 
     //if node11 is offline at the point of stopping node10 then we need to update the counter for node11
     client.publish("g02StrtTopic", "3", true); 
     client.publish("g02StpTopic", "3", true); 
     stopGeneratorOne.disable();  
  }
}
