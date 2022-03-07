

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>


#define BOARD_ID "node11"

#define GENERATOR_01_SOLENOID_VALVE 5      //out
#define GENERATOR_02_SOLENOID_VALVE 4      //out
#define GENERATOR_03_SOLENOID_VALVE 2      //out
#define GENERATOR_01_33KVA_SOLENOID_VALVE 22 //out
#define GENERATOR_02_33KVA_SOLENOID_VALVE 31  //out
#define UNDERGROUND_TANK01_SOLENOID_VALVE 26
#define UNDERGROUND_TANK02_SOLENOID_VAVLE 30
#define DAY_TANK_SOLENOID_VAVLE 32
#define PUMP_01_TO_DAY_TANK 29
#define PUMP_02_TO_DAY_TANK 33
#define INCOMER_PUMP_TO_UNDER_GROUND_TANK 36

#define GENERATOR_01_FLOW_METER 19
#define GENERATOR_02_FLOW_METER 20
#define GENERATOR_03_FLOW_METER 21
#define GENERATOR_01_335KVA_FLOW_METER 25
#define GENERATOR_02_335KVA_FLOW_METER 27
#define UNDERGROUND_TANK01_FLOW_METER 35
#define UNDERGROUND_TANK02_FLOW_METER 35
#define DAY_TANK_FLOW_METER 28
#define DAY_TANK_TEMPERATURE_SENSOR 33
#define UNDERGROUND_TANK_TEMPERATURE_SENSOR 34

#define UNDERGROUND_REPLENISH_SWITCH 18
#define GEN01_FLOW_SWITCH 37
#define GEN02_FLOW_SWITCH 38
#define GEN03_FLOW_SWITCH 39
#define DAY_TANK_FLOW_SWITCH 40
#define UNDERGROUND_FLOW_SWITCH 41

#define DEBUG 1

//const char* ssid = "SpectranetInspired";
//const char* password = "inspired";

const char* ssid = "iomariTPLink"; // Wifi SSID
const char* password = "#InspireD#"; // Wifi Password

const char* clientName = "FuelMSys";

long lastReconnectAttempt = 0;

const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 58);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient10;
PubSubClient client(espClient10);

Scheduler userScheduler;

OneWire dayTankTemp(DAY_TANK_TEMPERATURE_SENSOR);
OneWire underGroundTemp(UNDERGROUND_TANK_TEMPERATURE_SENSOR);

DallasTemperature dayTankTemperature(&dayTankTemp);
DallasTemperature underGroundTankTemperature(&underGroundTemp);

#define DEACTIVATE HIGH
#define ACTIVATE LOW

#define MINIMUM_TANK_LEVEL 60
#define MAXIMUM_TANK_LEVEL 90

int gen01RefillCount = 0;
int stopGen01RefillCount = 0;

void ActivateSolenoidValve(int pinNumber) {
  digitalWrite(pinNumber, ACTIVATE);
}

void DeactivateSolenoidValve(int pinNumber) {
  digitalWrite(pinNumber, DEACTIVATE);
}

enum GeneratorFuelTankRefillState {INIT_STATE = 0,
                                   FUEL_REFILL_STARTED = 1,
                                   FUEL_REFILL_IN_PROGRESS = 2,
                                   FUEL_REFILL_STOPPED = 3,
                                   FUEL_REFILL_COMPLETED = 4,
                                   FUEL_TANK_FULL = 5,
                                   FUEL_REFILL_NOT_IN_PROGRESS = 6
                                  } gen01FuelTankState, gen02FuelTankState, gen03FuelTankState, gen01335KVATankState, gen02335KVATankState, dayTankState;

struct SolenoidValvesControl {
  int gen01SenorPin;
  int gen02SenorPin;
  int gen03SenorPin;
  int gen01335KVASensorPin;
  int gen02335KVASensorPin;
};

