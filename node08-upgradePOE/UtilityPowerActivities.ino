


#define ENGAGE_CONTACTOR HIGH
#define DISENGAGE_CONTACTOR LOW

#define ENGAGE_BREAKER LOW
#define DISENGAGE_BREAKER HIGH

#define TRUE HIGH
#define FALSE LOW

#define POWER_AVAILABLE HIGH
#define POWER_UNAVAILABLE LOW


#define MIN_FREQUENCY_RANGE 49
#define MAX_FREQUENCY_RANGE 51

#define MIN_VOLTAGE_RANGE 190.00
#define MAX_VOLTAGE_RANGE 240.00 

#define ACTIVE HIGH

#define WINDOW_SIZE 20

int powerAvailableAcbIncomer = 0;
int previousStateAcbIncomer = 0;

int overAndUnderVoltageState = 0;
int previousOverAndUnderVoltageState = 0;

int phaseSequenceState = 0;
int previousphaseSequenceState = 0;
 
float redPhaseReading = 0;
float bluePhaseReading = 0;
float yellowPhaseReading = 0; 
int frequencyReading = 0;

int redPhaseAcVolt;
int bluePhaseAcVolt;
int yellowPhaseAcVolt;

int rCurr;
int bCurr;
int yCurr;

int RED_PHASE_INDEX = 0;
int BLUE_PHASE_INDEX = 0;
int YELLOW_PHASE_INDEX = 0;

float RED_PAHSE_VALUE = 0;
float BLUE_PAHSE_VALUE = 0;
float YELLOW_PAHSE_VALUE = 0;

int RED_PHASE_SUM = 0;
int BLUE_PHASE_SUM = 0;
int YELLOW_PHASE_SUM = 0;

int RED_PHASE_READINGS[WINDOW_SIZE];
int BLUE_PHASE_READINGS[WINDOW_SIZE];
int YELLOW_PHASE_READINGS[WINDOW_SIZE];

int RED_PHASE_AVERAGED = 0;
int BLUE_PHASE_AVERAGED = 0;
int YELLOW_PHASE_AVERAGED = 0;

int sendUtilityAnalysis = 0;
int sendUtilityAnalysisCount = 10;

void threePhaseHysterisis(){
    redPhaseAcVolt = analogRead(UTILITY_RED_PHASE_VOLTAGE); // read the ADC, channel for Vin
    bluePhaseAcVolt = analogRead(UTILITY_BLUE_PHASE_VOLTAGE); // read the ADC, channel for Vin
    yellowPhaseAcVolt = analogRead(UTILITY_YELLOW_PHASE_VOLTAGE); // read the ADC, channel for Vin
  
    redPhaseVoltage.update(redPhaseAcVolt, rCurr);
    bluePhaseVoltage.update(bluePhaseAcVolt, bCurr);
    yellowPhaseVoltage.update(yellowPhaseAcVolt, yCurr);
  
    //RmsReading.update(adcVal-512);  // without automatic baseline restoration (BLR_OFF), 
                                      // substract a fixed DC offset in ADC-units here.
    cnt++;
    if(cnt >= 500) { // publish every sec
      redPhaseVoltage.publish();
      bluePhaseVoltage.publish();
      yellowPhaseVoltage.publish();
  
      float redVoltage = ((redPhaseVoltage.rmsVal1) - 78);
      float blueVoltage = ((bluePhaseVoltage.rmsVal1) - 78);
      float yellowVoltage = ((yellowPhaseVoltage.rmsVal1) - 78);

      processUtilityPowerQuality(redVoltage, blueVoltage, yellowVoltage);
      cnt=0;
  }

  while(nextLoop > micros());  // wait until the end of the time interval
  nextLoop += LPERIOD;  // set next loop time to current time + LOOP_PERIOD                                                                          
}


