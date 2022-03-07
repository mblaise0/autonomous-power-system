



class MonitorAndRefill335KVAGenerator01FuelTank{

  private:
    int Generator01_33kva_fuel_tank_level;
    
  public:  
    void refill335KVAGen01Tank(void);
    void stop335KVAGen01TankRefilling(void);     
    void process335KVAGenerator01FuelLevelAndTakeAction(void);
   
    MonitorAndRefill335KVAGenerator01FuelTank(int generator01_33kva_fuel_tank_level){
      Generator01_33kva_fuel_tank_level = generator01_33kva_fuel_tank_level;
    }

    void setGenerator01335KVAFuelTankLevel(int generator01335KVAFuelTank){
      Generator01_33kva_fuel_tank_level = generator01335KVAFuelTank;
    }

    int getGenerator01335KVAFuelTankLevel(){
      return Generator01_33kva_fuel_tank_level;
    }

};



void MonitorAndRefill335KVAGenerator01FuelTank :: process335KVAGenerator01FuelLevelAndTakeAction() {
  
  generator_01_335kva_fuel_tank.generator01335kvaFuelLevelSense = getGenerator01335KVAFuelTankLevel();
  if (generator_01_335kva_fuel_tank.generator01335kvaFuelLevelSense <= MINIMUM_TANK_LEVEL){
     refill335KVAGen01Tank();
     generator_01_335kva_fuel_tank.stop335KVAGen01RefillCount = 0;
  }else if(generator_01_335kva_fuel_tank.generator01335kvaFuelLevelSense >= MAXIMUM_TANK_LEVEL){
    generator_01_335kva_fuel_tank.stop335KVAGen01RefillCount++;
    if (generator_01_335kva_fuel_tank.stop335KVAGen01RefillCount == 1){
      stop335KVAGen01TankRefilling();
      generator_01_335kva_fuel_tank.gen01335KVARefillcount = 0;
      Serial.println("335KVA-Generator01 fuel level: 100%");
    }
  }
  
}


