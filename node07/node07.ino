
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

#define DEBUG 1

//ACTUATORS
#define GEN_CONTACTOR_01_COIL 2
#define GEN_CONTACTOR_02_COIL 5
#define GEN_CONTACTOR_03_COIL 23
#define GEN_CONTACTOR_04_COIL 21
#define GEN_CONTACTOR_05_COIL 15

//INTERRUPTS
#define GEN_CONTACTOR_01_AUX 4
#define GEN_CONTACTOR_02_AUX 18
#define GEN_CONTACTOR_03_AUX 19
#define GEN_CONTACTOR_04_AUX 22
#define GEN_CONTACTOR_05_AUX 13
#define GEN_POWER_AVAILABLE 12
#define DISTRIBUTION_04_POWER_AVAILABLE 14
#define DISTRIBUTION_05_POWER_AVAILABLE 27


enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 
State currentState;

const char* ssid = "iomariTPLink";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 48);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient7;
PubSubClient client(espClient7);

Scheduler userScheduler; // to control your personal task


//prototype
  void genContactor01AuxRoutine();
  void genContactor02AuxRoutine();
  void genContactor03AuxRoutine();
  void genContactor04AuxRoutine();
  void genContactor05AuxRoutine();
  void genPowerAvailableRoutine();
  void distribution04PowerAvailableRoutine();
  void distribution05PowerAvailableRoutine();

  void enableGenContactor01CoilRoutine();
  void disableGenContactor01CoilRoutine();
  void enableGenContactor02CoilRoutine();
  void disableGenContactor02CoilRoutine();
  void enableGenContactor03CoilRoutine();
  void disableGenContactor03CoilRoutine();
  void enableGenContactor04CoilRoutine();
  void disableGenContactor04CoilRoutine();
  void enableGenContactor05CoilRoutine();
  void disableGenContactor05CoilRoutine();
  void disengageAllGenContactorsRoutine();
  void engageAllGenContactorsRoutine();

  Task enableGenContactor01Coil(1500, TASK_FOREVER, &enableGenContactor01CoilRoutine);
  Task disableGenContactor01Coil(1500, TASK_FOREVER, &disableGenContactor01CoilRoutine);
  Task enableGenContactor02Coil(1500, TASK_FOREVER, &enableGenContactor02CoilRoutine);
  Task disableGenContactor02Coil(1500, TASK_FOREVER, &disableGenContactor02CoilRoutine);
  Task enableGenContactor03Coil(1500, TASK_FOREVER, &enableGenContactor03CoilRoutine);
  Task disableGenContactor03Coil(1500, TASK_FOREVER, &disableGenContactor03CoilRoutine);
  Task enableGenContactor04Coil(1500, TASK_FOREVER, &enableGenContactor04CoilRoutine);
  Task disableGenContactor04Coil(1500, TASK_FOREVER, &disableGenContactor04CoilRoutine);
  Task enableGenContactor05Coil(1500, TASK_FOREVER, &enableGenContactor05CoilRoutine);
  Task disableGenContactor05Coil(1500, TASK_FOREVER, &disableGenContactor05CoilRoutine); 
  Task disengageAllGenContactors(1500, TASK_FOREVER, &disengageAllGenContactorsRoutine);
  Task engageAllGenContactors(1500, TASK_FOREVER, &engageAllGenContactorsRoutine);

  Task checkGenContactor01Aux(1500, TASK_FOREVER, &genContactor01AuxRoutine);
  Task checkGenContactor02Aux(1500, TASK_FOREVER, &genContactor02AuxRoutine);
  Task checkGenContactor03Aux(1500, TASK_FOREVER, &genContactor03AuxRoutine);
  Task checkGenContactor04Aux(1500, TASK_FOREVER, &genContactor04AuxRoutine);
  Task checkGenContactor05Aux(1500, TASK_FOREVER, &genContactor05AuxRoutine);
  Task checkGenPowerAvailable(1500, TASK_FOREVER, &genPowerAvailableRoutine);
  Task checkDistribution04PowerAvailable(2500, TASK_FOREVER, &distribution04PowerAvailableRoutine);
  Task checkDistribution05PowerAvailable(1500, TASK_FOREVER, &distribution05PowerAvailableRoutine);

  

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  
    int actuatorPins[] = {GEN_CONTACTOR_01_COIL, GEN_CONTACTOR_03_COIL, GEN_CONTACTOR_02_COIL, GEN_CONTACTOR_04_COIL, GEN_CONTACTOR_05_COIL};
    for (int i = 0;  i < 5; i++) 
    {
      pinMode(actuatorPins[i], OUTPUT);
      digitalWrite(actuatorPins[i], HIGH);
    }

    int interruptPins[] = {GEN_CONTACTOR_01_AUX, GEN_CONTACTOR_02_AUX, GEN_CONTACTOR_03_AUX, GEN_CONTACTOR_04_AUX, GEN_CONTACTOR_05_AUX, GEN_POWER_AVAILABLE, DISTRIBUTION_04_POWER_AVAILABLE, DISTRIBUTION_05_POWER_AVAILABLE};   
    for (int i = 0;  i < 8; i++) 
    {
      pinMode(interruptPins[i], INPUT);
    }
    node07Task();

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
  reciever(inValue);

//  testSend();
}

boolean reconnect() {
  if (client.connect("Node07")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node07 connected");
    // ... and resubscribe
    client.subscribe("node07", 1);
    client.subscribe("rstTopic", 1);

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

char reciever(String Value) {
   Serial.println("From callback: " +Value);

  if ((Value == "E.AGENC")){ 
     engageAllGenContactors.enable();
  }else if((Value == "RST")){
    disengageAllGenContactors.enable();
  }
}


void node07Task() {
    userScheduler.addTask(checkGenContactor01Aux);
    checkGenContactor01Aux.enable();
  
    userScheduler.addTask(checkGenContactor02Aux);
    checkGenContactor02Aux.enable();
  
    userScheduler.addTask(checkGenContactor03Aux);
    checkGenContactor03Aux.enable();
  
    userScheduler.addTask(checkGenContactor04Aux);
    checkGenContactor04Aux.enable();
  
    userScheduler.addTask(checkGenContactor05Aux);
    checkGenContactor05Aux.enable();
  
    userScheduler.addTask(checkGenPowerAvailable);
    checkGenPowerAvailable.enable();
  
    userScheduler.addTask(checkDistribution04PowerAvailable);
    checkDistribution04PowerAvailable.enable();
  
    userScheduler.addTask(checkDistribution05PowerAvailable);
    checkDistribution05PowerAvailable.enable();

    userScheduler.addTask(enableGenContactor01Coil);
    userScheduler.addTask(disableGenContactor01Coil);
    userScheduler.addTask(enableGenContactor02Coil);
    userScheduler.addTask(disableGenContactor02Coil);
    userScheduler.addTask(enableGenContactor03Coil);
    userScheduler.addTask(disableGenContactor03Coil);
    userScheduler.addTask(enableGenContactor04Coil);
    userScheduler.addTask(disableGenContactor04Coil);
    userScheduler.addTask(enableGenContactor05Coil);
    userScheduler.addTask(enableGenContactor05Coil);
    userScheduler.addTask(disengageAllGenContactors);
    userScheduler.addTask(engageAllGenContactors);
//    userScheduler.addTask(taskSendMessage);
//    taskSendMessage.enable();
}
