

class processGenerator02FuelLevelAndTakeAction{
  virtual void Generator02FuelLevelAnalysis() = 0;
};

class MonitorAndRefillGen02FuelTank{

  private:
    int Generator02FuelTankLevel;

  public:
    void refillGenerator02Tank(void);
    void stopGen02TankFilling(void);    
    void processGenerator02FuelLevelAndTakeAction(void);
   
    MonitorAndRefillGen02FuelTank(int generator02FuelTankLevel){
      Generator02FuelTankLevel = generator02FuelTankLevel;
    }

    void setGenerator02FuelTankLevel(int generator02FuelTank){
      Generator02FuelTankLevel = generator02FuelTank;
    }

    int getGenerator02FuelTankLevel(){
      return Generator02FuelTankLevel;
    }

};



void MonitorAndRefillGen02FuelTank :: processGenerator02FuelLevelAndTakeAction() {
  
  generator_02_fuel_tank.generator02FuelLevelSense = getGenerator02FuelTankLevel();
  if (generator_02_fuel_tank.generator02FuelLevelSense <= MINIMUM_TANK_LEVEL){
     refillGenerator02Tank();
     vTaskDelay(1000);
     generator_02_fuel_tank.stopGen02RefillCount = 0;
//     Serial.println("*******************Gen02-refueling in progress....********************");
  }else if(generator_02_fuel_tank.generator02FuelLevelSense >= MAXIMUM_TANK_LEVEL){
    generator_02_fuel_tank.stopGen02RefillCount++;
    if (generator_02_fuel_tank.stopGen02RefillCount == 1){
      stopGen02TankFilling();
      generator_02_fuel_tank.gen02RefillCount = 0;
      Serial.println("Gen02 fuel level: 100%");
    }
  }
  
}


void MonitorAndRefillGen02FuelTank :: refillGenerator02Tank(void) {
  generator_02_fuel_tank.gen02RefillCount++;
  gen02FuelTankState = FUEL_REFILL_IN_PROGRESS;
  if (generator_02_fuel_tank.gen02RefillCount == 1){
    ActivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
    Serial.println("Gen02 SolenoidValve: Activated");
  }
  //activate the flowMeter
  generator_02_fuel_tank.gen02_currentMillis = millis();
  if (generator_02_fuel_tank.gen02_currentMillis - generator_02_fuel_tank.gen02_previousMillis > generator_02_fuel_tank.gen02_flowInterval) {
    
    generator_02_fuel_tank.gen02_pulse1Sec = generator_02_fuel_tank.gen02_pulseCount;
    generator_02_fuel_tank.gen02_pulseCount = 0;
    generator_02_fuel_tank.gen02_flowRate = ((1000.0 / (millis() - generator_02_fuel_tank.gen02_previousMillis)) * generator_02_fuel_tank.gen02_pulse1Sec) / generator_02_fuel_tank.gen02_calibrationFactor;
    generator_02_fuel_tank.gen02_previousMillis = millis();
    generator_02_fuel_tank.gen02_flowMilliLitres = (generator_02_fuel_tank.gen02_flowRate / 60) * 1000;
    generator_02_fuel_tank.gen02_totalMilliLitres += generator_02_fuel_tank.gen02_flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Generator02 Flow rate: ");
    Serial.print(int(generator_02_fuel_tank.gen02_flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Generator02 Output Liquid Quantity: ");
    Serial.print(generator_02_fuel_tank.gen02_totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(generator_02_fuel_tank.gen02_totalMilliLitres / 1000);
    Serial.println("L");
  }
}

void MonitorAndRefillGen02FuelTank :: stopGen02TankFilling() {
  gen02FuelTankState = FUEL_REFILL_STOPPED;
  DeactivateSolenoidValve(generatorFuelControlSensor.gen02SenorPin);
  Serial.println("Gen02 SolenoidValve: Deactivated");
}


void monitorGen02FuelLevel() {
  /*
   * fuel level from the FuelGuage sensor will report fuel level periodically
   * approprate actions will be taken if fuel goes to a particular level
   */
   MonitorAndRefillGen02FuelTank monitorAndRefillGen02FuelTank = MonitorAndRefillGen02FuelTank(0);
   generator_02_fuel_tank.currentFuelLevel = 90; 
   monitorAndRefillGen02FuelTank.setGenerator02FuelTankLevel(generator_02_fuel_tank.currentFuelLevel);
   monitorAndRefillGen02FuelTank.processGenerator02FuelLevelAndTakeAction();
   gen02FuelTankState = FUEL_REFILL_STARTED;
}
