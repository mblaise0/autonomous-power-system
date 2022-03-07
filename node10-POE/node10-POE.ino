
/*
 * Node10 = NIMC-Generator01, this has been tested to accomodate the circle of two CAT Generators, 
 * the Generator Module awaits a signal to start Generator when theres Utility failure, it runs once
 * in a cycle and shuts the Generator down once utility is available and a command from the Node-Controller comes in
 * to shut down the Generator. Speacial cases where it may run more than once in a circle are scenarios when a Generator
 * labelled Fit refuses to start it automatically tries to start the next Fit Generator.
 * 
 * Inspired Technologies Ltd.
 * by Justus C. Diala
 * 
 */
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>


#define START_OR_STOP_GEN01 2
#define POWER_AVAILABLE_GEN01 4

#define DEBUG 1

long lastReconnectAttempt = 0;

// CHANGE THESE SETTINGS FOR YOUR APPLICATION
const char* mqttServerIp = "192.168.2.44"; // IP address of the MQTT broker
const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node10";
const char* mqttUsername = NULL;
const char* mqttPassword = NULL;

IPAddress local_ip(192, 168, 2, 45);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(192, 168, 1, 2);

Scheduler userScheduler; 

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

// Initializations of network clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);

static bool eth_connected = false;
uint64_t chipid;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

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
  WiFi.setSleep(false);
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  
  pinMode(START_OR_STOP_GEN01, OUTPUT);
  pinMode(POWER_AVAILABLE_GEN01, INPUT);

  userScheduler.addTask(powerAvailableGenOne);
  powerAvailableGenOne.enable();
  userScheduler.addTask(startGeneratorOne);
  userScheduler.addTask(stopGeneratorOne);
  userScheduler.addTask(restGenStartStop);
  
  digitalWrite(START_OR_STOP_GEN01, HIGH);
  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  ETH.config(local_ip, gateway, subnet, dns);
   startGenSignalCount = 2;
   startGenSignal = 0; 
  
   stopGenSignalCount = 2;
   stopGenSignal = 0; 
  
   nxtGenSignal = 0;
   nxtGenSignalCount = 2;

  lastReconnectAttempt = 0;
}

void loop()
{
  // check if ethernet is connected
  if (eth_connected) {
    // now take care of MQTT client...
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
  }else if ((Value == "genOneNxt")){
    nxtGenSignal++;
    char data1[10];
    if (nxtGenSignal == nxtGenSignalCount){
     startGenSignal = 0;   //at this point the all renew their signals to go for another round again
     stopGenSignal = 0;
     //update the start and stop signal count to the broker
     String signalCount = (String)startGenSignal;
     String stopSignalCount = (String)stopGenSignal;

     signalCount.toCharArray(data1, (signalCount.length() + 1));
     stopSignalCount.toCharArray(data1, (stopSignalCount.length() + 1));

     mqttClient.publish("g01StrtTopic", data1, true); 
     mqttClient.publish("g01StpTopic", data1, true); 
     Serial.println("Gen01 is next to run...");
    }
  }else if ((inTopic == "g01StrtTopic")){
    Serial.println("value at this topic: "+Value);
    startGenSignal = Value.toInt();
  }else if ((inTopic == "g01StpTopic")){
    Serial.println("value at this topic: "+Value);
    stopGenSignal = Value.toInt();
  }
}
