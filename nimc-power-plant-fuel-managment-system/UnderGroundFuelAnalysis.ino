
#define FUEL_FLOW_DICTATED LOW
#define NO_FUEL_FLOW_DICTATED HIGH

#define MAXIMUM_FUEL_LEVEL_UN 95
#define MINIMUM_FUEL_LEVEL_UN 50
#define AVERAGE_FUEL_LEVEL_UN 60

#define ACTIVATE HIGH
#define DEACTIVATE LOW

void ActivateUnderGroundSelonoidValve(int sensorPin) {
   digitalWrite(sensorPin, HIGH);
}

void DeactivateUnderGroundTankSolenoidValve(int sensorPin) {
   digitalWrite(sensorPin, LOW);
}

void activatePump(int pinNumber) {
  digitalWrite(pinNumber, ACTIVATE);
}

void deactivatePump(int pinNumber){
  digitalWrite(pinNumber, DEACTIVATE);
}

class UnderGroundTank01FuelAnalysis : DayTankFuelLevelAnalysis{

  private:
    int UnderGroundTankLevelGauge;

  public:
  void replenishUnderGroundFuelTank(); 
  void stopReplenishmentProcess();
  void processUnderGroundTankFuelLevelAnalysis();
  
//    UnderGroundTank01FuelAnalysis(int underGroundTankLevelGauge){
//      UnderGroundTankLevelGauge = underGroundTankLevelGauge;
//    }

    void setUnderGroundFuelTankLevel(int underGroundTankLevelGauge){
      UnderGroundTankLevelGauge = underGroundTankLevelGauge;
    }

    int getUnderGroundFuelTankLevel() {
      return UnderGroundTankLevelGauge;
    }
};




void UnderGroundTank01FuelAnalysis :: replenishUnderGroundFuelTank() {
  /*
   * a switch is activated once the process starts 
   * informing the system that replenishment is
   * in progress.
   */
   underGroundTank01Analysis.tankReplenishmentReport = digitalRead(UNDERGROUND_REPLENISH_SWITCH);
   if  (underGroundTank01Analysis.tankReplenishmentReport == FUEL_FLOW_DICTATED) {
        underGroundTank01Analysis.replenishCount++;
        underGroundTank01Analysis.stopReplenishCount = 0;
//        process tank temperature 
        underGroundTankTemperature.requestTemperatures();
        underGroundTank01Analysis.tank01Temp = underGroundTankTemperature.getTempCByIndex(0);
        if (underGroundTank01Analysis.replenishCount == 1) {
          ActivateUnderGroundSelonoidValve(UNDERGROUND_TANK01_SOLENOID_VALVE);
          activatePump(INCOMER_PUMP_TO_UNDER_GROUND_TANK);
          #ifdef DEBUG
          Serial.println("Fuel level LOW; UnderGroundTankFuel Replenishing Activated.....");
          #endif
        }
        underGroundTank01Analysis.currentMillis = millis();
        if (underGroundTank01Analysis.currentMillis - underGroundTank01Analysis.previousMillis > underGroundTank01Analysis.flowInterval) {
          
          underGroundTank01Analysis.pulse1Sec = underGroundTank01Analysis.pulseCount;
          underGroundTank01Analysis.pulseCount = 0;
          underGroundTank01Analysis.flowRate = ((1000.0 / (millis() - underGroundTank01Analysis.previousMillis)) * underGroundTank01Analysis.pulse1Sec) / underGroundTank01Analysis.calibrationFactor;
          underGroundTank01Analysis.previousMillis = millis();
          underGroundTank01Analysis.flowMilliLitres = (underGroundTank01Analysis.flowRate / 60) * 1000;
          underGroundTank01Analysis.totalMilliLitres += underGroundTank01Analysis.flowMilliLitres;
      
          // Print the flow rate for this second in litres / minute
          Serial.print("Underground-Tank Flow rate: ");
          Serial.print(int(underGroundTank01Analysis.flowRate));  // Print the integer part of the variable
          Serial.print("L/min");
          Serial.print("\t");       // Print tab space
      
          // Print the cumulative total of litres flowed since starting
          Serial.print("Underground-Tank Output Liquid Quantity: ");
          Serial.print(underGroundTank01Analysis.totalMilliLitres);
          Serial.print("mL / ");
          Serial.print(underGroundTank01Analysis.totalMilliLitres / 1000);
          Serial.print("L ");
          Serial.println(" Temperature is: "+ (String)underGroundTank01Analysis.tank01Temp+ " Celcius");
        }
   }else{
    underGroundTank01Analysis.replenishCount = 0;
    underGroundTank01Analysis.stopReplenishCount = 0; 
   }
}


