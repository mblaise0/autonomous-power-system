
/*
 * Wireless Version 3
 * 
 * Node11 = NIMC-Generator02, this has been tested to accomodate the circle of two CAT Generators, 
 * the Generator Module awaits a signal to start Generator when theres Utility failure, it runs once
 * in a cycle and shuts the Generator down once utility is available and a command from the Node-Controller comes in
 * to shut down the Generator. Speacial cases where it may run more than once in a circle are scenarios when a Generator
 * labelled Fit refuses to start it automatically tries to start the next Fit Generator.
 * 
 * Inspired Technologies Ltd.
 * by Justus C. Diala
 * 
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>


#define BOARD_ID "node11"

#define START_OR_STOP_GEN02 2
#define POWER_AVAILABLE_GEN02 5


#define DEBUG 1

//const char* ssid = "SpectranetInspired";
//const char* password = "inspired";

const char* ssid = "iomariTPLink"; // Wifi SSID
const char* password = "#InspireD#"; // Wifi Password

const char* clientName = "node11";

long lastReconnectAttempt = 0;

const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 55);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient10;
PubSubClient client(espClient10);

Scheduler userScheduler; 

void powerAvailabilityGenertor02();

void startGen02();
void stopGen02();
void batteryPercentage();
Task evaluateBatteryAndFuelAnalysis(2000, TASK_FOREVER, &batteryPercentage);
Task powerAvailableGenTwo(1500, TASK_FOREVER, &powerAvailabilityGenertor02);
Task startGeneratorTwo(1500, TASK_FOREVER, &startGen02);
Task stopGeneratorTwo(1500, TASK_FOREVER, &stopGen02);

int startGenSignalCount;
int startGenSignal; 

int stopGenSignalCount;
int stopGenSignal; 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  
  pinMode(START_OR_STOP_GEN02, OUTPUT);
  pinMode(POWER_AVAILABLE_GEN02, INPUT);

  userScheduler.addTask(powerAvailableGenTwo);
  powerAvailableGenTwo.enable();
  userScheduler.addTask(startGeneratorTwo);
  userScheduler.addTask(stopGeneratorTwo);
  userScheduler.addTask(evaluateBatteryAndFuelAnalysis);
  evaluateBatteryAndFuelAnalysis.enable();
  
  digitalWrite(START_OR_STOP_GEN02, HIGH);
  
  startGenSignalCount = 2;
  startGenSignal = 0; 
  
  stopGenSignalCount = 2;
  stopGenSignal = 0; 
  
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
    //if (client.connect(clientName, mqttUsername, mqttPassword) { // if credentials is nedded
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","Node11 connected");
      // ... and resubscribe
      client.subscribe("node11", 1);
      client.subscribe("strtGen", 1);
      client.subscribe("stpGen", 1);
      client.subscribe("catGenTwo", 1);
      client.subscribe("emGen02", 1);
      client.subscribe("g02StrtTopic", 1);
      client.subscribe("g02StpTopic", 1);
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
