

/*
 * Tested for 2 CAT Generator actsas the Node-Controller 
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>
#include <ESP32Ping.h>

#define DEBUG 1

#define BOARD_ID "Node-Controller"

const char* ssid = "iomariTPLink";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 56);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

/*******************************************************
 * IP Address of All nodes in plant
 * 
 */
const char* node11 = "192.168.2.55";
const char* node10 = "192.168.2.45";
const char* node9 = "192.168.2.46";
const char* node8 = "192.168.2.47";
const char* node7 = "192.168.2.48";
const char* node6 = "192.168.2.49";
const char* node5 = "192.168.2.50";
const char* node5V1 = "192.168.2.57";
const char* node4 = "192.168.2.51";
const char* node3 = "192.168.2.52";
const char* node2 = "192.168.2.53";
const char* node1 = "192.168.2.54";
 /*******************************************************/
WiFiClient espClient09;
PubSubClient client(espClient09);

Scheduler userScheduler; // to control your personal task


//Prototypes
//Prototypes
void initializationRoutine();
void instructionToActivateAcbBreaker();
void instructionToEngageUtilityDistribtionContactors();
void instructionToActivateInterlockUtilityBreaker();
void instructionToActivateUtilityToStabilizerBreaker();

void instructionToEngageUtilityOrGenContactor();
void instructionToEngageStabilizerContactor();
void instructionToActivateStabilizerBreakerToDcGrid();
void instructionToEngageOutputBreakerToDcGrid();

void instructionToConnectGenPowerToDistributionAndStabilizer();
void instructionToEngageGen01Contactors();
void instructionToEngageGen02Contactors();
void instructionToEngageGen03Contactors();
void instructionToActivateGenDistributionSideBreaker();
void instructionToEngageGenDistribtionContactors();
void instructionToActivateInterlockGenBreaker();
void instructionToActivateInterlockGenBreaker();

void utilityGoodSignal();
void utilityNotGoodOrUnavailable();
void authorityTochangePowerSourceToUtility();
void authorityTochangePowerSourceToGen();

void instructionToConnectPowerToDistributionAndStabilizer();
void instructionToActivateGenToStabilizerBreaker();

void engage335kvaContactor01Routine();
void engage335kvaContactor02Routine();
void closeOutputBreaker335kvaPanelRoutine();
void engage335ContactorRoutine();

/*************************************/
void startFitGenerator();
void stopGeneratorRunningRoutine();

void startGenerator01Routine();
void startGenerator02Routine();
void startGenerator03Routine();

void startGen335kvaGeneratorRoutine();

void stopGenerator01Routine();
void stopGenerator02Routine();
void stopGenerator03Routine();

void stop335kvaGeneratorRoutine();

//Utility Tasks
Task initialisationTask(3000, TASK_FOREVER, &initializationRoutine);
Task startUtilityGoodProcess(3000, TASK_FOREVER, &utilityGoodSignal);  //remember to  restart the delay at the point where utility failure i.e startUtilityGoodProcess.restartDelayed();
Task startUtilityNotGoodOrUnavailableProcess(3000, TASK_FOREVER, &utilityNotGoodOrUnavailable);
Task startProcessTochangePowerSourceToUtility(3000, TASK_FOREVER,&authorityTochangePowerSourceToUtility);
Task startProcessTochangePowerSourceToGen(3000, TASK_FOREVER,&authorityTochangePowerSourceToGen);

Task activateAcbBreaker(3000, TASK_FOREVER, &instructionToActivateAcbBreaker);
Task engageDistribtionContactors(3000, TASK_FOREVER, &instructionToEngageUtilityDistribtionContactors);
Task activateInterlockUtilityBreaker(3000, TASK_FOREVER, &instructionToActivateInterlockUtilityBreaker);
Task activateUtilityToStabilizerBreaker(3000, TASK_FOREVER, &instructionToActivateUtilityToStabilizerBreaker);

Task engageUtilityOrGenContactor(3000, TASK_FOREVER, &instructionToEngageUtilityOrGenContactor); // this contactor is been used by both Gen and Utility
Task engageStabilizerContactor(3000, TASK_FOREVER, &instructionToEngageStabilizerContactor);
Task engageStabilizerBreakerToDcGrid(3000, TASK_FOREVER, &instructionToActivateStabilizerBreakerToDcGrid);
Task engageOutputBreakerToDcGrid(3000, TASK_FOREVER, &instructionToEngageOutputBreakerToDcGrid);

