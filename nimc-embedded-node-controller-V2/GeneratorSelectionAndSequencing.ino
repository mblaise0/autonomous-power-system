
#define START_GEN HIGH
#define STOP_GEN HIGH
#define RESET LOW


//if a gen needs to start for about 21times a week, we can have a condition such that allows it to run 21 times before it reset
//e.g if (selectFitGenerator <= 21) for gen01 gen02 if ((selectFitGenerator > 21) && (selectFitGenerator >= 42))
//same goes for the stop signal

void startFitGenerator() {
  if (selectFitGenerator == 1){
    startGen01.enable();
  }else if (selectFitGenerator == 2){
    startGen02.enable();
    #if DEBUG
    Serial.println("*************Start Gen02 condition to start has been met**************");
    #endif
    selectFitGenerator = 0;            // this is resetted at this point because the 335kva Generator  is not added to the cycle yet
  }else if (selectFitGenerator == 3){
    //we can set a condition here that only triggers at a particular time
//    if (//set time when this should be triggered){
        startGen335kvaGenerator.enable();
//    }
  }
//  client.publish("SSG", "0", true);         // reset the UtilityGood counter the broker
  startGeneratorFitToRun.disable();
}


void startGenerator01Routine() {
   char data2[1024];
   startGen01Counter++;
   startGen01AllowedWaitTime++;
   
  if ((startGen01Counter == 1) ||(startGen01Counter == 5)) {
    String instruction = "STRTGEN";
    instruction.toCharArray(data2, (instruction.length() + 1));
    for (int i = 0; i < 3; i++){
      client.publish("node10", data2, 2);
      vTaskDelay(400 / portTICK_PERIOD_MS);  
    }
    startGen01AllowedWaitTime = 0;   //reset the wait time for reset
    selectFitGenerator = 1;
    client.publish("fitGenStrtC", "1", true);
    Serial.println("Success! Mandate To start generator01 has been issued.");
  }

  if (startGen01AllowedWaitTime == 5) {
       char data3[750];
        startGen02.enable();         
        selectFitGenerator = 0;      //in the case when a gen needs to run for a number of times in a circle, this will be modified to the counter where Gen02 starts
        stopGenRunningCount = 1;
        String reportSelectedFitGeneratorCount = (String)selectFitGenerator;
        String reportStopGenRunningCount = (String)stopGenRunningCount;

        reportSelectedFitGeneratorCount.toCharArray(data3, (reportSelectedFitGeneratorCount.length() + 1));
        reportStopGenRunningCount.toCharArray(data3, (reportStopGenRunningCount.length() + 1));

        client.publish("fitGenStrtC", data3, true);
        client.publish("stpRGenC", data3, true);
        
        startGen01Counter = 0;
        startGen01AllowedWaitTime = 0;
        startGen01.disable();
      Serial.println("Gen01 has Failed to start, therefore process to Gen02 has been enabled...");
  }
}

void startGenerator02Routine() {
   char data2[750];
   startGen02AllowedWaitTime++;
   startGen02Counter++;
   Serial.println("startGen02Counter: "+ (String)startGen02Counter);
  if ((startGen02Counter == 1) || (startGen02Counter == 3)) {
      String instruction = "STRTGEN";
      instruction.toCharArray(data2, (instruction.length() + 1));
      for (int i = 0; i < 3; i++){
        client.publish("node11", data2, 2);
        vTaskDelay(400 / portTICK_PERIOD_MS);  
      }
      Serial.println("Success! Mandate To start generator02 has been issued.");
      startGen02AllowedWaitTime = 0;
      selectFitGenerator = 0;
      client.publish("fitGenStrtC", "0", true);
  }
  if (startGen02AllowedWaitTime == 4) {
        char data3[750];
        startGen01.enable();
        selectFitGenerator = 1;
        stopGenRunningCount = 0;
        String reportSelectedFitGeneratorCount = (String)selectFitGenerator;
        String reportStopGenRunningCount = (String)stopGenRunningCount;

        reportSelectedFitGeneratorCount.toCharArray(data3, (reportSelectedFitGeneratorCount.length() + 1));
        reportStopGenRunningCount.toCharArray(data3, (reportStopGenRunningCount.length() + 1));

        client.publish("fitGenStrtC", data3, true);
        client.publish("stpRGenC", data3, true);
        
        startGen02Counter = 0;
        startGen02AllowedWaitTime = 0;
        startGen02.disable();
      Serial.println("Gen02 has Failed to start, therefore process to Gen01 has been enabled...");
  }
}


