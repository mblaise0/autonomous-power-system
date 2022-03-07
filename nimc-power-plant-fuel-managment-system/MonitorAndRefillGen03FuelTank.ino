

class processGenerator03FuelLevelAndTakeAction{
  virtual void Generator03FuelLevelAnalysis() = 0;
};

class MonitorAndRefillGen03FuelTank{

  private:
    int Generator03FuelTankLevel;

  public:
    void refillGenerator03Tank(void);
    void stopGen03TankFilling(void);    
    void processGenerator03FuelLevelAndTakeAction(void);
   
    MonitorAndRefillGen03FuelTank(int generator03FuelTankLevel){
      Generator03FuelTankLevel = generator03FuelTankLevel;
    }

    void setGenerator03FuelTankLevel(int generator03FuelTank){
      Generator03FuelTankLevel = generator03FuelTank;
    }

    int getGenerator03FuelTankLevel(){
      return Generator03FuelTankLevel;
    }

};



void MonitorAndRefillGen03FuelTank :: processGenerator03FuelLevelAndTakeAction() {
  
  generator_03_fuel_tank.generator03FuelLevelSense = getGenerator03FuelTankLevel();
  if (generator_03_fuel_tank.generator03FuelLevelSense <= MINIMUM_TANK_LEVEL){
     refillGenerator03Tank();
     vTaskDelay(1000);
     generator_03_fuel_tank.stopGen03RefillCount = 0;
//     Serial.println("*******************Gen03-refueling in progress....********************");
  }else if(generator_03_fuel_tank.generator03FuelLevelSense >= MAXIMUM_TANK_LEVEL){
    generator_03_fuel_tank.stopGen03RefillCount++;
    if (generator_03_fuel_tank.stopGen03RefillCount == 1){
      stopGen03TankFilling();
      generator_03_fuel_tank.gen03RefillCount = 0;
      Serial.println("Gen03 fuel level: 100%");
    }
  }
  
}


void MonitorAndRefillGen03FuelTank :: refillGenerator03Tank(void) {
  generator_03_fuel_tank.gen03RefillCount++;
  gen03FuelTankState = FUEL_REFILL_IN_PROGRESS;
  if (generator_03_fuel_tank.gen03RefillCount == 1){
    ActivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);
    Serial.println("Gen03 SolenoidValve: Activated");
  }
  //activate the flowMeter
  generator_03_fuel_tank.gen03_currentMillis = millis();
  if (generator_03_fuel_tank.gen03_currentMillis - generator_03_fuel_tank.gen03_previousMillis > generator_03_fuel_tank.gen03_flowInterval) {
    
    generator_03_fuel_tank.gen03_pulse1Sec = generator_03_fuel_tank.gen03_pulseCount;
    generator_03_fuel_tank.gen03_pulseCount = 0;
    generator_03_fuel_tank.gen03_flowRate = ((1000.0 / (millis() - generator_03_fuel_tank.gen03_previousMillis)) * generator_03_fuel_tank.gen03_pulse1Sec) / generator_03_fuel_tank.gen03_calibrationFactor;
    generator_03_fuel_tank.gen03_previousMillis = millis();
    generator_03_fuel_tank.gen03_flowMilliLitres = (generator_03_fuel_tank.gen03_flowRate / 60) * 1000;
    generator_03_fuel_tank.gen03_totalMilliLitres += generator_03_fuel_tank.gen03_flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Generator03 Flow rate: ");
    Serial.print(int(generator_03_fuel_tank.gen03_flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Generator02 Output Liquid Quantity: ");
    Serial.print(generator_03_fuel_tank.gen03_totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(generator_03_fuel_tank.gen03_totalMilliLitres / 1000);
    Serial.println("L");
  }
}

void MonitorAndRefillGen03FuelTank :: stopGen03TankFilling() {
  gen03FuelTankState = FUEL_REFILL_STOPPED;
  DeactivateSolenoidValve(generatorFuelControlSensor.gen03SenorPin);
  Serial.println("Gen03 SolenoidValve: Deactivated");
}


void monitorGen03FuelLevel() {
  /*
   * fuel level from the FuelGuage sensor will report fuel level periodically
   * approprate actions will be taken if fuel goes to a particular level
   */
   MonitorAndRefillGen03FuelTank monitorAndRefillGen03FuelTank = MonitorAndRefillGen03FuelTank(0);
   generator_03_fuel_tank.currentFuelLevel = 92; 
   monitorAndRefillGen03FuelTank.setGenerator03FuelTankLevel(generator_03_fuel_tank.currentFuelLevel);
   monitorAndRefillGen03FuelTank.processGenerator03FuelLevelAndTakeAction();
   gen03FuelTankState = FUEL_REFILL_STARTED;
}
