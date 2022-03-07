
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
  selectAnotherFitGenerator.enable();    //monitor to be sure that a generator comes on else crank another fit generator. If a generator comes on at within the crank time this is diabled
  selectAnotherFitGenerator.enableDelayed(10000);
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

int reportFailedGenStartSignal = 0;
int reportFailedGenStopSignal = 0;

void reportGeneratorFailedStart() {
  char data2[750];
  /*
   * this routine checks that the generator module instructed to start actually recieves the signal
   * if not it automacally increment the counter for that Generator and goes to the next Generator 
   * update the counter of the failed Generator
   */
//  reportFailedGenStartSignal++;
//  if (reportFailedGenStartSignal == 3){
    String instruction = "STRTGEN";
    instruction.toCharArray(data2, (instruction.length() + 1)); 
    for (int i = 0; i < 3; i++){
       client.publish("strtGen", data2, 2);
       vTaskDelay(300);
    } 
//  }
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



void updateGen01Counter() {
  
  client.publish("g01StrtTopic", "3", true);
  client.publish("g01StpTopic", "0", true); 
  client.publish("g02StrtTopic", "3", true);
  client.publish("g02StpTopic", "0", true);
//  updateGenerator01Counter.disable();
}

void updateGen02Counter() {
  
  client.publish("g02StrtTopic", "6", true);
  client.publish("g02StpTopic", "3", true);
  client.publish("g01StrtTopic", "3", true);
  client.publish("g01StpTopic", "3", true); 
//  updateGenerator02Counter.disable();
}
