




void instructionToEngageGen01Contactors(){
  char data2[750];

  String instruction = "E.GenCN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node2), 1) {
     client.publish("node02", data2, 2);
  }else {
    Serial.println("Error :(");
  }

  Serial.println("Command To Engage GenContactor01 has been issued");
  engageGen01Contactors.disable();
}


void instructionToEngageGen02Contactors(){
  char data2[750];

  String instruction = "E.G2CN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node2), 1) {
     client.publish("node02", data2, 2);
  }else {
    Serial.println("Error :(");
  }

  Serial.println("Command To Engage GenContactor02 has been issued");
  engageGen02Contactors.disable();
}


void instructionToEngageGen03Contactors(){
  char data2[750];

  String instruction = "E.G3CN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node3), 1) {
     client.publish("node03", data2, 2);
  }else {
    Serial.println("Error :(");
  }

  Serial.println("Command To Engage GenContactor03 has been issued");
  engageGen03Contactors.disable();
}


void instructionToActivateGenDistributionSideBreaker(){
  char data2[750];


  String instruction = "C.GDSB";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node1), 1) {
     client.publish("node01", data2, 2);
  }else {
    Serial.println("Error :(");
  }

  /*
   * after closing the Distribution Breaker, 
   * we need to start closing the contactors to the distribution
   * Based on level of pirority(1 = DG BLOCK, 2 = ADMIN_BLOCK)
   */

   Serial.println("Command To ActivateGenDistributionSideBreaker has been issued");
   activateGenDistributionSideBreaker.disable();
}

void instructionToEngageGenDistribtionContactors(){
  char data2[750];

  String instruction = "E.AGENC";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node7), 1) {
     client.publish("node07", data2, 2);
  }else {
    Serial.println("Error :(");
  }

  Serial.println("Command To Engage GenDistribtionContactors has been issued");

 activateGenToStabilizerBreaker.enable();
 activateGenToStabilizerBreaker.enableDelayed(6000);
 engageGenDistribtionContactors.disable();
}

void instructionToActivateInterlockGenBreaker(){
  char data2[750];
  
  String instruction = "E.IGEN";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node1), 1) {
     client.publish("node01", data2, 2);
  }else {
    Serial.println("Error :(");
  }  
  Serial.println("Command To Engage ActivateInterlockGenBreaker has been issued");
  // we have to give a reasonable waiting time to ensure that both utility&GenContactor has been engaged
  //before we activate the GenToStabBreaker
  
  engageUtilityOrGenContactor.enable();
  engageUtilityOrGenContactor.enableDelayed(4000);
  activateInterlockGenBreaker.disable();
}

void instructionToActivateGenToStabilizerBreaker(){
  char data2[750];
  String instruction = "E.GITOSTB";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node4), 1) {
     client.publish("node04", data2, 2);
  }else {
    Serial.println("Error :(");
  }
  Serial.println("Command To Engage ActivateGenToStabilizerBreaker has been issued");
  /*
   * we need to set a chain of activities at this point in other to send power to Datacenter because
   * stabilizer contactor, stabilizerBreakerToDataCenter and outputBreakerToDatacenter are all
   * being shared by noth utility and Generator.
   */
   engageStabilizerContactor.enable();
   engageStabilizerContactor.enableDelayed(5000);
   activateGenToStabilizerBreaker.disable();
}