void startGenerator03Routine() {
   char data2[750];
   startGen03AllowedWaitTime++;
   startGen03Counter++;
   Serial.println("startGen03Counter: "+ (String)startGen03Counter);
  if ((startGen03Counter == 1) || (startGen03Counter == 5)) {
      String instruction = "STRTGEN";
      instruction.toCharArray(data2, (instruction.length() + 1));
      for (int i = 0; i < 3; i++){
        client.publish("node11", data2, 2);
        vTaskDelay(400 / portTICK_PERIOD_MS);  
      }
      Serial.println("Success! Mandate To start generator03 has been issued.");
      startGen03AllowedWaitTime = 0;
      selectFitGenerator = 0;
      stopGenRunningCount = 1;
      client.publish("fitGenStrtC", "0", true);
  }
  if (startGen03AllowedWaitTime == 5) {
        char data3[750];
        startGen01.enable();
        selectFitGenerator = 1;
        stopGenRunningCount = 0;
        String reportSelectedFitGeneratorCount = (String)selectFitGenerator;
        String reportStopGenRunningCount = (String)stopGenRunningCount;

        reportSelectedFitGeneratorCount.toCharArray(data3, (reportSelectedFitGeneratorCount.length() + 1));
        reportStopGenRunningCount.toCharArray(data3, (reportStopGenRunningCount.length() + 1));

        client.publish("fitGenStrtC", data3, true);
        client.publish("stpRGenC", data3, true);
        
        startGen03Counter = 0;
        startGen03AllowedWaitTime = 0;
        startGen03.disable();
      Serial.println("Gen03 has Failed to start, therefore process to Gen01 has been enabled...");
  }
}


void startGen335kvaGeneratorRoutine() {
   char data1[750];

  String instruction = "STRTGEN";
  instruction.toCharArray(data1, (instruction.length() + 1));
  for (int i = 0; i < 3; i++){
    client.publish("node09", data1, 2);
    vTaskDelay(400 / portTICK_PERIOD_MS);  
  }
  Serial.println("Success! Mandate To start generator has been issued.");
  startGen335kvaGenerator.disable();
}

/**********************************************************************************/


void stopGeneratorRunningRoutine(){
  if (stopGenRunningCount == 1) {
    stopGenerator01.enable();
  }else if (stopGenRunningCount == 2){
    stopGenerator02.enable();
    stopGenRunningCount = 0;     // this is resetted at this point because the 335kva Generator  is not added to the cycle yet
  }else if (stopGenRunningCount == 3){
    stop335kvaGenerator.enable();
  }
//  client.publish("SSB", "0", true);    // reset the UtilityNotGoood counter the broker
  stopGeneratorRuning.disable();
}


void stopGenerator01Routine() {
  char data2[750];
  gen01StopErrorCount++;
  gen01StpCounter++;
  String instruction = "STPGEN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if (gen01StpCounter == 1){
    for (int i = 0; i < 3; i++){
      client.publish("node10", data2, 2);
      vTaskDelay(400 / portTICK_PERIOD_MS);  
    }
    Serial.println("Success! Mandate To stop generator01 has been issued.");
    client.publish("stpRGenC", "1", true);
  }
  /*
   * start a counter for the gen if the gen doesnt stop before 5mins then report error
   */
  Serial.println("gen01StopErrorCount: "+ (String)gen01StopErrorCount);
  if (gen01StopErrorCount == 78){         //5mins = 78 counts
     client.publish("gen01Err", "gen01FailedStp", 2);
     Serial.println("Error! Generator01 has failed to stop.");
     gen01StpCounter = 0;
     gen01StopErrorCount = 0;
     stopGenerator01.disable();
  }
}