//Utility Tasks
Task engageGen01Contactors(2000, TASK_FOREVER, &instructionToEngageGen01Contactors);
Task engageGen02Contactors(2000, TASK_FOREVER, &instructionToEngageGen02Contactors);
Task engageGen03Contactors(2000, TASK_FOREVER, &instructionToEngageGen03Contactors);
Task activateGenDistributionSideBreaker(3000, TASK_FOREVER, &instructionToActivateGenDistributionSideBreaker);
Task engageGenDistribtionContactors(3000, TASK_FOREVER, &instructionToEngageGenDistribtionContactors);
Task activateInterlockGenBreaker(3000, TASK_FOREVER, &instructionToActivateInterlockGenBreaker);
Task activateGenToStabilizerBreaker(3000, TASK_FOREVER, &instructionToActivateGenToStabilizerBreaker);
Task connectPowerToDistributionAndStabilizer(4000, TASK_FOREVER, &instructionToConnectPowerToDistributionAndStabilizer);

//Auxillaries report
Task connectGenPowerToDistributionAndStabilizer(3000, TASK_FOREVER, &instructionToConnectGenPowerToDistributionAndStabilizer);

Task startGeneratorFitToRun(1500, TASK_FOREVER, &startFitGenerator);
Task stopGeneratorRuning(1500, TASK_FOREVER, &stopGeneratorRunningRoutine);

Task engage335kva01Contactor(2000, TASK_FOREVER, &engage335kvaContactor01Routine);
Task engage335kva02Contactor(2000, TASK_FOREVER, &engage335kvaContactor02Routine);
Task closeOutputBreaker335kvaPanel(2000, TASK_FOREVER, &closeOutputBreaker335kvaPanelRoutine);
Task engage335Contactor(2000, TASK_FOREVER, &engage335ContactorRoutine);

Task startGen01(2000, TASK_FOREVER, &startGenerator01Routine);
Task startGen02(2000, TASK_FOREVER, &startGenerator02Routine);
Task startGen03(2000, TASK_FOREVER, &startGenerator03Routine);

Task startGen335kvaGenerator(2000, TASK_FOREVER, &startGen335kvaGeneratorRoutine);

Task stopGenerator01(2000, TASK_FOREVER, &stopGenerator01Routine);
Task stopGenerator02(2000, TASK_FOREVER, &stopGenerator02Routine);
Task stopGenerator03(2000, TASK_FOREVER, &stopGenerator03Routine);

Task stop335kvaGenerator(2000, TASK_FOREVER, &stop335kvaGeneratorRoutine);


enum : byte{PLANT_IDLE,
            BRINGING_ON_NEW_POWER_SOURCE = 0,
            PLANT_RUNNING_AND_STABLE = 1,
            POWER_CHANGE_IN_PROGRESS = 2,
            UTILITY_UNAVAILABLE = 3,
            UTILITY_POWER_RESTORED = 4,
            UTILITY_POWER_ACTIVE_ON_GRID = 5,
            POWER_CHANGE_PROCESS_INITIATED = 6,
            POWER_CHANGE_PROCESS_COMPLETED = 7,
            GENERATOR_PWR_SOURCE_ACTIVATED = 8,
            UTILITY_POWER_IN_PROGRESS = 9,
            GENERATOR01_POWER_IN_PROGRESS = 10,
            GENERATOR02_POWER_IN_PROGRESS = 11,
            UTILITY_POWER_RUNNING = 12,
            GENERATOR01_POWER_RUNNING = 13,
            GENERATOR02_POWER_RUNNING = 14
          } powerPlantOperationalState, previousPlantOperationState;

//Flags         
bool utilityGoodFlag = true;
bool utilityBadFlag = true;
bool utilityUnavailableFlag = true;

int utilityGoodFlagCount = 0;
int utilityNotGoodFlagCount = 0;
int initialUtilityGoodFlagCount = 0;
int initialUtilityGoodWaitingInterval = 3;
int utilityGoodWaitingInterval = 20;
int utilityNotGoodWaitingInterval = 5;

int startGeneratorFlagCount = 0;
int startGeneratorWaitingInterval = 1;

int gen01RunningCount = 0;
int gen01RunningCountWaitingInterval = 4;

int gen02RunningCount = 0;
int gen02RunningCountWaitingInterval = 4;

int g01runningCount = 0;
int g02runningCount = 0;
int g03runningCount = 0;

int kvaGenRunningCount = 0;

int stopGeneratorFlagCount = 0;
int stopGeneratorWaitingInterval = 10;