void UnderGroundTank01FuelAnalysis :: stopReplenishmentProcess() {
  DeactivateUnderGroundTankSolenoidValve(UNDERGROUND_TANK01_SOLENOID_VALVE);
}

void UnderGroundTank01FuelAnalysis :: processUnderGroundTankFuelLevelAnalysis() {
  underGroundTank01Analysis.stopReplenishCount++;
  underGroundTank01Analysis.fuelLevelReport = getUnderGroundFuelTankLevel();
  if (underGroundTank01Analysis.fuelLevelReport >= MAXIMUM_FUEL_LEVEL_UN) {
     if (underGroundTank01Analysis.stopReplenishCount == 1){
      stopReplenishmentProcess();
      deactivatePump(INCOMER_PUMP_TO_UNDER_GROUND_TANK);
      Serial.println("Underground Tank01 Fuel Level has attained maximum level; replenishment Stopped...");
      delay(3000);
    }
  }else if (underGroundTank01Analysis.fuelLevelReport <= AVERAGE_FUEL_LEVEL_UN){
    //report fuel level to warn fuel level has gone down
    replenishUnderGroundFuelTank();
    Serial.println("Underground Tank01 Fuel level has reached its average; prepare to replenish Tank...");
    delay(3000);
  }else if (underGroundTank01Analysis.fuelLevelReport == MINIMUM_FUEL_LEVEL_UN) {
    replenishUnderGroundFuelTank();
    Serial.println("Error!!! Underground Tank01 Fuel level has gone below average, refill immediately...");
    delay(3000);
  }
}


void monitorUndergroundTank01FuelLevel() {
  UnderGroundTank01FuelAnalysis underGroundTank01FuelAnalysis;
  DayTankFuelLevelAnalysis dayTankFuelLevelAnalysis;
  underGroundTank01Analysis.fuelLevelReading = 96;
  dayTankFuelLevelAnalysis.setUnderGroundTank02Report(underGroundTank01Analysis.fuelLevelReading);
  dayTankFuelLevelAnalysis.setUnderGroundTank01Report(underGroundTank01Analysis.fuelLevelReading);
  underGroundTank01FuelAnalysis.setUnderGroundFuelTankLevel(underGroundTank01Analysis.fuelLevelReading);
  underGroundTank01FuelAnalysis.processUnderGroundTankFuelLevelAnalysis();
}


/********************************************Under-ground Tank02 *********************************************************************/

class UnderGroundTank02FuelAnalysis{

  private:
    int UnderGroundTankLevelGauge;

  public:
  void replenishUnderGroundFuelTank(); 
  void stopReplenishmentProcess();
  void processUnderGroundTankFuelLevelAnalysis();
  
//    UnderGroundTank02FuelAnalysis(int underGroundTankLevelGauge){
//      UnderGroundTankLevelGauge = underGroundTankLevelGauge;
//    }

    void setUnderGroundFuelTankLevel(int underGroundTankLevelGauge){
      UnderGroundTankLevelGauge = underGroundTankLevelGauge;
    }

    int getUnderGroundFuelTankLevel() {
      return UnderGroundTankLevelGauge;
    }
};

