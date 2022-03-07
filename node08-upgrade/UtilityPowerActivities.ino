


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
  
      float redVoltage = (redPhaseVoltage.rmsVal1);
      float blueVoltage = (bluePhaseVoltage.rmsVal1);
      float yellowVoltage = (yellowPhaseVoltage.rmsVal1);

      processUtilityPowerQuality(redVoltage, blueVoltage, yellowVoltage);
//      Serial.print(redVoltage - 74); // [V]
//      Serial.print(", ");
//      Serial.print(blueVoltage - 74); // [V]
//      Serial.print(", ");
//      Serial.println(yellowVoltage - 74); // [V]
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
              client.publish("utilAn", data2, 2); 
              #if DEBUG
              Serial.println(instruction+ "  PhaseSequence: "+ (String)phaseSequenceState+ "  OverAndUnderVoltage: "+ (String)overAndUnderVoltageState+ "  AcbIncomer: "+ (String)powerAvailableAcbIncomer);
              #endif
  //            vTaskDelay(10000);
              StaticJsonDocument<200> doc;
              doc["phaseSequenc"] = phaseSequenceState;
              doc["overNunder"] = overAndUnderVoltageState;
              doc["AcbIn"] = powerAvailableAcbIncomer;
              
              String payload = doc.as<String>();
              payload.toCharArray(data2, (payload.length() + 1));
              client.publish("outTopic", data2, 2); 
              sendUtilityAnalysis = 0;
            }
          }else{
            //utility is good  
            if (sendUtilityAnalysis == sendUtilityAnalysisCount){                  
              String instruction = "UTLPWA1";
              instruction.toCharArray(data2, (instruction.length() + 1));
              client.publish("utilAn", data2, 2); 
  //            vTaskDelay(10000);
              StaticJsonDocument<200> doc;
              doc["phaseSeq"] = phaseSequenceState;
              doc["overNunder"] = overAndUnderVoltageState;
              doc["AcbIn"] = powerAvailableAcbIncomer;
              
              String payload = doc.as<String>();
              payload.toCharArray(data2, (payload.length() + 1));
              client.publish("outTopic", data2, 2); 
              #if DEBUG
              Serial.println(instruction+ "  PhaseSequence: "+ (String)phaseSequenceState+ "  OverAndUnderVoltage: "+ (String)overAndUnderVoltageState+ "  AcbIncomer: "+ (String)powerAvailableAcbIncomer);                    
              #endif
              sendUtilityAnalysis = 0;
            }
          }
    }else{
        if (sendUtilityAnalysis == sendUtilityAnalysisCount){          
          currentState = DISABLE;
          Serial.println("Utility- Power: Unavailable");
          //utility is unavailable
           String instruction = "UTLPWA0";
           instruction.toCharArray(data2, (instruction.length() + 1));
           client.publish("utilAn", data2, 2); 
           #if DEBUG
           Serial.println(instruction+ "  PhaseSequence: "+ (String)phaseSequenceState+ "  OverAndUnderVoltage: "+ (String)overAndUnderVoltageState+ "  AcbIncomer: "+ (String)powerAvailableAcbIncomer);
           #endif
  //         vTaskDelay(10000);
           StaticJsonDocument<200> doc;
           doc["phaseSequenc"] = phaseSequenceState;
           doc["overNunder"] = overAndUnderVoltageState;
           doc["AcbIn"] = powerAvailableAcbIncomer;
          
           String payload = doc.as<String>();
           payload.toCharArray(data2, (payload.length() + 1));
           client.publish("outTopic", data2, 2); 
           sendUtilityAnalysis = 0;
        }
    }
}