int g01signalCount = 0;
int g02signalCount = 0;

int genStartWaitSignal = 0;
int genStartWaitingInterval = 4;

int selectFitGenerator = 0;
int stopGenRunningCount = 0;

int startGen01AllowedWaitTime = 0;
int startGen02AllowedWaitTime = 0;
int startGen03AllowedWaitTime = 0;

int startGen01Counter = 0;
int startGen02Counter = 0;
int startGen03Counter = 0;
int startKvaGeneratorCounter = 0;

int gen01StopErrorCount = 0;
int gen02StopErrorCount = 0;
int gen03StopErrorCount = 0;

int g01StoppingCount = 0;
int g02StoppingCount = 0;
int g03StoppingCount = 0;

int gen01StpCounter = 0;
int gen02StpCounter = 0;
int gen03StpCounter = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);

  userScheduler.addTask(startUtilityGoodProcess);
  userScheduler.addTask(startProcessTochangePowerSourceToUtility);
  userScheduler.addTask(activateAcbBreaker);
  userScheduler.addTask(activateInterlockUtilityBreaker);
  userScheduler.addTask(engageDistribtionContactors);
  userScheduler.addTask(connectPowerToDistributionAndStabilizer);
  userScheduler.addTask(engageUtilityOrGenContactor);
  userScheduler.addTask(activateUtilityToStabilizerBreaker);
  userScheduler.addTask(engageStabilizerContactor);
  userScheduler.addTask(engageStabilizerBreakerToDcGrid);
  userScheduler.addTask(engageOutputBreakerToDcGrid);
  userScheduler.addTask(startUtilityNotGoodOrUnavailableProcess);
  
  userScheduler.addTask(startProcessTochangePowerSourceToGen);
  userScheduler.addTask(engageGen01Contactors);
  userScheduler.addTask(connectGenPowerToDistributionAndStabilizer);
  userScheduler.addTask(activateGenDistributionSideBreaker);
  userScheduler.addTask(engageGenDistribtionContactors);
  userScheduler.addTask(activateInterlockGenBreaker);
  userScheduler.addTask(activateGenToStabilizerBreaker);
  userScheduler.addTask(initialisationTask);
  userScheduler.addTask(startGeneratorFitToRun);
  userScheduler.addTask(stopGeneratorRuning);

  userScheduler.addTask(engage335kva01Contactor);
  userScheduler.addTask(engage335kva02Contactor);
  userScheduler.addTask(closeOutputBreaker335kvaPanel);
  userScheduler.addTask(engage335Contactor);
  userScheduler.addTask(engageGen02Contactors);

  userScheduler.addTask(startGen01);
  userScheduler.addTask(startGen02);
  userScheduler.addTask(startGen03);
  userScheduler.addTask(startGen335kvaGenerator);

  userScheduler.addTask(stopGenerator01);
  userScheduler.addTask(stopGenerator02);
  userScheduler.addTask(stopGenerator03);
  userScheduler.addTask(stop335kvaGenerator);
  
  lastReconnectAttempt = 0;
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
   String inValue;
   String topicStr = topic;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
//    inValue = (char)payload[i];
  }
  char payloadString[length+1];
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0';
  inValue = payloadString;
  Serial.println();  
  parseCommand(inValue, topicStr);

}

boolean reconnect() {
  if (client.connect(BOARD_ID)) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node-controller connected");
    // ... and resubscribe
    client.subscribe("genTopic", 1);
    client.subscribe("utilIns", 1);
    client.subscribe("utilAn", 1);
    client.subscribe("nodestatus", 1);
    client.subscribe("genReport", 1);
    client.subscribe("fitGenStrtC", 1);
    client.subscribe("stpRGenC", 1);
    client.subscribe("SSG", 1);
//    client.subscribe("plantState", 1);
  }
   else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  return client.connected();
}

void loop() {
  if (!client.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        // Attempt to reconnect
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
  }else {  
      client.loop();
      userScheduler.execute();
  }
}