void UnderGroundTank02FuelAnalysis :: replenishUnderGroundFuelTank() {
  /*
   * a switch is activated once the process starts 
   * informing the system that replenishment is
   * in progress.
   */
   underGroundTank02Analysis.tankReplenishmentReport = digitalRead(UNDERGROUND_REPLENISH_SWITCH);
   if  (underGroundTank02Analysis.tankReplenishmentReport == FUEL_FLOW_DICTATED) {
        underGroundTank02Analysis.replenishCount++;
        underGroundTank02Analysis.stopReplenishCount = 0;
//        process tank temperature 
        underGroundTankTemperature.requestTemperatures();
        underGroundTank02Analysis.tank01Temp = underGroundTankTemperature.getTempCByIndex(0);
        if (underGroundTank02Analysis.replenishCount == 1) {
          ActivateUnderGroundSelonoidValve(UNDERGROUND_TANK01_SOLENOID_VALVE);
          activatePump(INCOMER_PUMP_TO_UNDER_GROUND_TANK);
          #ifdef DEBUG
          Serial.println("Fuel level LOW; UnderGroundTankFuel Replenishing Activated.....");
          #endif
        }
        underGroundTank02Analysis.currentMillis = millis();
        if (underGroundTank02Analysis.currentMillis - underGroundTank02Analysis.previousMillis > underGroundTank02Analysis.flowInterval) {
          
          underGroundTank02Analysis.pulse1Sec = underGroundTank02Analysis.pulseCount;
          underGroundTank02Analysis.pulseCount = 0;
          underGroundTank02Analysis.flowRate = ((1000.0 / (millis() - underGroundTank02Analysis.previousMillis)) * underGroundTank02Analysis.pulse1Sec) / underGroundTank02Analysis.calibrationFactor;
          underGroundTank02Analysis.previousMillis = millis();
          underGroundTank02Analysis.flowMilliLitres = (underGroundTank02Analysis.flowRate / 60) * 1000;
          underGroundTank02Analysis.totalMilliLitres += underGroundTank02Analysis.flowMilliLitres;
      
          // Print the flow rate for this second in litres / minute
          Serial.print("Underground-Tan01 Flow rate: ");
          Serial.print(int(underGroundTank02Analysis.flowRate));  // Print the integer part of the variable
          Serial.print("L/min");
          Serial.print("\t");       // Print tab space
      
          // Print the cumulative total of litres flowed since starting
          Serial.print("Underground-Tank01 Output Liquid Quantity: ");
          Serial.print(underGroundTank02Analysis.totalMilliLitres);
          Serial.print("mL / ");
          Serial.print(underGroundTank02Analysis.totalMilliLitres / 1000);
          Serial.print("L ");
          Serial.println(" Temperature- Tank01 is: "+ (String)underGroundTank02Analysis.tank01Temp+ " Celcius");
        }
   }else{
    underGroundTank02Analysis.replenishCount = 0;
    underGroundTank02Analysis.stopReplenishCount = 0; 
   }
}


void UnderGroundTank02FuelAnalysis :: stopReplenishmentProcess() {
  DeactivateUnderGroundTankSolenoidValve(UNDERGROUND_TANK01_SOLENOID_VALVE);
}

void UnderGroundTank02FuelAnalysis :: processUnderGroundTankFuelLevelAnalysis() {
  underGroundTank02Analysis.stopReplenishCount++;
  underGroundTank02Analysis.fuelLevelReport = getUnderGroundFuelTankLevel();
  if (underGroundTank02Analysis.fuelLevelReport >= MAXIMUM_FUEL_LEVEL_UN) {
     if (underGroundTank02Analysis.stopReplenishCount == 1){
      stopReplenishmentProcess();
      deactivatePump(INCOMER_PUMP_TO_UNDER_GROUND_TANK);
      Serial.println("Underground Tank02 Fuel Level has attained maximum level; replenishment Stopped...");
      delay(3000);
    }
  }else if (underGroundTank02Analysis.fuelLevelReport <= AVERAGE_FUEL_LEVEL_UN){
    //report fuel level to warn fuel level has gone down
    replenishUnderGroundFuelTank();
    Serial.println("Underground Tank02 Fuel level has reached its average; prepare to replenish Tank...");
    delay(3000);
  }else if (underGroundTank02Analysis.fuelLevelReport == MINIMUM_FUEL_LEVEL_UN) {
    replenishUnderGroundFuelTank();
    Serial.println("Error!!! Underground Tank02 Fuel level has gone below average, refill immediately...");
    delay(3000);
  }
}


void monitorUndergroundTank02FuelLevel() {

  UnderGroundTank02FuelAnalysis underGroundTank02FuelAnalysis;
  underGroundTank02Analysis.fuelLevelReading = 98;
  underGroundTank02FuelAnalysis.setUnderGroundFuelTankLevel(underGroundTank02Analysis.fuelLevelReading);
  underGroundTank02FuelAnalysis.processUnderGroundTankFuelLevelAnalysis();
}