SolenoidValvesControl generatorFuelControlSensor = {GENERATOR_01_SOLENOID_VALVE, GENERATOR_02_SOLENOID_VALVE, GENERATOR_03_SOLENOID_VALVE, GENERATOR_01_33KVA_SOLENOID_VALVE, GENERATOR_02_33KVA_SOLENOID_VALVE};

struct GeneratorFuelTank {
  int generator01FuelLevelSense;
  int generator02FuelLevelSense;
  int generator03FuelLevelSense;
  int generator01335kvaFuelLevelSense;
  int generator02335kvaFuelLevelSense;
  int gen01RefillCount;
  int gen02RefillCount;
  int gen03RefillCount;
  int gen01335KVARefillcount;
  int gen02335KVARefillcount;
  int stop335KVAGen01RefillCount;
  int stop335KVAGen02RefillCount;
  int stopGen01RefillCount;
  int stopGen02RefillCount;
  int stopGen03RefillCount;

  long gen01_currentMillis;
  long gen01_previousMillis;
  int gen01_flowInterval;
  float gen01_calibrationFactor;
  volatile byte gen01_pulseCount;
  byte gen01_pulse1Sec;
  float gen01_flowRate;
  unsigned int gen01_flowMilliLitres;
  unsigned long gen01_totalMilliLitres;

  long gen02_currentMillis;
  long gen02_previousMillis;
  int gen02_flowInterval;
  float gen02_calibrationFactor;
  volatile byte gen02_pulseCount;
  byte gen02_pulse1Sec;
  float gen02_flowRate;
  unsigned int gen02_flowMilliLitres;
  unsigned long gen02_totalMilliLitres;

  long gen03_currentMillis;
  long gen03_previousMillis;
  int gen03_flowInterval;
  float gen03_calibrationFactor;
  volatile byte gen03_pulseCount;
  byte gen03_pulse1Sec;
  float gen03_flowRate;
  unsigned int gen03_flowMilliLitres;
  unsigned long gen03_totalMilliLitres;

  long gen01335kva_currentMillis;
  long gen01335kva_previousMillis;
  int gen01335kva_flowInterval;
  float gen01335kva_calibrationFactor;
  volatile byte gen01335kva_pulseCount;
  byte gen01335kva_pulse1Sec;
  float gen01335kva_flowRate;
  unsigned int gen01335kva_flowMilliLitres;
  unsigned long gen01335kva_totalMilliLitres;

  long gen02335kva_currentMillis;
  long gen02335kva_previousMillis;
  int gen02335kva_flowInterval;
  float gen02335kva_calibrationFactor;
  volatile byte gen02335kva_pulseCount;
  byte gen02335kva_pulse1Sec;
  float gen02335kva_flowRate;
  unsigned int gen02335kva_flowMilliLitres;
  unsigned long gen02335kva_totalMilliLitres;

  int genNumber;
  long tankHeight;
  long tankLenght;
  long tankWidth;
  int fuelLevelSensorPin;
  int currentFuelLevel;
  int currentFuelVolume;
  unsigned long lastTimeFilled;
  unsigned long timeRefilledStarted;
  unsigned long timeRefilledStartedCompleted;
} generator_01_fuel_tank, generator_02_fuel_tank, generator_03_fuel_tank, generator_01_335kva_fuel_tank, generator_02_335kva_fuel_tank;


struct DayTankFuelAnalysis{
  int replenishCount;
  int replenishingStopCount;
  int readFuelLevel; 
  int fuelLevelSensorReading; 
  float dayTankTemp;
  long currentMillis;
  long previousMillis;
  float calibrationFactor;
  volatile byte pulseCount;
  byte pulse1Sec;
  float flowRate;
  unsigned int flowMilliLitres;
  unsigned long totalMilliLitres;
  int flowInterval;
  int underGroundTank01report;
  int underGroundTank02report;
}dayTankFuelAnalysis;