char parseCommand(String Value, String inTopic) {
   Serial.println("From callback: " +Value);
  if((Value == "UTLPWA1")){
      utilityGoodFlagCount ++;
      initialUtilityGoodFlagCount++;
      stopGeneratorFlagCount++;

      if (initialUtilityGoodFlagCount == initialUtilityGoodFlagCount){  //the plant will begin at this point and never come back to this Task again
        initialisationTask.enable();                                    //So when the plant starts it take 5counts with an interval of 6s to begin but 
      }                                                                 // but subsequent time utility is been restored the count is resetted to a longer waiting time
       #if DEBUG
       Serial.println("utilityGoodFlagCount: "+(String)utilityGoodFlagCount);
       #endif                                                                                            //to ensure that utility is stable or not a fluctuation utility Available signal before Gen power is cut off                                                            
      if (utilityGoodFlagCount == utilityGoodWaitingInterval){
          powerPlantOperationalState = UTILITY_POWER_RESTORED;
          Serial.println("powerPlantOperationalState: "+(String)UTILITY_POWER_RESTORED);
          stopGenRunningCount++;
          utilityNotGoodFlagCount = 0;
          startGeneratorFlagCount = 0;
          #if DEBUG
          Serial.println("the startGenFlagCount: "+ startGeneratorFlagCount);
          #endif
          startUtilityGoodProcess.enable();
          stopGeneratorRuning.enable();
          stopGeneratorRuning.enableDelayed(2000);
      } 
  }else if((Value == "pwAUG1")){
  /*
   * On recieving this signal, means power is available on the utility Grid, 
   * we now engage interlockUtilityBreaker and also send power to Distribution
   */
      powerPlantOperationalState = UTILITY_POWER_IN_PROGRESS;
      activateInterlockUtilityBreaker.enable(); 
  }else if((Value == "utilPw1")){
    /*
     * this signal signifies that the stabiler Breaker has been closed
     * so we close the final breaker which is the Output-BreakerTo-Datacenter
     */
     engageDistribtionContactors.enable();
  }else if((Value == "UTLPWA0")){
    /*
     * Utility Power available but "Not Good"
     * we need to renew the subscription for UtilityAvailable 
     * as well as start process to start to bring on gen power 
     * and engage the Generator01 contactor
     */
     utilityNotGoodFlagCount ++; 
     startGeneratorFlagCount++;
     stopGeneratorFlagCount = 0;
     powerPlantOperationalState = UTILITY_UNAVAILABLE;
     #if DEBUG
     Serial.println("startGeneratorFlagCount when count Before completion: "+ (String)startGeneratorFlagCount);
     #endif
     if(startGeneratorFlagCount == startGeneratorWaitingInterval){
        selectFitGenerator++;
        startUtilityNotGoodOrUnavailableProcess.enable(); 
        startGeneratorFitToRun.enable();
        startGeneratorFitToRun.enableDelayed(3000);
        utilityGoodFlagCount = 16;   //this was formerly 10    
        stopGeneratorFlagCount = 0;
        #if DEBUG
        Serial.println("******************************selectedGeneratorFlagCount: ********************************"+ (String)selectFitGenerator);
        #endif
     }
     if(utilityNotGoodFlagCount == utilityNotGoodWaitingInterval){
       utilityGoodFlagCount = 16;   //this was formerly 10    
       stopGeneratorFlagCount = 0;
     }
   }else if((Value == "UTLPWA10")){
    /*
     * Utility Power available but "Not Good"
     * we need to renew the subscription for UtilityAvailable 
     * as well as start process to start to bring on gen power 
     * and engage the Generator01 contactor
     */
     utilityNotGoodFlagCount ++; 
     startGeneratorFlagCount++;
     stopGeneratorFlagCount = 0;
     powerPlantOperationalState = UTILITY_UNAVAILABLE;
     #if DEBUG
     Serial.println("startGeneratorFlagCount when count Before completion: "+ (String)startGeneratorFlagCount);
     #endif
     if(startGeneratorFlagCount == startGeneratorWaitingInterval){
        selectFitGenerator++;
        startUtilityNotGoodOrUnavailableProcess.enable(); 
        startGeneratorFitToRun.enable();
        startGeneratorFitToRun.enableDelayed(3000);
        utilityGoodFlagCount = 16;   //this was formerly 10    
        stopGeneratorFlagCount = 0;
        #if DEBUG
        Serial.println("******************************selectedGeneratorFlagCount: ********************************"+ (String)selectFitGenerator);
        #endif
     }
     if(utilityNotGoodFlagCount == utilityNotGoodWaitingInterval){
       utilityGoodFlagCount = 16;   //this was formerly 10    
       stopGeneratorFlagCount = 0;
     }
   }else if((Value == "pwrGen1")){
       powerPlantOperationalState = GENERATOR01_POWER_RUNNING;
       #if DEBUG
       Serial.println("************************Generator01 has been turned on, Power available***************************");
       #endif
   }else if((Value == "pwrGen21")){
       powerPlantOperationalState = GENERATOR02_POWER_RUNNING;
       #if DEBUG
       Serial.println("************************Generator02 has been turned on, Power available***************************");
       #endif
   }else if((Value == "gen03Av1")){
      /*
       * Utility Power Unavailable
       * we need to renew the subscription for UtilityAvailable 
       * as well as start process to start to bring on gen power 
       */
         engageGen03Contactors.enable();
   }else if((Value == "ggpw1")){
      /*
       * Power is now available on the Generator Grid 
       * we need to channel this power to Distribution aswell as Stabilizer 
       */
        activateGenDistributionSideBreaker.enable();
        activateGenDistributionSideBreaker.enableDelayed(3000);
        
        activateInterlockGenBreaker.enable();
        activateInterlockGenBreaker.enableDelayed(1000);
  }else if((Value == "genPw1")){
    /*
     * this signal signifies that Node04 is 
     * Ready to change power source
     */
      engageGenDistribtionContactors.enable();
      engageGenDistribtionContactors.enableDelayed(2000);
      #if DEBUG
      Serial.println("Success! 335 Contactor has been Engaged");
      #endif
  }else if((Value == "335PWG011")){
    /*
     * Engage 335kvaGen01 contactor 
     */
     engage335kva01Contactor.enable();
     engage335kva01Contactor.enableDelayed(2000);
  }else if((Value == "335GPW021")){
    /*
     * Engage 335kvaGen02 contactor 
     */
    engage335kva02Contactor.enable();
    engage335kva02Contactor.enableDelayed(2000);
  }else if((Value == "PWAMG1")){
    /*
     * close the outputBreaker from 335panel 
     * Ready to change power source
     */
   closeOutputBreaker335kvaPanel.enable();
   closeOutputBreaker335kvaPanel.enableDelayed(2000);
  }else if((Value == "pw3351")){
    /*
     * close the 335Contactor on mainPanel node03
     */
   engage335Contactor.enable();
   engage335Contactor.enableDelayed(2000);
  }else if((Value == "pwDCG1")){
    /*
     * close the OutputBreakerToDC
     */
    engageOutputBreakerToDcGrid.enable();
    engageOutputBreakerToDcGrid.enableDelayed(2000);
  }else if((Value == "g01-running")){
    /*
     * Gen01 is Running, the node controller goes extra mile as to update the counter
     * 
     */
     char data1[10];
     g01runningCount++;
     if (g01runningCount == 2){
        Serial.println("Generator01 has started");
        startGen01.disable();
        startGen01Counter = 0;
        startGen01AllowedWaitTime = 0;
        engageGen01Contactors.enable();
        engageGen01Contactors.enableDelayed(1500);
        g01runningCount = 0;
     }

  }else if((Value == "g02-running")){
    /*
     * Gen02 is Running
     */
     char data1[10];
     g02runningCount++;
     if (g02runningCount == 2){
        Serial.println("Generator02 has started");
        startGen02.disable();
        startGen02Counter = 0;
        startGen02AllowedWaitTime = 0;
        engageGen02Contactors.enable();
        engageGen02Contactors.enableDelayed(1500);
        g02runningCount = 0;
     }
  }else if((Value == "g03-running")){
    /*
     * Gen03 is Running
     */
     char data1[10];
     g03runningCount++;
     if (g02runningCount == 2){
        Serial.println("Generator03 has started");
        startGen03.disable();
        startGen03Counter = 0;
        startGen03AllowedWaitTime = 0;
        engageGen03Contactors.enable();
        engageGen03Contactors.enableDelayed(1500);
        g02runningCount = 0;
     }
  }else if((Value == "g01-stpd")){
    /*
     * Gen03 has stopped
     */
     char data1[10];
     g01StoppingCount++;
     if (g01StoppingCount == 2){
        Serial.println("Generator01 has stopped");
        stopGenerator01.disable();
        gen01StopErrorCount = 0;
        gen01StpCounter = 0;
        g01StoppingCount = 0;
     }
  }else if((Value == "g02-stpd")){
    /*
     * Gen03 has stopped
     */
     char data1[10];
     g02StoppingCount++;
     if (g02StoppingCount == 2){
        Serial.println("Generator02 has stopped");
        stopGenerator02.disable();
        gen02StopErrorCount = 0;
        gen02StpCounter = 0;
        g02StoppingCount = 0;
     }
  }else if((Value == "g03-stpd")){
    /*
     * Gen03 has stopped
     */
     char data1[10];
     g03StoppingCount++;
     if (g03StoppingCount == 2){
        Serial.println("Generator03 has stopped");
        stopGenerator03.disable();
        gen03StopErrorCount = 0;
        gen03StpCounter = 0;
        g03StoppingCount = 0;
     }
  }else if((Value == "33kva-running")){
    /*
     * 335KVA Generator is Running
     */
     kvaGenRunningCount++;
     if (kvaGenRunningCount == 2){
        Serial.println("335KVAGenerator has started");
        startGen335kvaGenerator.disable();
        startKvaGeneratorCounter = 0;
        kvaGenRunningCount = 0;
     }
  }else if ((inTopic == "fitGenStrtC")){
    selectFitGenerator = Value.toInt(); 
  }else if ((inTopic == "stpRGenC")){
    stopGenRunningCount = Value.toInt(); 
  }else if ((inTopic == "plantState")){
    if (Value == "utility"){
      //utility was running
      userScheduler.deleteTask(initialisationTask);
      startUtilityGoodProcess.abort();
      utilityGoodFlagCount = 25;
    }else if (Value == "Gen01"){
      //gen01 was running
      selectFitGenerator = 1;
      stopGenRunningCount = 0;
    }else if (Value == "Gen02"){
     //gen01 was running
      selectFitGenerator = 0;
      stopGenRunningCount = 1;
    }else if (Value == "utilProg"){
      //utility was still progress
      userScheduler.deleteTask(initialisationTask);
      utilityGoodFlagCount = 25;
      
    }
    utilityGoodFlagCount = Value.toInt(); 
  }
}




