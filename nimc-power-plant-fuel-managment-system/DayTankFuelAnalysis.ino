
#define MAXIMUM_FUEL_LEVEL 95
#define MINIMUM_FUEL_LEVEL 80
#define AVERAGE_FUEL_LEVEL 60

void ActivateDaySelonoidValve(int sensorPin) {
  digitalWrite(sensorPin, HIGH);
}

void DeactivateDayTankSolenoidValve(int sensorPin) {
  digitalWrite(sensorPin, LOW);
}

class DayTankFuelLevelAnalysis{
  private:
   int DayTankGuageLevel;
   int UndergroundTank01Report;
   int UndergroundTank02Report;

  public:
   void analyiseDayTankFuelLevel();
   void replenishDayTankFuelLevel();
   void stopReplenishingProcess();
   void checkUnderGroundTankLevel();
   
//   DayTankFuelLevelAnalysis(int dayTankGuageLevel) {
//      DayTankGuageLevel = dayTankGuageLevel;
//   }

   void setDayTankFuelGuageLevel(int dayTankGuageLevel) {
      DayTankGuageLevel = dayTankGuageLevel;
   }

   int getDayTankFuelGuageLevel() {
      return DayTankGuageLevel;
   }

   void setUnderGroundTank01Report(int fuelLevel){
    UndergroundTank01Report = fuelLevel;
   }

   int getUnderGroundTank01Report(){
    return UndergroundTank01Report;
   }

   void setUnderGroundTank02Report(int fuelLevel){
    UndergroundTank02Report = fuelLevel;
   }

   int getUnderGroundTank02Report(){
    return UndergroundTank02Report;
   }
};

void DayTankFuelLevelAnalysis :: analyiseDayTankFuelLevel() {
  dayTankFuelAnalysis.readFuelLevel = getDayTankFuelGuageLevel();
  if (dayTankFuelAnalysis.readFuelLevel >= MAXIMUM_FUEL_LEVEL){
    dayTankFuelAnalysis.replenishCount = 0;
    dayTankFuelAnalysis.replenishingStopCount++;
    if (dayTankFuelAnalysis.replenishingStopCount == 1) {
      stopReplenishingProcess();
      #ifdef DEBUG
      Serial.println("Day-Tank Fuel level: 100%");
      #endif
    }
  }else if (dayTankFuelAnalysis.readFuelLevel <= MINIMUM_FUEL_LEVEL) {
    dayTankFuelAnalysis.replenishingStopCount = 0;
    dayTankState = FUEL_REFILL_STARTED;
    replenishDayTankFuelLevel();
  }else if (dayTankFuelAnalysis.readFuelLevel <= AVERAGE_FUEL_LEVEL){
    #ifdef DEBUG
    Serial.println("************************ERROR!! Fuel level Below AVERAGE**************************");
    #endif
    dayTankFuelAnalysis.replenishCount = 0;
    dayTankFuelAnalysis.replenishingStopCount = 0;
  }
}

void DayTankFuelLevelAnalysis :: replenishDayTankFuelLevel() {
  dayTankFuelAnalysis.replenishCount++;
  dayTankTemperature.requestTemperatures();
  dayTankFuelAnalysis.dayTankTemp = dayTankTemperature.getTempCByIndex(0);
  dayTankFuelAnalysis.underGroundTank01report = getUnderGroundTank01Report();
  dayTankFuelAnalysis.underGroundTank02report = getUnderGroundTank02Report();
  Serial.println("Tank01: " +(String)dayTankFuelAnalysis.underGroundTank01report+ " Tank02: "+ (String)dayTankFuelAnalysis.underGroundTank02report);
  
  if((dayTankFuelAnalysis.underGroundTank01report >= 65) && (dayTankFuelAnalysis.underGroundTank02report >= 65)){

    //choose tank01
  }else if((dayTankFuelAnalysis.underGroundTank01report >= 65) && (dayTankFuelAnalysis.underGroundTank02report <= 65)){

    //choose tank01
  }if((dayTankFuelAnalysis.underGroundTank01report <= 65) && (dayTankFuelAnalysis.underGroundTank02report >= 65)){
    
    //choose tank02
  }if((dayTankFuelAnalysis.underGroundTank01report <= 65) && (dayTankFuelAnalysis.underGroundTank02report <= 65)){

    //choose tank01 and report
  }
  
  if (dayTankFuelAnalysis.replenishCount == 1) {
    ActivateDaySelonoidValve(DAY_TANK_SOLENOID_VAVLE);
    #ifdef DEBUG
    Serial.println("Fuel level Below Minimum; Fuel Replenishing Activated.....");
    #endif
  }
  dayTankFuelAnalysis.currentMillis = millis();
  if (dayTankFuelAnalysis.currentMillis - dayTankFuelAnalysis.previousMillis > dayTankFuelAnalysis.flowInterval) {
    
    dayTankFuelAnalysis.pulse1Sec = dayTankFuelAnalysis.pulseCount;
    dayTankFuelAnalysis.pulseCount = 0;
    dayTankFuelAnalysis.flowRate = ((1000.0 / (millis() - dayTankFuelAnalysis.previousMillis)) * dayTankFuelAnalysis.pulse1Sec) / dayTankFuelAnalysis.calibrationFactor;
    dayTankFuelAnalysis.previousMillis = millis();
    dayTankFuelAnalysis.flowMilliLitres = (dayTankFuelAnalysis.flowRate / 60) * 1000;
    dayTankFuelAnalysis.totalMilliLitres += dayTankFuelAnalysis.flowMilliLitres;

    // Print the flow rate for this second in litres / minute
    Serial.print("Day-Tank Flow rate: ");
    Serial.print(int(dayTankFuelAnalysis.flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Day-Tank Output Liquid Quantity: ");
    Serial.print(dayTankFuelAnalysis.totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(dayTankFuelAnalysis.totalMilliLitres / 1000);
    Serial.print("L");
    Serial.println("Temperature is: "+ (String)dayTankFuelAnalysis.dayTankTemp);

  }
}

void DayTankFuelLevelAnalysis :: stopReplenishingProcess() {
  dayTankState = FUEL_TANK_FULL;
  DeactivateDayTankSolenoidValve(DAY_TANK_SOLENOID_VAVLE);
  Serial.println("Day-Tank SolenoidValve: Deactivated");
}

void reportDayTankFuelLevelGuage() {
//  //fuel level reading is being taken here using the modBus sensor
  dayTankFuelAnalysis.fuelLevelSensorReading = 55;
  DayTankFuelLevelAnalysis dayTankFuelLevelAnalysis;
  dayTankFuelLevelAnalysis.setDayTankFuelGuageLevel(dayTankFuelAnalysis.fuelLevelSensorReading);
  dayTankFuelLevelAnalysis.analyiseDayTankFuelLevel();
}