struct underGroundFuelTank{
  int tankReplenishmentReport;
  int replenishCount;
  int stopReplenishCount;
  float tank01Temp;
  float tank02Temp;
  long currentMillis;
  long previousMillis;
  float calibrationFactor;
  volatile byte pulseCount;
  byte pulse1Sec;
  float flowRate;
  unsigned int flowMilliLitres;
  unsigned long totalMilliLitres;
  int flowInterval;
  int fuelLevelReport;
  int fuelLevelReading;
}underGroundTank01Analysis,underGroundTank02Analysis;



//long currentMillis = 0;
//long previousMillis = 0;
//int flowInterval = 1000;
//float calibrationFactor = 4.5;
//volatile byte pulseCount;
//byte pulse1Sec = 0;
//float flowRate;
//unsigned int flowMilliLitres;
//unsigned long totalMilliLitres;

//long gen01_currentMillis = 0;
//long gen01_previousMillis = 0;
//int gen01_flowInterval = 1000;
//float gen01_calibrationFactor = 4.5;
//volatile byte gen01_pulseCount;
//byte gen01_pulse1Sec = 0;
//float gen01_flowRate;
//unsigned int gen01_flowMilliLitres;
//unsigned long gen01_totalMilliLitres;
//
//long gen02_currentMillis = 0;
//long gen02_previousMillis = 0;
//int gen02_flowInterval = 1000;
//float gen02_calibrationFactor = 4.5;
//volatile byte gen02_pulseCount;
//byte gen02_pulse1Sec = 0;
//float gen02_flowRate;
//unsigned int gen02_flowMilliLitres;
//unsigned long gen02_totalMilliLitres;
//
//long gen03_currentMillis = 0;
//long gen03_previousMillis = 0;
//int gen03_flowInterval = 1000;
//float gen03_calibrationFactor = 4.5;
//volatile byte gen03_pulseCount;
//byte gen03_pulse1Sec = 0;
//float gen03_flowRate;
//unsigned int gen03_flowMilliLitres;
//unsigned long gen03_totalMilliLitres;

void IRAM_ATTR gen01_pulseCounter()
{
  generator_01_fuel_tank.gen01_pulseCount++;
}

void IRAM_ATTR gen02_pulseCounter()
{
  generator_02_fuel_tank.gen02_pulseCount++;
}

void IRAM_ATTR gen03_pulseCounter()
{
  generator_01_fuel_tank.gen03_pulseCount++;
}

void IRAM_ATTR gen01_335kva_pulseCounter()
{
  generator_01_335kva_fuel_tank.gen01335kva_pulseCount++;
}

void IRAM_ATTR gen02_335kva_pulseCounter()
{
  generator_02_335kva_fuel_tank.gen02335kva_pulseCount++;
}

void IRAM_ATTR dayTankPulseCounter()
{
  dayTankFuelAnalysis.pulseCount++;
}

void IRAM_ATTR undergroundTank01PulseCounter()
{
  underGroundTank01Analysis.pulseCount++;
}


void IRAM_ATTR undergroundTank02PulseCounter()
{
  underGroundTank02Analysis.pulseCount++;
}

//void refillGenerator01Tank();
//Task refillGenerator01FuelTank(1500, TASK_FOREVER, &refillGenerator01Tank);

void monitorGen01FuelLevel();
void monitorGen02FuelLevel();
void monitorGen03FuelLevel();
void monitor335KVAGen01FuelLevel();
void monitor335KVAGen02FuelLevel();
void reportDayTankFuelLevelGuage();
void monitorUndergroundTank01FuelLevel();
void monitorUndergroundTank02FuelLevel();