//float redPhaseRoutine(){
//  /* 1- AC Voltage Measurement */
//  if(micros() >= redPhaseVoltageLastSample + 1000 )                                                                      
//    {
//      redPhaseVoltageSampleRead = (analogRead(UTILITY_RED_PHASE_VOLTAGE)- 512)+ redPhaseVoltageOffset1;                             
//      redPhaseVoltageSampleSum = redPhaseVoltageSampleSum + sq(redPhaseVoltageSampleRead) ;                                             
//      
//      redPhaseVoltageSampleCount = redPhaseVoltageSampleCount + 1;                                                              
//      redPhaseVoltageLastSample = micros() ;                                                                           
//    }
//  
//  if(redPhaseVoltageSampleCount == 1000)                                                                                
//    {
//      redPhaseVoltageMean = redPhaseVoltageSampleSum/redPhaseVoltageSampleCount;                                                       
//      RMSRedPhaseVoltageMean = (sqrt(redPhaseVoltageMean))*1.5;                                                                 
//      adjustRMSRedPhaseVoltageMean = RMSRedPhaseVoltageMean + redPhaseVoltageOffset2;                                                                                                                                                                                                      /* square root of the average value*/                                                                                                             
//      FinalRedRMSVoltage = RMSRedPhaseVoltageMean + redPhaseVoltageOffset2;                                                      
//      if(FinalRedRMSVoltage <= 2.5)                                                                                
//      {FinalRedRMSVoltage = 0;}
////      Serial.print(" The Voltage RMS value is: ");
////      Serial.print((FinalRedRMSVoltage -  2070),decimalPrecision);
////      Serial.println(" V ");
//      float _redPhase = (FinalRedRMSVoltage -  2070),decimalPrecision;
//      Serial.println(_redPhase);
//
////      threePhaseHysterisisReport(_redPhase);
//      redPhaseVoltageSampleSum =0;                                                                                      
//      redPhaseVoltageSampleCount=0;                                                                                    
//    }
//}
//
//
//float bluePhaseRoutine(){
//  /* 1- AC Voltage Measurement */
//  if(micros() >= bluePhaseVoltageLastSample + 1000 )                                                                      
//    {
//      bluePhaseVoltageSampleRead = (analogRead(UTILITY_BLUE_PHASE_VOLTAGE)- 512)+ bluePhaseVoltageOffset1;                             
//      bluePhaseVoltageSampleSum = bluePhaseVoltageSampleSum + sq(bluePhaseVoltageSampleRead) ;                                             
//      
//      bluePhaseVoltageSampleCount = bluePhaseVoltageSampleCount + 1;                                                              
//      bluePhaseVoltageLastSample = micros() ;                                                                           
//    }
//  
//  if(bluePhaseVoltageSampleCount == 1000)                                                                                
//    {
//      bluePhaseVoltageMean = bluePhaseVoltageSampleSum/bluePhaseVoltageSampleCount;                                                       
//      RMSbluePhaseVoltageMean = (sqrt(bluePhaseVoltageMean))*1.5;                                                                 
//      adjustRMSbluePhaseVoltageMean = RMSbluePhaseVoltageMean + bluePhaseVoltageOffset2;                                                                                                                                                                                                      /* square root of the average value*/                                                                                                             
//      FinalBlueRMSVoltage = RMSbluePhaseVoltageMean + bluePhaseVoltageOffset2;                                                      
//      if(FinalBlueRMSVoltage <= 2.5)                                                                                
//      {FinalBlueRMSVoltage = 0;}
//      float _bluePhase = (FinalBlueRMSVoltage -  2070),decimalPrecision;
////      Serial.print(" The Voltage RMS value is: ");
////      Serial.print((FinalBlueRMSVoltage -  2070),decimalPrecision);
////      Serial.println(" V ");
//      bluePhaseVoltageSampleSum =0;                                                                                      
//      bluePhaseVoltageSampleCount=0;                                                                                    
//    }
//}
//
//
//float yellowPhaseRoutine(){
//  /* 1- AC Voltage Measurement */
//  if(micros() >= yellowPhaseVoltageLastSample + 1000 )                                                                      
//    {
//      yellowPhaseVoltageSampleRead = (analogRead(UTILITY_YELLOW_PHASE_VOLTAGE)- 512)+ yellowPhaseVoltageOffset1;                             
//      yellowPhaseVoltageSampleSum = yellowPhaseVoltageSampleSum + sq(yellowPhaseVoltageSampleRead) ;                                             
//      
//      yellowPhaseVoltageSampleCount = yellowPhaseVoltageSampleCount + 1;                                                              
//      yellowPhaseVoltageLastSample = micros() ;                                                                           
//    }
//  
//  if(yellowPhaseVoltageSampleCount == 1000)                                                                                
//    {
//      yellowPhaseVoltageMean = yellowPhaseVoltageSampleSum/yellowPhaseVoltageSampleCount;                                                       
//      RMSyellowPhaseVoltageMean = (sqrt(yellowPhaseVoltageMean))*1.5;                                                                 
//      adjustRMSyellowPhaseVoltageMean = RMSyellowPhaseVoltageMean + yellowPhaseVoltageOffset2;                                                                                                                                                                                                      /* square root of the average value*/                                                                                                             
//      FinalYellowRMSVoltage = RMSyellowPhaseVoltageMean + yellowPhaseVoltageOffset2;                                                      
//      if(FinalYellowRMSVoltage <= 2.5)                                                                                
//      {FinalYellowRMSVoltage = 0;}
//      float _yellowPhase = (FinalYellowRMSVoltage -  2070),decimalPrecision;
//
////      Serial.print(" The Voltage RMS value is: ");
////      Serial.print((FinalYellowRMSVoltage -  2070),decimalPrecision);
////      Serial.println(" V ");
//      yellowPhaseVoltageSampleSum =0;                                                                                      
//      yellowPhaseVoltageSampleCount=0;                                                                                    
//    }
//}
//