void MonitorAndRefill335KVAGenerator01FuelTank :: refill335KVAGen01Tank(void) {
  generator_01_335kva_fuel_tank.gen01335KVARefillcount++;
  gen01335KVATankState = FUEL_REFILL_IN_PROGRESS;
  if (generator_01_335kva_fuel_tank.gen01335KVARefillcount == 1){
    ActivateSolenoidValve(generatorFuelControlSensor.gen01335KVASensorPin);
    Serial.println("33KVAGen01 SolenoidValve: Activated");
  }
  //activate the flowMeter
  generator_01_335kva_fuel_tank.gen01335kva_currentMillis = millis();
  if (generator_01_335kva_fuel_tank.gen01335kva_currentMillis - generator_01_335kva_fuel_tank.gen01335kva_previousMillis > generator_01_335kva_fuel_tank.gen01335kva_flowInterval) {
    
    generator_01_335kva_fuel_tank.gen01335kva_pulse1Sec = generator_01_335kva_fuel_tank.gen01335kva_pulseCount;
    generator_01_335kva_fuel_tank.gen01335kva_pulseCount = 0;
    generator_01_335kva_fuel_tank.gen01335kva_flowRate = ((1000.0 / (millis() - generator_01_335kva_fuel_tank.gen01335kva_previousMillis)) * generator_01_335kva_fuel_tank.gen01335kva_pulse1Sec) / generator_01_335kva_fuel_tank.gen01335kva_calibrationFactor;
    generator_01_335kva_fuel_tank.gen01335kva_previousMillis = millis();
    generator_01_335kva_fuel_tank.gen01335kva_flowMilliLitres = (generator_01_335kva_fuel_tank.gen01335kva_flowRate / 60) * 1000;
    generator_01_335kva_fuel_tank.gen01335kva_totalMilliLitres += generator_01_335kva_fuel_tank.gen01335kva_flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("33KVAGen01 Flow rate: ");
    Serial.print(int(generator_01_335kva_fuel_tank.gen01335kva_flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("33KVAGen01 Output Liquid Quantity: ");
    Serial.print(generator_01_335kva_fuel_tank.gen01335kva_totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(generator_01_335kva_fuel_tank.gen01335kva_totalMilliLitres / 1000);
    Serial.println("L");
  }
}

void MonitorAndRefill335KVAGenerator01FuelTank :: stop335KVAGen01TankRefilling() {
  gen01335KVATankState = FUEL_REFILL_STOPPED;
  DeactivateSolenoidValve(generatorFuelControlSensor.gen01335KVASensorPin);
  Serial.println("335KVAGen01 SolenoidValve: Deactivated");
}


void monitor335KVAGen01FuelLevel() {
  /*
   * fuel level from the FuelGuage sensor will report fuel level periodically
   * approprate actions will be taken if fuel goes to a particular level
   */
   MonitorAndRefill335KVAGenerator01FuelTank monitorAndRefill335KVAGenerator01FuelTank = MonitorAndRefill335KVAGenerator01FuelTank(0);
   generator_01_335kva_fuel_tank.currentFuelLevel = 92; 
   monitorAndRefill335KVAGenerator01FuelTank.setGenerator01335KVAFuelTankLevel(generator_01_335kva_fuel_tank.currentFuelLevel);
   monitorAndRefill335KVAGenerator01FuelTank.process335KVAGenerator01FuelLevelAndTakeAction();
   gen01335KVATankState = FUEL_REFILL_STARTED;
}

/****************************************************Generator03TankControl************************************************************/


class MonitorAndRefill335KVAGenerator02FuelTank{

  private:
    int Generator02_33kva_fuel_tank_level;
    
  public:
    void refill335KVAGen02Tank(void);
    void stop335KVAGen02TankRefilling(void);   
    void process335KVAGenerator02FuelLevelAndTakeAction(void);
   
    MonitorAndRefill335KVAGenerator02FuelTank(int generator02_33kva_fuel_tank_level){
      Generator02_33kva_fuel_tank_level = generator02_33kva_fuel_tank_level;
    }

    void setGenerator02335KVAFuelTankLevel(int generator02335KVAFuelTank) {
      Generator02_33kva_fuel_tank_level = generator02335KVAFuelTank;
    }

    int getGenerator02335KVAFuelTankLevel(){
      return Generator02_33kva_fuel_tank_level;
    }
};



void MonitorAndRefill335KVAGenerator02FuelTank :: process335KVAGenerator02FuelLevelAndTakeAction() {
  
  generator_02_335kva_fuel_tank.generator02335kvaFuelLevelSense = getGenerator02335KVAFuelTankLevel();
  if (generator_02_335kva_fuel_tank.generator02335kvaFuelLevelSense <= MINIMUM_TANK_LEVEL){
     refill335KVAGen02Tank();
     generator_02_335kva_fuel_tank.stop335KVAGen02RefillCount = 0;
  }else if(generator_02_335kva_fuel_tank.generator02335kvaFuelLevelSense >= MAXIMUM_TANK_LEVEL){
    generator_02_335kva_fuel_tank.stop335KVAGen02RefillCount++;
    if (generator_02_335kva_fuel_tank.stop335KVAGen02RefillCount == 1){
      stop335KVAGen02TankRefilling();
      generator_02_335kva_fuel_tank.gen02335KVARefillcount = 0;
      Serial.println("335KVA-Generator02 fuel level: 100%");
    }
  }
  
}


void MonitorAndRefill335KVAGenerator02FuelTank :: refill335KVAGen02Tank(void) {
  generator_02_335kva_fuel_tank.gen02335KVARefillcount++;
  gen02335KVATankState = FUEL_REFILL_IN_PROGRESS;
  if (generator_02_335kva_fuel_tank.gen02335KVARefillcount == 1){
    ActivateSolenoidValve(generatorFuelControlSensor.gen02335KVASensorPin);
    Serial.println("33KVAGen02 SolenoidValve: Activated");
  }
  //activate the flowMeter
  generator_02_335kva_fuel_tank.gen02335kva_currentMillis = millis();
  if (generator_02_335kva_fuel_tank.gen02335kva_currentMillis - generator_02_335kva_fuel_tank.gen02335kva_previousMillis > generator_02_335kva_fuel_tank.gen02335kva_flowInterval) {
    
    generator_02_335kva_fuel_tank.gen02335kva_pulse1Sec = generator_02_335kva_fuel_tank.gen02335kva_pulseCount;
    generator_02_335kva_fuel_tank.gen02335kva_pulseCount = 0;
    generator_02_335kva_fuel_tank.gen02335kva_flowRate = ((1000.0 / (millis() - generator_02_335kva_fuel_tank.gen02335kva_previousMillis)) * generator_02_335kva_fuel_tank.gen02335kva_pulse1Sec) / generator_02_335kva_fuel_tank.gen02335kva_calibrationFactor;
    generator_02_335kva_fuel_tank.gen02335kva_previousMillis = millis();
    generator_02_335kva_fuel_tank.gen02335kva_flowMilliLitres = (generator_02_335kva_fuel_tank.gen02335kva_flowRate / 60) * 1000;
    generator_02_335kva_fuel_tank.gen02335kva_totalMilliLitres += generator_02_335kva_fuel_tank.gen02335kva_flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("33KVAGen02 Flow rate: ");
    Serial.print(int(generator_02_335kva_fuel_tank.gen02335kva_flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("33KVAGen02 Output Liquid Quantity: ");
    Serial.print(generator_02_335kva_fuel_tank.gen02335kva_totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(generator_02_335kva_fuel_tank.gen02335kva_totalMilliLitres / 1000);
    Serial.println("L");
  }
}


void MonitorAndRefill335KVAGenerator02FuelTank :: stop335KVAGen02TankRefilling() {
  gen02335KVATankState = FUEL_REFILL_STOPPED;
  DeactivateSolenoidValve(generatorFuelControlSensor.gen02335KVASensorPin);
  Serial.println("335KVAGen02 SolenoidValve: Deactivated");
}


void monitor335KVAGen02FuelLevel() {
  /*
   * fuel level from the FuelGuage sensor will report fuel level periodically
   * approprate actions will be taken if fuel goes to a particular level
   */
   MonitorAndRefill335KVAGenerator02FuelTank monitorAndRefill335KVAGenerator02FuelTank = MonitorAndRefill335KVAGenerator02FuelTank(0);
   generator_02_335kva_fuel_tank.currentFuelLevel = 92; 
   monitorAndRefill335KVAGenerator02FuelTank.setGenerator02335KVAFuelTankLevel(generator_02_335kva_fuel_tank.currentFuelLevel);
   monitorAndRefill335KVAGenerator02FuelTank.process335KVAGenerator02FuelLevelAndTakeAction();
   gen02335KVATankState = FUEL_REFILL_STARTED;
}