Task queryGen01FuelLevelAndTakeAction(2000, TASK_FOREVER, monitorGen01FuelLevel);
Task queryGen02FuelLevelAndTakeAction(2000, TASK_FOREVER, monitorGen02FuelLevel);
Task queryGen03FuelLevelAndTakeAction(2000, TASK_FOREVER, monitorGen03FuelLevel);
Task query33KVAGen01FuelLevelAndTakeAction(2000, TASK_FOREVER, monitor335KVAGen01FuelLevel);
Task query33KVAGen02FuelLevelAndTakeAction(2000, TASK_FOREVER, monitor335KVAGen02FuelLevel);
Task monitorDayTankFuelLevelAndTakeAction(100, TASK_FOREVER, reportDayTankFuelLevelGuage);
Task monitorUndergroundTank01FuelLevelAndTakeAction(100, TASK_FOREVER, &monitorUndergroundTank01FuelLevel);
Task monitorUndergroundTank02FuelLevelAndTakeAction(100, TASK_FOREVER, &monitorUndergroundTank02FuelLevel);

void setup() {
  Serial.begin(115200);
  //  setup_wifi();
  //  client.setServer(mqtt_server, 11883);
  //  client.setCallback(callback);
  dayTankTemperature.begin();
  underGroundTankTemperature.begin();
  
  int actuatorSensorPins[] = {GENERATOR_01_SOLENOID_VALVE,  GENERATOR_02_SOLENOID_VALVE, GENERATOR_03_SOLENOID_VALVE, GENERATOR_01_33KVA_SOLENOID_VALVE, GENERATOR_02_33KVA_SOLENOID_VALVE, DAY_TANK_SOLENOID_VAVLE, UNDERGROUND_TANK01_SOLENOID_VALVE, UNDERGROUND_TANK02_SOLENOID_VAVLE, INCOMER_PUMP_TO_UNDER_GROUND_TANK};
  for (int i = 0; i < 11; i++) {
    pinMode(actuatorSensorPins[i], OUTPUT);
  }

  int flowMeterSensorPin[] = {GENERATOR_01_FLOW_METER, GENERATOR_02_FLOW_METER, GENERATOR_03_FLOW_METER, GENERATOR_01_335KVA_FLOW_METER, GENERATOR_02_335KVA_FLOW_METER, UNDERGROUND_REPLENISH_SWITCH, DAY_TANK_FLOW_METER, UNDERGROUND_TANK01_FLOW_METER, UNDERGROUND_TANK02_FLOW_METER};
  for (int i = 0; i < 8; i++) {
    pinMode(flowMeterSensorPin[i], INPUT_PULLUP);
  }

  int flowSwitchSensorPin[] = {UNDERGROUND_REPLENISH_SWITCH, GEN01_FLOW_SWITCH, GEN02_FLOW_SWITCH, GEN03_FLOW_SWITCH, DAY_TANK_FLOW_SWITCH, UNDERGROUND_FLOW_SWITCH};
  for (int i = 0; i < 6; i++) {
    pinMode(flowSwitchSensorPin[i], INPUT);
  }
  attachInterrupt(digitalPinToInterrupt(GENERATOR_01_FLOW_METER), gen01_pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(GENERATOR_02_FLOW_METER), gen02_pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(GENERATOR_03_FLOW_METER), gen03_pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(GENERATOR_01_335KVA_FLOW_METER), gen01_335kva_pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(GENERATOR_02_335KVA_FLOW_METER), gen02_335kva_pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(DAY_TANK_FLOW_METER), dayTankPulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(UNDERGROUND_TANK01_FLOW_METER), undergroundTank01PulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(UNDERGROUND_TANK02_FLOW_METER), undergroundTank02PulseCounter, FALLING);

  generator_01_fuel_tank.gen01_pulseCount = 0;
  generator_02_fuel_tank.gen02_pulseCount = 0;
  generator_03_fuel_tank.gen03_pulseCount = 0;
  generator_01_335kva_fuel_tank.gen01335kva_pulseCount = 0;
  generator_02_335kva_fuel_tank.gen02335kva_pulseCount = 0;

  generator_01_fuel_tank.gen01_flowRate = 0.0;
  generator_02_fuel_tank.gen02_flowRate = 0.0;
  generator_03_fuel_tank.gen03_flowRate = 0.0;
  generator_01_335kva_fuel_tank.gen01335kva_flowRate = 0.0;
  generator_02_335kva_fuel_tank.gen02335kva_flowRate = 0.0;

  generator_01_fuel_tank.gen01_flowMilliLitres = 0;
  generator_02_fuel_tank.gen02_flowMilliLitres = 0;
  generator_03_fuel_tank.gen03_flowMilliLitres = 0;
  generator_01_335kva_fuel_tank.gen01335kva_flowMilliLitres = 0;
  generator_02_335kva_fuel_tank.gen02335kva_flowMilliLitres = 0;

  generator_01_fuel_tank.gen01_totalMilliLitres = 0;
  generator_02_fuel_tank.gen02_totalMilliLitres = 0;
  generator_03_fuel_tank.gen03_totalMilliLitres = 0;
  generator_01_335kva_fuel_tank.gen01335kva_totalMilliLitres = 0;
  generator_02_335kva_fuel_tank.gen02335kva_totalMilliLitres = 0;

  generator_01_fuel_tank.gen01_previousMillis = 0;
  generator_02_fuel_tank.gen02_previousMillis = 0;
  generator_03_fuel_tank.gen03_previousMillis = 0;
  generator_01_335kva_fuel_tank.gen01335kva_previousMillis = 0;
  generator_02_335kva_fuel_tank.gen02335kva_previousMillis = 0;

  generator_01_fuel_tank.gen01_currentMillis = 0;
  generator_01_fuel_tank.gen01_previousMillis = 0;
  generator_01_fuel_tank.gen01_flowInterval = 1000;
  generator_01_fuel_tank.gen01_calibrationFactor = 4.5;

  generator_02_fuel_tank.gen02_currentMillis = 0;
  generator_02_fuel_tank.gen02_previousMillis = 0;
  generator_02_fuel_tank.gen02_flowInterval = 1000;
  generator_02_fuel_tank.gen02_calibrationFactor = 4.5;

  generator_03_fuel_tank.gen03_currentMillis = 0;
  generator_03_fuel_tank.gen03_previousMillis = 0;
  generator_03_fuel_tank.gen03_flowInterval = 1000;
  generator_03_fuel_tank.gen03_calibrationFactor = 4.5;

  generator_01_335kva_fuel_tank.gen01335kva_currentMillis = 0;
  generator_01_335kva_fuel_tank.gen01335kva_previousMillis = 0;
  generator_01_335kva_fuel_tank.gen01335kva_flowInterval = 1000;
  generator_01_335kva_fuel_tank.gen01335kva_calibrationFactor = 4.5;

  generator_02_335kva_fuel_tank.gen02335kva_currentMillis = 0;
  generator_02_335kva_fuel_tank.gen02335kva_previousMillis = 0;
  generator_02_335kva_fuel_tank.gen02335kva_flowInterval = 1000;
  generator_02_335kva_fuel_tank.gen02335kva_calibrationFactor = 4.5;

  generator_01_fuel_tank.generator01FuelLevelSense = 0;
  generator_02_fuel_tank.generator02FuelLevelSense = 0;
  generator_03_fuel_tank.generator03FuelLevelSense = 0;
  generator_01_335kva_fuel_tank.generator01335kvaFuelLevelSense = 0;
  generator_02_335kva_fuel_tank.generator02335kvaFuelLevelSense = 0;

  generator_01_fuel_tank.gen01RefillCount = 0;
  generator_02_fuel_tank.gen02RefillCount = 0;
  generator_03_fuel_tank.gen03RefillCount = 0;
  generator_01_335kva_fuel_tank.gen01335KVARefillcount = 0;
  generator_02_335kva_fuel_tank.gen02335KVARefillcount = 0;

  dayTankFuelAnalysis.replenishCount = 0;
  dayTankFuelAnalysis.replenishingStopCount = 0;
  dayTankFuelAnalysis.currentMillis = 0;
  dayTankFuelAnalysis.previousMillis = 0;
  dayTankFuelAnalysis.flowInterval = 1000;
  dayTankFuelAnalysis.calibrationFactor = 4.5;
  dayTankFuelAnalysis.pulse1Sec = 0;
  dayTankFuelAnalysis.pulseCount = 0;
  dayTankFuelAnalysis.flowRate = 0.0;
  dayTankFuelAnalysis.flowMilliLitres = 0;
  dayTankFuelAnalysis.totalMilliLitres = 0;
  dayTankFuelAnalysis.previousMillis = 0;


  underGroundTank01Analysis.replenishCount = 0;
  underGroundTank01Analysis.stopReplenishCount = 0;
  underGroundTank01Analysis.currentMillis = 0;
  underGroundTank01Analysis.previousMillis = 0;
  underGroundTank01Analysis.flowInterval = 1000;
  underGroundTank01Analysis.calibrationFactor = 4.5;
  underGroundTank01Analysis.pulse1Sec = 0;
  underGroundTank01Analysis.pulseCount = 0;
  underGroundTank01Analysis.flowRate = 0.0;
  underGroundTank01Analysis.flowMilliLitres = 0;
  underGroundTank01Analysis.totalMilliLitres = 0;
  underGroundTank01Analysis.previousMillis = 0;

  underGroundTank02Analysis.replenishCount = 0;
  underGroundTank02Analysis.stopReplenishCount = 0;
  underGroundTank02Analysis.currentMillis = 0;
  underGroundTank02Analysis.previousMillis = 0;
  underGroundTank02Analysis.flowInterval = 1000;
  underGroundTank02Analysis.calibrationFactor = 4.5;
  underGroundTank02Analysis.pulse1Sec = 0;
  underGroundTank02Analysis.pulseCount = 0;
  underGroundTank02Analysis.flowRate = 0.0;
  underGroundTank02Analysis.flowMilliLitres = 0;
  underGroundTank02Analysis.totalMilliLitres = 0;
  underGroundTank02Analysis.previousMillis = 0;

  dayTankFuelAnalysis.underGroundTank01report = 0;
  dayTankFuelAnalysis.underGroundTank02report = 0;
  
  userScheduler.addTask(queryGen01FuelLevelAndTakeAction);
  queryGen01FuelLevelAndTakeAction.enable();

  userScheduler.addTask(queryGen02FuelLevelAndTakeAction);
  queryGen02FuelLevelAndTakeAction.enable();

  userScheduler.addTask(queryGen03FuelLevelAndTakeAction);
  queryGen03FuelLevelAndTakeAction.enable();

  userScheduler.addTask(query33KVAGen01FuelLevelAndTakeAction);
  query33KVAGen01FuelLevelAndTakeAction.enable();

  userScheduler.addTask(query33KVAGen02FuelLevelAndTakeAction);
  query33KVAGen02FuelLevelAndTakeAction.enable();

  userScheduler.addTask(monitorDayTankFuelLevelAndTakeAction);
  monitorDayTankFuelLevelAndTakeAction.enable();
  
  userScheduler.addTask(monitorUndergroundTank01FuelLevelAndTakeAction);
  monitorUndergroundTank01FuelLevelAndTakeAction.enable();

  userScheduler.addTask(monitorUndergroundTank02FuelLevelAndTakeAction);
  monitorUndergroundTank02FuelLevelAndTakeAction.enable();
  
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
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  char payloadString[length + 1];
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0';
  inValue = payloadString;
  Serial.println();
  reciever(inValue, topicStr);
}


boolean reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      //if (client.connect(clientName, mqttUsername, mqttPassword) { // if credentials is nedded
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Node11 connected");
      // ... and resubscribe
      client.subscribe("node11", 1);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  //  if (!client.connected()) {
  //      long now = millis();
  //      if (now - lastReconnectAttempt > 5000) {
  //        lastReconnectAttempt = now;
  //        // Attempt to reconnect
  //        if (reconnect()) {
  //          lastReconnectAttempt = 0;
  //        }
  //      }
  //  }else {
  //      client.loop();
  userScheduler.execute();
  //  }
}

char reciever(String Value, String inTopic) {

}