void initializationRoutine(){
  if(initialUtilityGoodFlagCount == 4){
      startUtilityGoodProcess.enable();
      utilityGoodFlagCount = 21;
      userScheduler.deleteTask(initialisationTask);
  }
}

void utilityGoodSignal(){
     startProcessTochangePowerSourceToUtility.enable();
     startProcessTochangePowerSourceToUtility.enableDelayed(3000);
     startUtilityGoodProcess.disable();
}


void authorityTochangePowerSourceToUtility(){
  char data2[350];
  String instruction = "RST";
  instruction.toCharArray(data2, (instruction.length() + 1));
  client.publish("rstTopic", data2, 2);

  Serial.println("mandate to change power Source to Utility has been issued");
   /*
    * At this point we delay a bit(like 8s) to 
    * ensure that all Breakers and 
    * Contactors has been resetted
    */
//    activateAcbBreaker.setInterval(10000);
    activateAcbBreaker.enable();
    activateAcbBreaker.enableDelayed(7000);
    startProcessTochangePowerSourceToUtility.disable();
    //delete the startUtilityGoodProcess Task
//    userScheduler.deleteTask(startUtilityGoodProcess);
}


void instructionToConnectPowerToDistributionAndStabilizer(){  //both distribution and interlockUtility are activated
  activateInterlockUtilityBreaker.enable(); 
  activateInterlockUtilityBreaker.enableDelayed(7000);

  engageDistribtionContactors.enable();
  engageDistribtionContactors.enableDelayed(4000);

  connectPowerToDistributionAndStabilizer.disable();
}


