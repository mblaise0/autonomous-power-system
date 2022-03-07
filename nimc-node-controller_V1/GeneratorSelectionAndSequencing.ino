
#define START_GEN HIGH
#define STOP_GEN HIGH
#define RESET LOW


void startFitGenerator() {
   char data2[750];

  String instruction = "STRTGEN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  for (int i = 0; i < 3; i++){
    client.publish("strtGen", data2, 2);
    vTaskDelay(400 / portTICK_PERIOD_MS);  
  }
  Serial.println("Success! Mandate To start generator has been issued.");
  
  startGeneratorFitToRun.disable();

}


void stopGenerator() {
  char data2[750];

  String instruction = "STPGEN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  for (int i = 0; i < 3; i++){
    client.publish("stpGen", data2, 2);
    vTaskDelay(400 / portTICK_PERIOD_MS);  
  }
  Serial.println("Success! Mandate To stop generator has been issued.");
  stopGeneratorRuning.disable();
}

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
