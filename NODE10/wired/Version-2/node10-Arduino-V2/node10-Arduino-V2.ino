
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <AsyncDelay.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 45);
IPAddress mqttServerIp(192, 168, 2, 44);

const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node10";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

Scheduler userScheduler; 

#define START_OR_STOP_GEN01 2
#define POWER_AVAILABLE_GEN01 4

#define DEBUG 1

long lastReconnectAttempt = 0;

void powerAvailabilityGenertor01();

void startGen01();
void stopGen01();

void restAllLoadActuators();

Task restGenStartStop(1500, TASK_FOREVER, &restAllLoadActuators);
Task powerAvailableGenOne(1500, TASK_FOREVER, &powerAvailabilityGenertor01);
Task startGeneratorOne(1500, TASK_FOREVER, &startGen01);
Task stopGeneratorOne(1500, TASK_FOREVER, &stopGen01);

int startGenSignalCount;
int startGenSignal; 

int stopGenSignalCount;
int stopGenSignal; 

int nxtGenSignal;
int nxtGenSignalCount;

volatile unsigned long start_time_micros;
volatile unsigned long delayTime;

void callback(char* topic, byte* payload, unsigned int length) {
  String inValue;
  String topicStr = topic;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  char payloadString[length+1];
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0';
  inValue = payloadString;
  Serial.println();  
  reciever(inValue, topicStr);
}



boolean reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqttClientName)) {
    //if (mqttClient.connect(mqttClientName, mqttUsername, mqttPassword) { // if credentials is nedded
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","Node10 connected");
      // ... and resubscribe
      mqttClient.subscribe("node10", 1);
      mqttClient.subscribe("strtGen", 1);
      mqttClient.subscribe("stpGen", 1);
      mqttClient.subscribe("catGenOne", 1);
      mqttClient.subscribe("emGen01", 1);
      mqttClient.subscribe("g01StrtTopic", 1);
      mqttClient.subscribe("g01StpTopic", 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  Ethernet.begin(mac, ip);

  pinMode(START_OR_STOP_GEN01, OUTPUT);
  pinMode(POWER_AVAILABLE_GEN01, INPUT);

  userScheduler.addTask(powerAvailableGenOne);
  powerAvailableGenOne.enable();
  userScheduler.addTask(startGeneratorOne);
  userScheduler.addTask(stopGeneratorOne);
  userScheduler.addTask(restGenStartStop);
  
  digitalWrite(START_OR_STOP_GEN01, HIGH);

  startGenSignalCount = 2;
  startGenSignal = 0; 
  
  stopGenSignalCount = 2;
  stopGenSignal = 0; 
  
  nxtGenSignal = 0;
  nxtGenSignalCount = 2;

  lastReconnectAttempt = 0;
}


void loop() {
  if (!mqttClient.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
    reconnect();
  } else {
    mqttClient.loop();
    userScheduler.execute();
  }
}



char reciever(String Value, String inTopic) {
   Serial.println("From callback: " +Value);

  if ((Value == "STRTGEN")){ 
      startGenSignal++;
      if (startGenSignal == startGenSignalCount){
         startGeneratorOne.enable();
         Serial.println("Inside the startGen routine");
      }
  }else if ((Value == "STPGEN")){ 
      stopGenSignal++;
      if (stopGenSignal == stopGenSignalCount){
         stopGeneratorOne.enable();
      }   
  }
}
