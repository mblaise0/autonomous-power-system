


void instructionToActivateAcbBreaker(){
  char data2[750]; 
  String instruction = "C.ACB";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node1), 1) {
     client.publish("node01", data2, 2);
  }else {
    Serial.println("Error :(");
  } 

  Serial.println("Command To Close ACB has been issued");
  activateAcbBreaker.disable();
  
}


void instructionToEngageUtilityDistribtionContactors(){
  char data2[750]; 
  String instruction = "E.AUTLC";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node6), 1) {
     client.publish("node06", data2, 2);
  }else {
    Serial.println("Error :(");
  }  
  /*
   * this signal signifies that the power has gone to the stabilizers
   * so we need to close the utilityToStabilizerBreaker and afterwards close the StabilizerContactor
   */
  Serial.println("Command To Close UtilityToDistributionContactors has been issued");
   activateUtilityToStabilizerBreaker.enable();
   activateUtilityToStabilizerBreaker.enableDelayed(5000);
   engageDistribtionContactors.disable();
}


void instructionToActivateInterlockUtilityBreaker(){
  char data2[750];  
  String instruction = "C.IUTIL";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node1), 1) {
     client.publish("node01", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Close interlockUtilityBreaker has been issued");
  engageUtilityOrGenContactor.enable();
  engageUtilityOrGenContactor.enableDelayed(3000);
  activateInterlockUtilityBreaker.disable(); 

}


void instructionToActivateUtilityToStabilizerBreaker(){
  char data2[750];
  String instruction = "E.UTOSTB";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node4), 1) {
     client.publish("node04", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Engage UtilityToStabilizerBreaker has been issued");
 engageStabilizerContactor.enable();
 engageStabilizerContactor.enableDelayed(3000);
 activateUtilityToStabilizerBreaker.disable();
}


void instructionToEngageUtilityOrGenContactor(){
  char data2[750];
  String instruction = "E.UOGC";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node5), 1) {
     client.publish("node05", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Engage UtilityOrGenContactor has been issued");
  engageUtilityOrGenContactor.disable();

}


void instructionToEngageStabilizerContactor(){
  char data2[750];
  String instruction = "E.STBC";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node4), 1) {
     client.publish("node04", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Engage StabilizerContactor has been issued");
  engageStabilizerBreakerToDcGrid.enable();
  engageStabilizerBreakerToDcGrid.enableDelayed(3000);
  engageStabilizerContactor.disable();
}


void instructionToActivateStabilizerBreakerToDcGrid(){
  char data2[750];
  String instruction = "E.SBTODC";
  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node3), 1) {
     client.publish("node03", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Engage StabilizerBreakerToDcGrid has been issued");
 engageOutputBreakerToDcGrid.enable();
 engageOutputBreakerToDcGrid.enableDelayed(3000);
 engageStabilizerBreakerToDcGrid.disable();
}


void instructionToEngageOutputBreakerToDcGrid(){
  char data2[750];
  String instruction = "E.OBTODC";

  instruction.toCharArray(data2, (instruction.length() + 1));
  if(Ping.ping(node3), 1) {
     client.publish("node03", data2, 2);
  }else {
    Serial.println("Error :(");
  } 
  Serial.println("Command To Engage OutputBreakerToDcGrid has been issued");
  Serial.println("Change over process has been completed Successfully! Power is now available at all points.");

  engageOutputBreakerToDcGrid.disable();

}
