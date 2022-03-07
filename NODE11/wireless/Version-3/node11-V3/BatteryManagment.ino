

int batterySense = 0;
int fuelLevl = 0;

 enum FuelAndBatteryState {
  NOT_GOOD = 0,
  GOOD = 1  
 };
 FuelAndBatteryState FNBState;

class AbstractGeneratorSequencing{
    virtual int GeneratorFitToRun() = 0;
};

class GeneratorSequencing:AbstractGeneratorSequencing{
    private:
        String Name;
        int BatteryLevel;
        int FuelLevel;

    public:
        GeneratorSequencing(String name, int batteryLevel, int fuelLevel){
            Name = name;
            BatteryLevel = batteryLevel;
            FuelLevel = fuelLevel;
        }

        void setGeneratorName(String name){
            Name = name;
        }

        String getGeneratorName(){
            return Name;
        }

        void setBatteryLevel(int batteryLevel){
            BatteryLevel = batteryLevel;
        }

        int getBatteryLevel(){
            return BatteryLevel;
        }

        void setFuelLevel(int fuelLevel){
            FuelLevel = fuelLevel;
        }

        int getFuelLevel(){
            return FuelLevel;
        }

        int GeneratorFitToRun(){
            if ((BatteryLevel < 50) || (FuelLevel < 50)){
                  Serial.println(Name + ""+"is not fit to run"+ "" + " BatteryLevel: "+ (String)BatteryLevel +""+ " FuelLevel: "+ (String)FuelLevel);
                  FNBState = NOT_GOOD;
//                std :: cout << Name << "is not fit to run " << "BatteryLevel: " << BatteryLevel << " FuelLevel: " << FuelLevel << std::endl;  
            }else{
                  Serial.println(Name + ""+"is fit to run"+ "" + " BatteryLevel: "+ (String)BatteryLevel +""+ " FuelLevel: "+ (String)FuelLevel);
                  FNBState = GOOD;
//                std :: cout << Name << "is fit to run " << "BatteryLevel: " << BatteryLevel << " FuelLevel: " << FuelLevel << std::endl;  
            }
            return FNBState;
        }
};




void batteryPercentage() {
  batterySense = 51;
  fuelLevl = 55;
  GeneratorSequencing generatorSequencing = GeneratorSequencing("name", 0, 0);
  generatorSequencing.setGeneratorName("CAT01");
  generatorSequencing.setBatteryLevel(batterySense);
  generatorSequencing.setFuelLevel(fuelLevl);
  generatorSequencing.GeneratorFitToRun();
  vTaskDelay(2000);
}
