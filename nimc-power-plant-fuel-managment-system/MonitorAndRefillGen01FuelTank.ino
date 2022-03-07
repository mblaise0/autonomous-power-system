

class processGenerator01FuelLevelAndTakeAction{
  virtual void Generator01FuelLevelAnalysis() = 0;
};


class MonitorAndRefillGen01FuelTank{

  private:
    int Generator01FuelTankLevel;
    int Generator02FuelTankLevel;
    int Generator03FuelTankLevel;

  public:
    void refillGenerator01Tank(void);
    void stopGen01TankFilling(void);    
    void processGenerator01FuelLevelAndTakeAction(void);
   
    MonitorAndRefillGen01FuelTank(int generator01FuelTankLevel){
      Generator01FuelTankLevel = generator01FuelTankLevel;
    }

    void setGenerator01FuelTankLevel(int generator01FuelTank){
      Generator01FuelTankLevel = generator01FuelTank;
    }

    int getGenerator01FuelTankLevel(){
      return Generator01FuelTankLevel;
    }

};

void MonitorAndRefillGen01FuelTank :: processGenerator01FuelLevelAndTakeAction() {
  
  generator_01_fuel_tank.generator01FuelLevelSense = getGenerator01FuelTankLevel();
  if (generator_01_fuel_tank.generator01FuelLevelSense <= MINIMUM_TANK_LEVEL){
     refillGenerator01Tank();
     vTaskDelay(1000);
     generator_01_fuel_tank.stopGen01RefillCount = 0;
//     Serial.println("*******************Gen01-refueling in progress....********************");
  }else if(generator_01_fuel_tank.generator01FuelLevelSense >= MAXIMUM_TANK_LEVEL){
    generator_01_fuel_tank.stopGen01RefillCount++;
    if (generator_01_fuel_tank.stopGen01RefillCount == 1){
      stopGen01TankFilling();
      generator_01_fuel_tank.gen01RefillCount = 0;
      Serial.println("Gen01 fuel level: 100%");
    }
  }
  
}

void MonitorAndRefillGen01FuelTank :: refillGenerator01Tank(void) {
  generator_01_fuel_tank.gen01RefillCount++;
  gen01FuelTankState = FUEL_REFILL_IN_PROGRESS;
  if (generator_01_fuel_tank.gen01RefillCount == 1){
    ActivateSolenoidValve(generatorFuelControlSensor.gen01SenorPin);
    Serial.println("Gen01 SolenoidValve: Activated");
  }
  //activate the flowMeter
  generator_01_fuel_tank.gen01_currentMillis = millis();
  if (generator_01_fuel_tank.gen01_currentMillis - generator_01_fuel_tank.gen01_previousMillis > generator_01_fuel_tank.gen01_flowInterval) {
    
    generator_01_fuel_tank.gen01_pulse1Sec = generator_01_fuel_tank.gen01_pulseCount;
    generator_01_fuel_tank.gen01_pulseCount = 0;
    generator_01_fuel_tank.gen01_flowRate = ((1000.0 / (millis() - generator_01_fuel_tank.gen01_previousMillis)) * generator_01_fuel_tank.gen01_pulse1Sec) / generator_01_fuel_tank.gen01_calibrationFactor;
    generator_01_fuel_tank.gen01_previousMillis = millis();
    generator_01_fuel_tank.gen01_flowMilliLitres = (generator_01_fuel_tank.gen01_flowRate / 60) * 1000;
    generator_01_fuel_tank.gen01_totalMilliLitres += generator_01_fuel_tank.gen01_flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Generator01 Flow rate: ");
    Serial.print(int(generator_01_fuel_tank.gen01_flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Generator01 Output Liquid Quantity: ");
    Serial.print(generator_01_fuel_tank.gen01_totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(generator_01_fuel_tank.gen01_totalMilliLitres / 1000);
    Serial.println("L");
  }
}

void MonitorAndRefillGen01FuelTank :: stopGen01TankFilling() {
  gen01FuelTankState = FUEL_REFILL_STOPPED;
  DeactivateSolenoidValve(generatorFuelControlSensor.gen01SenorPin);
  Serial.println("Gen01 SolenoidValve: Deactivated");
}

void monitorGen01FuelLevel() {
  /*
   * fuel level from the FuelGuage sensor will report fuel level periodically
   * approprate actions will be taken if fuel goes to a particular level
   */
   MonitorAndRefillGen01FuelTank monitorAndRefillGen01FuelTank = MonitorAndRefillGen01FuelTank(0);
   generator_01_fuel_tank.currentFuelLevel = 90; 
   monitorAndRefillGen01FuelTank.setGenerator01FuelTankLevel(generator_01_fuel_tank.currentFuelLevel);
   monitorAndRefillGen01FuelTank.processGenerator01FuelLevelAndTakeAction();
   gen01FuelTankState = FUEL_REFILL_STARTED;
}