void processUtilityPowerQuality(float rPhase,float bPhase, float yPhase){ 
    char data2[1024];
    sendUtilityAnalysis++;
    
    phaseSequenceState = digitalRead(PHASE_SEQUENCE);  
    overAndUnderVoltageState = digitalRead(OVER_AND_UNDER_VOLTAGE);
    powerAvailableAcbIncomer = digitalRead(POWER_AVAILABLE_ACB_INCOMER);
    frequencyReading = 51;
    
    RED_PHASE_SUM = RED_PHASE_SUM - RED_PHASE_READINGS[RED_PHASE_INDEX];
    BLUE_PHASE_SUM = BLUE_PHASE_SUM - BLUE_PHASE_READINGS[BLUE_PHASE_INDEX];
    YELLOW_PHASE_SUM = YELLOW_PHASE_SUM - YELLOW_PHASE_READINGS[YELLOW_PHASE_INDEX];
    
    RED_PAHSE_VALUE = rPhase;
    BLUE_PAHSE_VALUE = bPhase;
    YELLOW_PAHSE_VALUE = yPhase;

    RED_PHASE_READINGS[RED_PHASE_INDEX] = RED_PAHSE_VALUE; 
    BLUE_PHASE_READINGS[BLUE_PHASE_INDEX] = BLUE_PAHSE_VALUE; 
    YELLOW_PHASE_READINGS[YELLOW_PHASE_INDEX] = YELLOW_PAHSE_VALUE; 

    RED_PHASE_SUM = RED_PHASE_SUM + RED_PAHSE_VALUE;  
    BLUE_PHASE_SUM = BLUE_PHASE_SUM + BLUE_PAHSE_VALUE;  
    YELLOW_PHASE_SUM = YELLOW_PHASE_SUM + YELLOW_PAHSE_VALUE;  

    RED_PHASE_INDEX = (RED_PHASE_INDEX+1) % WINDOW_SIZE;
    BLUE_PHASE_INDEX = (BLUE_PHASE_INDEX+1) % WINDOW_SIZE;
    YELLOW_PHASE_INDEX = (YELLOW_PHASE_INDEX+1) % WINDOW_SIZE;

    RED_PHASE_AVERAGED = RED_PHASE_SUM / WINDOW_SIZE;
    BLUE_PHASE_AVERAGED = BLUE_PHASE_SUM / WINDOW_SIZE;
    YELLOW_PHASE_AVERAGED = YELLOW_PHASE_SUM / WINDOW_SIZE;
    
    if (powerAvailableAcbIncomer == ACTIVE) {
       initializationFlag = false;
       currentState = ENABLE;
       if ((RED_PHASE_AVERAGED < MIN_VOLTAGE_RANGE || RED_PHASE_AVERAGED >= MAX_VOLTAGE_RANGE) || (BLUE_PHASE_AVERAGED < MIN_VOLTAGE_RANGE || BLUE_PHASE_AVERAGED >= MAX_VOLTAGE_RANGE) || 
         (YELLOW_PHASE_AVERAGED < MIN_VOLTAGE_RANGE || YELLOW_PHASE_AVERAGED >= MAX_VOLTAGE_RANGE) || (frequencyReading < MIN_FREQUENCY_RANGE || frequencyReading > MAX_FREQUENCY_RANGE) 
          || (overAndUnderVoltageState == FALSE || phaseSequenceState == FALSE)){
            //utility is not good
            if (sendUtilityAnalysis == sendUtilityAnalysisCount){
              String instruction = "UTLPWA10";
              instruction.toCharArray(data2, (instruction.length() + 1));
              mqttClient.publish("utilAn", data2, 2);
              #if DEBUG 
              StaticJsonDocument<200> doc;
              doc["pSeq"] = phaseSequenceState;
              doc["oNU"] = overAndUnderVoltageState;
              doc["AcbIn"] = powerAvailableAcbIncomer;
              doc["r"] = RED_PHASE_AVERAGED;
              doc["b"] = BLUE_PHASE_AVERAGED;
              doc["y"] = YELLOW_PHASE_AVERAGED;
  
              String payload = doc.as<String>();
              payload.toCharArray(data2, (payload.length() + 1));
              mqttClient.publish("outTopic", data2, 2); 
              Serial.println(data2);
              #endif
              sendUtilityAnalysis = 0;
            }
          }else{
            //utility is good
            if (sendUtilityAnalysis == sendUtilityAnalysisCount){          
              String instruction = "UTLPWA1";
              instruction.toCharArray(data2, (instruction.length() + 1));
              mqttClient.publish("utilAn", data2, 2); 
              #if DEBUG 
              StaticJsonDocument<200> doc;
              doc["pSeq"] = phaseSequenceState;
              doc["oNU"] = overAndUnderVoltageState;
              doc["AcbIn"] = powerAvailableAcbIncomer;
              doc["r"] = RED_PHASE_AVERAGED;
              doc["b"] = BLUE_PHASE_AVERAGED;
              doc["y"] = YELLOW_PHASE_AVERAGED;
              
              String payload = doc.as<String>();
              payload.toCharArray(data2, (payload.length() + 1));
              mqttClient.publish("outTopic", data2, 2);
              Serial.println(data2);
              #endif 
              sendUtilityAnalysis = 0;
            }
          }
    }else{
        if (sendUtilityAnalysis == sendUtilityAnalysisCount){          
          //utility is unavailable
           String instruction = "UTLPWA0";
           instruction.toCharArray(data2, (instruction.length() + 1));
           mqttClient.publish("utilAn", data2, 2); 
           #if DEBUG            
           StaticJsonDocument<200> doc;
            doc["pSeq"] = phaseSequenceState;
            doc["oNU"] = overAndUnderVoltageState;
            doc["AcbIn"] = powerAvailableAcbIncomer;
            doc["r"] = RED_PHASE_AVERAGED;
            doc["b"] = BLUE_PHASE_AVERAGED;
            doc["y"] = YELLOW_PHASE_AVERAGED;
          
           String payload = doc.as<String>();
           payload.toCharArray(data2, (payload.length() + 1));
           mqttClient.publish("outTopic", data2, 2); 
           Serial.println(data2);
           #endif            
           sendUtilityAnalysis = 0;
        }
    }
}