void stopGenerator02Routine() {
  char data3[750];
  gen02StopErrorCount++;
  gen02StpCounter++;
  String instruction = "STPGEN";
  instruction.toCharArray(data3, (instruction.length() + 1));
  if (gen02StpCounter == 1){
    for (int i = 0; i < 3; i++){
      client.publish("node11", data3, 2);
      vTaskDelay(400 / portTICK_PERIOD_MS);  
    }
    Serial.println("Success! Mandate To stop generator02 has been issued.");
    client.publish("stpRGenC", "0", true);
  }
  /*
   * start a counter for the gen if the gen doesnt stop before 5mins then report error
   */
  Serial.println("gen02StopErrorCount: "+ (String)gen02StopErrorCount);
  if (gen02StopErrorCount == 78){         //5mins = 78 count
     client.publish("gen02Err", "gen02FailedStp", 2);
     Serial.println("Error! Generator02 has failed to stop.");
     gen02StpCounter = 0;
     gen02StopErrorCount = 0;
     stopGenerator02.disable();
  }
}


void stopGenerator03Routine() {
  char data3[750];
  gen03StopErrorCount++;
  gen03StpCounter++;
  String instruction = "STPGEN";
  instruction.toCharArray(data3, (instruction.length() + 1));
  if (gen03StpCounter == 1){
    for (int i = 0; i < 3; i++){
      client.publish("node12", data3, 2);
      vTaskDelay(400 / portTICK_PERIOD_MS);  
    }
    Serial.println("Success! Mandate To stop generator03 has been issued.");
    client.publish("stpRGenC", "0", true);
  }
  /*
   * start a counter for the gen if the gen doesnt stop before 5mins then report error
   */
   Serial.println("gen03StopErrorCount: "+ (String)gen03StopErrorCount);
  if (gen03StopErrorCount == 78){         //5mins = 78 counts
     client.publish("gen03Err", "gen03FailedStp", 2);
     Serial.println("Error! Generator03 has failed to stop.");
  }
}

void stop335kvaGeneratorRoutine() {
  char data1[750];

  String instruction = "STPGEN";
  instruction.toCharArray(data1, (instruction.length() + 1));
  for (int i = 0; i < 3; i++){
    client.publish("node09", data1, 2);
    vTaskDelay(400 / portTICK_PERIOD_MS);  
  }
  Serial.println("Success! Mandate To stop stop335kvaGenerator has been issued.");
  stop335kvaGenerator.disable();
}
/*******************************************************************************/


void engage335kvaContactor01Routine(){
  char data2[750];
  String instruction = "E.335G01C";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node9), 1) {
    client.publish("node09", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  vTaskDelay(600 / portTICK_PERIOD_MS); 
  Serial.println("Success! Mandate To engage335kva01Contactor issued.");
  engage335kva01Contactor.disable();
}

void engage335kvaContactor02Routine(){
  char data2[750];
  String instruction = "E.335G02C";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node9), 1) {
    client.publish("node09", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  vTaskDelay(600 / portTICK_PERIOD_MS); 
  Serial.println("Success! Mandate To engage335kva01Contactor issued.");
  engage335kva02Contactor.disable();
}


void closeOutputBreaker335kvaPanelRoutine(){
  char data2[750];
  String instruction = "E.335OB";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node9), 1) {
    client.publish("node09", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  vTaskDelay(600 / portTICK_PERIOD_MS); 
  Serial.println("Success! Mandate To closeOutputBreaker335kvaPanel issued.");
  closeOutputBreaker335kvaPanel.disable();
}


void engage335ContactorRoutine() {
  char data2[750];
  String instruction = "E.C335C";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node3), 1) {
    client.publish("node03", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  vTaskDelay(600 / portTICK_PERIOD_MS); 
  Serial.println("Success! Mandate To engage335Contactor issued.");
  engage335Contactor.disable();
}


void updateGen01Counter() {
  
  client.publish("g01StrtTopic", "3", true);
  client.publish("g01StpTopic", "0", true); 
  client.publish("g02StrtTopic", "3", true);
  client.publish("g02StpTopic", "0", true);
}

void updateGen02Counter() {
  
  client.publish("g02StrtTopic", "6", true);
  client.publish("g02StpTopic", "3", true);
  client.publish("g01StrtTopic", "0", true);
  client.publish("g01StpTopic", "3", true); 
}
