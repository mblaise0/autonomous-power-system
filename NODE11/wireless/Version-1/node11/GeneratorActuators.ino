
#define START_GEN LOW
#define STOP_GEN LOW
#define RESET HIGH
#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW

int reportGen02StartError = 0;
int reportGen02StartWaitingInterval = 7;

/****************************************************************************************/
void startGen02() {
  char data2[750], data1[10];
  reportGen02StartError++;
  digitalWrite(START_OR_STOP_GEN02, START_GEN);
  client.publish("outTopic", "Gen02 recieved signal", 2); 

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

     client.publish("outTopic", data2, 2);
     client.publish("g02StrtTopic", data1, true); 
     vTaskDelay(500);
     //inform the node controller that gen01 is running
     for (int i = 0; i < 3; i++){
        client.publish("genReport", "g02-running", 2); 
        vTaskDelay(500);
     }
     reportGen02StartError = 0; 
     startGeneratorTwo.disable();  
  }else{
     if (reportGen02StartError == reportGen02StartWaitingInterval){
       digitalWrite(START_OR_STOP_GEN02,  RESET);
       Serial.println("Gen01 process has been enabled due to failure of Gen02...");
       for (int i = 0; i < 3; i++){
          client.publish("catGenOne", "genOneNxt", 2);    //informing node10 that gen01 is next to run( reset node10 Gen01 counter)
          vTaskDelay(500);
       }
       vTaskDelay(4000);
       //Start Gen01 from node 10
       for (int i = 0; i < 3; i++){
         client.publish("strtGen", "STRTGEN", 2);
//         client.publish("errMon", "335gen02", 2);
         vTaskDelay(600 / portTICK_PERIOD_MS);  
        }
       nxtGenSignal = 0;
       startGenSignal = 6;
       stopGenSignal = 6;
       reportGen02StartError = 0;

       String signalCount = (String)startGenSignal;
       String stopSignalCount = (String)stopGenSignal;

       signalCount.toCharArray(data1, (signalCount.length() + 1));
       stopSignalCount.toCharArray(data1, (stopSignalCount.length() + 1));

       client.publish("g02StrtTopic", data1, true);
       client.publish("g02StpTopic", data1, true); 
        
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
   client.publish("outTopic", data2, 2); 
   for (int i = 0; i < 3; i++){
      client.publish("catGenOne", "genOneNxt", 2);    //informing node10 that gen01 is next to run( reset node10 Gen01 counter)
      vTaskDelay(500);
   }
   nxtGenSignal = 0;
   startGenSignal = 6; 
   stopGenSignal = 6; 

   String stopSignalCount = (String)stopGenSignal;
   stopSignalCount.toCharArray(data1, (stopSignalCount.length() + 1));
   client.publish("g02StpTopic", data1, true);
   //supposing node10 is not online when node11 stops, we need to update the counter for node10
   client.publish("g01StrtTopic", "0", true); 
   client.publish("g01StpTopic", "0", true); 
   stopGeneratorTwo.disable();  
  }
}
