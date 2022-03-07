
/* NODE10 GENERATOR01 Wireless Verison-1
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

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

#define BOARD_ID "node10"

#define START_OR_STOP_GEN01 2
#define POWER_AVAILABLE_GEN01 5

#define DEBUG 1

const char* ssid = "iomariTPLINK";
const char* password = "#InspireD#";

const char* clientName = "node10";

long lastReconnectAttempt = 0;

const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 45);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient10;
PubSubClient client(espClient10);

Scheduler userScheduler; 

int startGenSignalCount;
int startGenSignal; 

int stopGenSignalCount;
int stopGenSignal; 

int nxtGenSignal;
int nxtGenSignalCount;

void powerAvailabilityGenertor01();

void startGen01();
void stopGen01();

Task powerAvailableGenOne(1500, TASK_FOREVER, &powerAvailabilityGenertor01);
Task startGeneratorOne(1500, TASK_FOREVER, &startGen01);
Task stopGeneratorOne(1500, TASK_FOREVER, &stopGen01);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  
  pinMode(START_OR_STOP_GEN01, OUTPUT);
  pinMode(POWER_AVAILABLE_GEN01, INPUT);

  userScheduler.addTask(powerAvailableGenOne);
  powerAvailableGenOne.enable();
  userScheduler.addTask(startGeneratorOne);
  userScheduler.addTask(stopGeneratorOne);
 
  digitalWrite(START_OR_STOP_GEN01, HIGH);

  startGenSignalCount = 2;
  startGenSignal = 0; 
  
  stopGenSignalCount = 2;
  stopGenSignal = 0; 
  
  nxtGenSignal = 0;
  nxtGenSignalCount = 2;
  
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
      client.publish("outTopic","Node10 connected");
      // ... and resubscribe
      client.subscribe("node10", 1);
      client.subscribe("strtGen", 1);
      client.subscribe("stpGen", 1);
      client.subscribe("catGenOne", 1);
      client.subscribe("emGen01", 1);
      client.subscribe("g01StrtTopic", 1);
      client.subscribe("g01StpTopic", 1);
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

     client.publish("g01StrtTopic", data1, true); 
     client.publish("g01StpTopic", data1, true); 
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