/************************************Generator*********************************************/
void utilityNotGoodOrUnavailable(){
   startProcessTochangePowerSourceToGen.enable();
   startProcessTochangePowerSourceToGen.enableDelayed(3000);
   //Give all the Nodes time to reset all load Breadkers before putting power on the Generator Grid, then engage the particular Engage the particular Gen Contactor
   //In this case we will be using Gen01 as test case
   startUtilityNotGoodOrUnavailableProcess.disable();
}

void authorityTochangePowerSourceToGen(){
  char data2[500];
  String instruction = "RST";
  instruction.toCharArray(data2, (instruction.length() + 1));
  client.publish("rstTopic", data2, 2);
  Serial.println("mandate to change power Source To Generator has been issued");

   startProcessTochangePowerSourceToGen.disable();
}

void instructionToConnectGenPowerToDistributionAndStabilizer(){
  /*
   * We activate the routine to enage the distribution side 
   * Breaker and afterwards the Interlock Gen Breaker
   */

   activateGenDistributionSideBreaker.enable();
   activateGenDistributionSideBreaker.enableDelayed(1000);
   // enable the Interlock Gen Breaker as well
   activateInterlockGenBreaker.enable();
   activateInterlockGenBreaker.enableDelayed(2500);
   //finally we disable the instruction signal
   connectGenPowerToDistributionAndStabilizer.disable();
   Serial.println("Inside the genTo Distribution Activities trigger");

}
