//
//#define DEACTIVATE HIGH
//#define ACTIVATE LOW
//
//#define MINIMUM_TANK_LEVEL 60
//#define MAXIMUM_TANK_LEVEL 90
//
//int gen01RefillCount = 0;
//int stopGen01RefillCount = 0;
//
//void ActivateSolenoidValve(int pinNumber){
//  digitalWrite(pinNumber, ACTIVATE);
//}
//
//void DeactivateSolenoidValve(int pinNumber){
//  digitalWrite(pinNumber, DEACTIVATE);
//}
//
//enum GeneratorFuelTankRefillState {INIT_STATE = 0,
//                                   FUEL_REFILL_STARTED = 1,
//                                   FUEL_REFILL_IN_PROGRESS = 2,
//                                   FUEL_REFILL_STOPPED = 3,
//                                   FUEL_REFILL_COMPLETED = 4,
//                                   FUEL_TANK_FULL = 5,
//                                   FUEL_REFILL_NOT_IN_PROGRESS = 6
//                                  } gen01FuelTankState, gen02FuelTankState, gen03FuelTankState, gen01335KVATankState, gen02335KVATankState;
//
//struct SolenoidValvesControl {
//  int gen01SenorPin;
//  int gen02SenorPin;
//  int gen03SenorPin;
//  int gen01335KVASensorPin;
//  int gen02335KVASensorPin;
//};
//
//SolenoidValvesControl generatorFuelControlSensor = {GENERATOR_01_SOLENOID_VALVE, GENERATOR_02_SOLENOID_VALVE, GENERATOR_03_SOLENOID_VALVE, GENERATOR_01_33KVA_SOLENOID_VALVE, GENERATOR_02_33KVA_SOLENOID_VALVE};
//
//class processGenerator01FuelLevelAndTakeAction{
//  virtual void Generator01FuelLevelAnalysis() = 0;
//};
//
//class processGenerator02FuelLevelAndTakeAction{
//  virtual void Generator02FuelLevelAnalysis() = 0;
//};
//
//class processGenerator03FuelLevelAndTakeAction{
//  virtual void Generator03FuelLevelAnalysis() = 0;
//};
//
//
//
//class MonitorAndRefillGen01FuelTank{
//
//  private:
//    int Generator01FuelTankLevel;
//    int Generator02FuelTankLevel;
//    int Generator03FuelTankLevel;
//
//  public:
//    void refillGenerator01Tank(void);
//    void stopGen01TankFilling(void);    
//    void processGenerator01FuelLevelAndTakeAction(void);
//   
//    MonitorAndRefillGen01FuelTank(int generator01FuelTankLevel){
//      Generator01FuelTankLevel = generator01FuelTankLevel;
//    }
//
//    void setGenerator01FuelTankLevel(int generator01FuelTank){
//      Generator01FuelTankLevel = generator01FuelTank;
//    }
//
//    int getGenerator01FuelTankLevel(){
//      return Generator01FuelTankLevel;
//    }
//
//};
//
//void MonitorAndRefillGen01FuelTank :: processGenerator01FuelLevelAndTakeAction() {
//  
//  generator_01_fuel_tank.generator01FuelLevelSense = getGenerator01FuelTankLevel();
//  if (generator_01_fuel_tank.generator01FuelLevelSense <= MINIMUM_TANK_LEVEL){
//     refillGenerator01Tank();
//     vTaskDelay(1000);
//     generator_01_fuel_tank.stopGen01RefillCount = 0;
////     Serial.println("*******************Gen01-refueling in progress....********************");
//  }else if(generator_01_fuel_tank.generator01FuelLevelSense >= MAXIMUM_TANK_LEVEL){
//    generator_01_fuel_tank.stopGen01RefillCount++;
//    if (generator_01_fuel_tank.stopGen01RefillCount == 1){
//      stopGen01TankFilling();
//      generator_01_fuel_tank.gen01RefillCount = 0;
//      Serial.println("Gen01 fuel level: 100%");
//    }
//  }
//  
//}
//
//void MonitorAndRefillGen01FuelTank :: refillGenerator01Tank(void) {
//  generator_01_fuel_tank.gen01RefillCount++;
//  gen01FuelTankState = FUEL_REFILL_IN_PROGRESS;
//  if (generator_01_fuel_tank.gen01RefillCount == 1){
//    ActivateSolenoidValve(generatorFuelControlSensor.gen01SenorPin);
//    Serial.println("Gen01 SolenoidValve: Activated");
//  }
//  //activate the flowMeter
//  generator_01_fuel_tank.gen01_currentMillis = millis();
//  if (generator_01_fuel_tank.gen01_currentMillis - generator_01_fuel_tank.gen01_previousMillis > generator_01_fuel_tank.gen01_flowInterval) {
//    
//    generator_01_fuel_tank.gen01_pulse1Sec = generator_01_fuel_tank.gen01_pulseCount;
//    generator_01_fuel_tank.gen01_pulseCount = 0;
//    generator_01_fuel_tank.gen01_flowRate = ((1000.0 / (millis() - generator_01_fuel_tank.gen01_previousMillis)) * generator_01_fuel_tank.gen01_pulse1Sec) / generator_01_fuel_tank.gen01_calibrationFactor;
//    generator_01_fuel_tank.gen01_previousMillis = millis();
//    generator_01_fuel_tank.gen01_flowMilliLitres = (generator_01_fuel_tank.gen01_flowRate / 60) * 1000;
//    generator_01_fuel_tank.gen01_totalMilliLitres += generator_01_fuel_tank.gen01_flowMilliLitres;
//    
//    // Print the flow rate for this second in litres / minute
//    Serial.print("Flow rate: ");
//    Serial.print(int(generator_01_fuel_tank.gen01_flowRate));  // Print the integer part of the variable
//    Serial.print("L/min");
//    Serial.print("\t");       // Print tab space
//
//    // Print the cumulative total of litres flowed since starting
//    Serial.print("Output Liquid Quantity: ");
//    Serial.print(generator_01_fuel_tank.gen01_totalMilliLitres);
//    Serial.print("mL / ");
//    Serial.print(generator_01_fuel_tank.gen01_totalMilliLitres / 1000);
//    Serial.println("L");
//  }
//}
//
//void MonitorAndRefillGen01FuelTank :: stopGen01TankFilling() {
//  gen01FuelTankState = FUEL_REFILL_STOPPED;
//  DeactivateSolenoidValve(generatorFuelControlSensor.gen01SenorPin);
//  Serial.println("Gen01 SolenoidValve: Deactivated");
//}
//
//void monitorGen01FuelLevel() {
//  /*
//   * fuel level from the FuelGuage sensor will report fuel level periodically
//   * approprate actions will be taken if fuel goes to a particular level
//   */
//   MonitorAndRefillGen01FuelTank monitorAndRefillGen01FuelTank = MonitorAndRefillGen01FuelTank(0);
//   generator_01_fuel_tank.currentFuelLevel = 90; 
//   monitorAndRefillGen01FuelTank.setGenerator01FuelTankLevel(generator_01_fuel_tank.currentFuelLevel);
//   monitorAndRefillGen01FuelTank.processGenerator01FuelLevelAndTakeAction();
//   gen01FuelTankState = FUEL_REFILL_STARTED;
//}
//
//
//
//
///****************************************************Generator02TankControl************************************************************/
//
//class MonitorAndRefillGen02FuelTank{
//
//  private:
//    int Generator02FuelTankLevel;
//
//  public:
//    void refillGenerator02Tank(void);
//    void stopGen02TankFilling(void);    
//    void processGenerator02FuelLevelAndTakeAction(void);
//   
//    MonitorAndRefillGen02FuelTank(int generator02FuelTankLevel){
//      Generator02FuelTankLevel = generator02FuelTankLevel;
//    }
//
//    void setGenerator02FuelTankLevel(int generator02FuelTank){
//      Generator02FuelTankLevel = generator02FuelTank;
//    }
//
//    int getGenerator02FuelTankLevel(){
//      return Generator02FuelTankLevel;
//    }
//
//};
//
//
//
//void MonitorAndRefillGen02FuelTank :: processGenerator02FuelLevelAndTakeAction() {
//  
//  generator_02_fuel_tank.generator02FuelLevelSense = getGenerator02FuelTankLevel();
//  if (generator_02_fuel_tank.generator02FuelLevelSense <= MINIMUM_TANK_LEVEL){
//     refillGenerator02Tank();
//     vTaskDelay(1000);
//     generator_02_fuel_tank.stopGen02RefillCount = 0;
////     Serial.println("*******************Gen02-refueling in progress....********************");
//  }else if(generator_02_fuel_tank.generator02FuelLevelSense >= MAXIMUM_TANK_LEVEL){
//    generator_02_fuel_tank.stopGen02RefillCount++;
//    if (generator_02_fuel_tank.stopGen02RefillCount == 1){
//      stopGen02TankFilling();
//      generator_02_fuel_tank.gen02RefillCount = 0;
//      Serial.println("Gen02 fuel level: 100%");
//    }
//  }
//  
//}
//
//
//void MonitorAndRefillGen02FuelTank :: refillGenerator02Tank(void) {
//  generator_02_fuel_tank.gen02RefillCount++;
//  gen02FuelTankState = FUEL_REFILL_IN_PROGRESS;
//  if (generator_02_fuel_tank.gen02RefillCount == 1){
//    ActivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
//    Serial.println("Gen02 SolenoidValve: Activated");
//  }
//  //activate the flowMeter
//  generator_02_fuel_tank.gen02_currentMillis = millis();
//  if (generator_02_fuel_tank.gen02_currentMillis - generator_02_fuel_tank.gen02_previousMillis > generator_02_fuel_tank.gen02_flowInterval) {
//    
//    generator_02_fuel_tank.gen02_pulse1Sec = generator_02_fuel_tank.gen02_pulseCount;
//    generator_02_fuel_tank.gen02_pulseCount = 0;
//    generator_02_fuel_tank.gen02_flowRate = ((1000.0 / (millis() - generator_02_fuel_tank.gen02_previousMillis)) * generator_02_fuel_tank.gen02_pulse1Sec) / generator_02_fuel_tank.gen02_calibrationFactor;
//    generator_02_fuel_tank.gen02_previousMillis = millis();
//    generator_02_fuel_tank.gen02_flowMilliLitres = (generator_02_fuel_tank.gen02_flowRate / 60) * 1000;
//    generator_02_fuel_tank.gen02_totalMilliLitres += generator_02_fuel_tank.gen02_flowMilliLitres;
//    
//    // Print the flow rate for this second in litres / minute
//    Serial.print("Flow rate: ");
//    Serial.print(int(generator_02_fuel_tank.gen02_flowRate));  // Print the integer part of the variable
//    Serial.print("L/min");
//    Serial.print("\t");       // Print tab space
//
//    // Print the cumulative total of litres flowed since starting
//    Serial.print("Output Liquid Quantity: ");
//    Serial.print(generator_02_fuel_tank.gen02_totalMilliLitres);
//    Serial.print("mL / ");
//    Serial.print(generator_02_fuel_tank.gen02_totalMilliLitres / 1000);
//    Serial.println("L");
//  }
//}
//
//void MonitorAndRefillGen02FuelTank :: stopGen02TankFilling() {
//  gen02FuelTankState = FUEL_REFILL_STOPPED;
//  DeactivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
//  Serial.println("Gen02 SolenoidValve: Deactivated");
//}
//
//
//void monitorGen02FuelLevel() {
//  /*
//   * fuel level from the FuelGuage sensor will report fuel level periodically
//   * approprate actions will be taken if fuel goes to a particular level
//   */
//   MonitorAndRefillGen02FuelTank monitorAndRefillGen02FuelTank = MonitorAndRefillGen02FuelTank(0);
//   generator_02_fuel_tank.currentFuelLevel = 90; 
//   monitorAndRefillGen02FuelTank.setGenerator02FuelTankLevel(generator_02_fuel_tank.currentFuelLevel);
//   monitorAndRefillGen02FuelTank.processGenerator02FuelLevelAndTakeAction();
//   gen02FuelTankState = FUEL_REFILL_STARTED;
//}
//
//
///****************************************************Generator03TankControl************************************************************/
//
//class MonitorAndRefillGen03FuelTank{
//
//  private:
//    int Generator03FuelTankLevel;
//
//  public:
//    void refillGenerator03Tank(void);
//    void stopGen03TankFilling(void);    
//    void processGenerator03FuelLevelAndTakeAction(void);
//   
//    MonitorAndRefillGen03FuelTank(int generator03FuelTankLevel){
//      Generator03FuelTankLevel = generator03FuelTankLevel;
//    }
//
//    void setGenerator03FuelTankLevel(int generator03FuelTank){
//      Generator03FuelTankLevel = generator03FuelTank;
//    }
//
//    int getGenerator03FuelTankLevel(){
//      return Generator03FuelTankLevel;
//    }
//
//};
//
//
//
//void MonitorAndRefillGen03FuelTank :: processGenerator03FuelLevelAndTakeAction() {
//  
//  generator_03_fuel_tank.generator03FuelLevelSense = getGenerator03FuelTankLevel();
//  if (generator_03_fuel_tank.generator03FuelLevelSense <= MINIMUM_TANK_LEVEL){
//     refillGenerator03Tank();
//     vTaskDelay(1000);
//     generator_03_fuel_tank.stopGen03RefillCount = 0;
////     Serial.println("*******************Gen03-refueling in progress....********************");
//  }else if(generator_03_fuel_tank.generator03FuelLevelSense >= MAXIMUM_TANK_LEVEL){
//    generator_03_fuel_tank.stopGen03RefillCount++;
//    if (generator_03_fuel_tank.stopGen03RefillCount == 1){
//      stopGen03TankFilling();
//      generator_03_fuel_tank.gen03RefillCount = 0;
//      Serial.println("Gen03 fuel level: 100%");
//    }
//  }
//  
//}
//
//
//void MonitorAndRefillGen03FuelTank :: refillGenerator03Tank(void) {
//  generator_03_fuel_tank.gen03RefillCount++;
//  gen03FuelTankState = FUEL_REFILL_IN_PROGRESS;
//  if (generator_03_fuel_tank.gen03RefillCount == 1){
//    ActivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);
//    Serial.println("Gen03 SolenoidValve: Activated");
//  }
//  //activate the flowMeter
//  generator_03_fuel_tank.gen03_currentMillis = millis();
//  if (generator_03_fuel_tank.gen03_currentMillis - generator_03_fuel_tank.gen03_previousMillis > generator_03_fuel_tank.gen03_flowInterval) {
//    
//    generator_03_fuel_tank.gen03_pulse1Sec = generator_03_fuel_tank.gen03_pulseCount;
//    generator_03_fuel_tank.gen03_pulseCount = 0;
//    generator_03_fuel_tank.gen03_flowRate = ((1000.0 / (millis() - generator_03_fuel_tank.gen03_previousMillis)) * generator_03_fuel_tank.gen03_pulse1Sec) / generator_03_fuel_tank.gen03_calibrationFactor;
//    generator_03_fuel_tank.gen03_previousMillis = millis();
//    generator_03_fuel_tank.gen03_flowMilliLitres = (generator_03_fuel_tank.gen03_flowRate / 60) * 1000;
//    generator_03_fuel_tank.gen03_totalMilliLitres += generator_03_fuel_tank.gen03_flowMilliLitres;
//    
//    // Print the flow rate for this second in litres / minute
//    Serial.print("Flow rate: ");
//    Serial.print(int(generator_03_fuel_tank.gen03_flowRate));  // Print the integer part of the variable
//    Serial.print("L/min");
//    Serial.print("\t");       // Print tab space
//
//    // Print the cumulative total of litres flowed since starting
//    Serial.print("Output Liquid Quantity: ");
//    Serial.print(generator_03_fuel_tank.gen03_totalMilliLitres);
//    Serial.print("mL / ");
//    Serial.print(generator_03_fuel_tank.gen03_totalMilliLitres / 1000);
//    Serial.println("L");
//  }
//}
//
//void MonitorAndRefillGen03FuelTank :: stopGen03TankFilling() {
//  gen03FuelTankState = FUEL_REFILL_STOPPED;
//  DeactivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);
//  Serial.println("Gen03 SolenoidValve: Deactivated");
//}
//
//
//void monitorGen03FuelLevel() {
//  /*
//   * fuel level from the FuelGuage sensor will report fuel level periodically
//   * approprate actions will be taken if fuel goes to a particular level
//   */
//   MonitorAndRefillGen03FuelTank monitorAndRefillGen03FuelTank = MonitorAndRefillGen03FuelTank(0);
//   generator_03_fuel_tank.currentFuelLevel = 92; 
//   monitorAndRefillGen03FuelTank.setGenerator03FuelTankLevel(generator_03_fuel_tank.currentFuelLevel);
//   monitorAndRefillGen03FuelTank.processGenerator03FuelLevelAndTakeAction();
//   gen03FuelTankState = FUEL_REFILL_STARTED;
//}
//
//
//
//
//
////class MonitorAndRefillGen02FuelTank{
////
////  private:
////    int Generator02FuelTankLevel;
////    
////  public:
////    SolenoidValvesControl generatorFuelControlSensor = {GENERATOR_01_SOLENOID_VALVE, GENERATOR_02_SOLENOID_VALVE, GENERATOR_03_SOLENOID_VALVE};
////    void refillGenerator02Tank(void);
////    void stopGen02TankFilling(void);    
////    void processGenerator02FuelLevelAndTakeAction(void);
////
////    
////    MonitorAndRefillGen02FuelTank(int generator02FuelTankLevel){
////      Generator01FuelTankLevel = generator01FuelTankLevel;
////    }
////
////    void setGenerator02FuelTankLevel(int generator02FuelTank){
////      Generator02FuelTankLevel = generator02FuelTank;
////    }
////
////    int getGenerator02FuelTankLevel(){
////      return Generator02FuelTankLevel;
////    }
////
////};
////class MonitorAndRefillFuelTank{
////
////  private:
////    int Generator01FuelTankLevel;
////    int Generator02FuelTankLevel;
////    int Generator03FuelTankLevel;
////
////  public:
////    SolenoidValvesControl generatorFuelControlSensor = {GENERATOR_01_SOLENOID_VALVE, GENERATOR_02_SOLENOID_VALVE, GENERATOR_03_SOLENOID_VALVE};
////    void refillGenerator01Tank(void);
////    void refillGenerator02Tank(void);
////    void refillGenerator03Tank(void);
////    void stopGen01TankFilling(void);
////    void stopGen02TankFilling(void);
////    void stopGen03TankFilling(void);
////    void processGenerator01FuelLevelAndTakeAction(void);
////    void processGenerator02FuelLevelAndTakeAction(void);
////    void processGenerator03FuelLevelAndTakeAction(void);
////    
////    MonitorAndRefillFuelTank(int generator01FuelTankLevel, int generator02FuelTankLevel, int generator03FuelTankLevel){
////      Generator01FuelTankLevel = generator01FuelTankLevel;
////      Generator02FuelTankLevel = generator02FuelTankLevel;
////      Generator03FuelTankLevel = generator03FuelTankLevel;
////    }
////
////    void setGenerator01FuelTankLevel(int generator01FuelTank){
////      Generator01FuelTankLevel = generator01FuelTank;
////    }
////
////    int getGenerator01FuelTankLevel(){
////      return Generator01FuelTankLevel;
////    }
////
////    void setGenerator02FuelTankLevel(int generator02FuelTank){
////      Generator02FuelTankLevel = generator02FuelTank;
////    }
////
////    int getGenerator02FuelTankLevel(){
////      return Generator02FuelTankLevel;
////    }
////
////    void setGenerator03FuelTankLevel(int generator03FuelTank){
////      Generator03FuelTankLevel = generator03FuelTank;
////    }
////
////    int getGenerator03FuelTankLevel(){
////      return Generator03FuelTankLevel;
////    }
////
////};
//
////void refillGenerator01Tank();
////Task refillGenerator01FuelTank(1500, TASK_FOREVER, &refillGenerator01Tank);
//
////userScheduler.addTask(refillGenerator01FuelTank);
////refillGenerator01FuelTank.enable();
//
//
//
////void MonitorAndRefillFuelTank :: processGenerator02FuelLevelAndTakeAction() {
////  
////}
////
////void MonitorAndRefillFuelTank :: processGenerator03FuelLevelAndTakeAction() {
////  
////}
//
//
//
////void MonitorAndRefillFuelTank :: refillGenerator02Tank(void) {
////  ActivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
////  Serial.println("Gen02 SolenoidValve: Activated");
////}
////
////void MonitorAndRefillFuelTank :: refillGenerator03Tank(void) {
////  ActivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);
////  Serial.println("Gen03 SolenoidValve: Activated");
////}
//
//
//
//
//
////void MonitorAndRefillFuelTank :: stopGen02TankFilling() {
////  DeactivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
////  Serial.println("Gen02 SolenoidValve: Deactivated");
////}
////
////void MonitorAndRefillFuelTank :: stopGen03TankFilling() {
////  DeactivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);  
////  Serial.println("Gen03 SolenoidValve: Deactivated");
////}
//
//
//
//
//
