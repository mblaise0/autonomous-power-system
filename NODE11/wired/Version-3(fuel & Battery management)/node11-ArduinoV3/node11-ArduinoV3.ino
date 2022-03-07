
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <AsyncDelay.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

//QueueHandle_t queue;
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 55);
IPAddress mqttServerIp(192, 168, 2, 44);

const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node11";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

Scheduler userScheduler; 

#define START_OR_STOP_GEN02 2
#define POWER_AVAILABLE_GEN02 4

#define DEBUG 1


long lastReconnectAttempt = 0;

void powerAvailabilityGenertor02();
void startGen02();
void stopGen02();

Task powerAvailableGenTwo(1500, TASK_FOREVER, &powerAvailabilityGenertor02);
Task startGeneratorTwo(1500, TASK_FOREVER, &startGen02);
Task stopGeneratorTwo(1500, TASK_FOREVER, &stopGen02);

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
      mqttClient.publish("outTopic","Node11 connected");
      // ... and resubscribe
      mqttClient.subscribe("node11", 1);
      mqttClient.subscribe("strtGen", 1);
      mqttClient.subscribe("stpGen", 1);
      mqttClient.subscribe("catGenTwo", 1);
      mqttClient.subscribe("emGen02", 1);
      mqttClient.subscribe("g02StrtTopic", 1);
      mqttClient.subscribe("g02StpTopic", 1);
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

  pinMode(START_OR_STOP_GEN02, OUTPUT);
  pinMode(POWER_AVAILABLE_GEN02, INPUT);

  userScheduler.addTask(powerAvailableGenTwo);
  powerAvailableGenTwo.enable();
  userScheduler.addTask(startGeneratorTwo);
  userScheduler.addTask(stopGeneratorTwo);
  
  digitalWrite(START_OR_STOP_GEN02, HIGH);
  
  startGenSignalCount = 2;
  startGenSignal = 0; 
  
  stopGenSignalCount = 2;
  stopGenSignal = 0; 
  
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
      batteryPercentage(); 
  }
}

char reciever(String Value, String inTopic) {
   Serial.println("From callback: " +Value);

  if ((Value == "STRTGEN")){ 
      startGenSignal++;
     if (startGenSignal == startGenSignalCount){
         startGeneratorTwo.enable();
      }
  }else if ((Value == "STPGEN")){ 
      stopGenSignal++;
      if (stopGenSignal == stopGenSignalCount){
         stopGeneratorTwo.enable();
      }  
  }
}
