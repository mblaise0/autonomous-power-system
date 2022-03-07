
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
#define UTILITY_CONTACTOR_01_COIL 12
#define UTILITY_CONTACTOR_02_COIL 14
#define UTILITY_CONTACTOR_03_COIL 33
#define UTILITY_CONTACTOR_04_COIL 13
#define UTILITY_CONTACTOR_05_COIL 2

//INTERRUPTS
#define UTILITY_CONTACTOR_01_AUX 27
#define UTILITY_CONTACTOR_02_AUX 26
#define UTILITY_CONTACTOR_03_AUX 32
#define UTILITY_CONTACTOR_04_AUX 15
#define UTILITY_CONTACTOR_05_AUX 4
#define UTILITY_POWER_AVAILABLE 25
#define DISTRIBUTION_01_POWER_AVAILABLE 34
#define DISTRIBUTION_02_POWER_AVAILABLE 19
#define DISTRIBUTION_03_POWER_AVAILABLE 18


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
IPAddress local_IP(192, 168, 2, 49);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);


WiFiClient espClient6;
PubSubClient client(espClient6);

Scheduler userScheduler; // to control your personal task


//prototypes

  void utilityContactor01AuxRoutine();
  void utilityContactor02AuxRoutine();
  void utilityContactor03AuxRoutine();
  void utilityContactor04AuxRoutine();
  void utilityContactor05AuxRoutine();
  void utilityPowerAvailableRoutine();
  void distribution01PowerAvailableRoutine();
  void distribution02PowerAvailableRoutine();
  void distribution03PowerAvailableRoutine();

  void enableUtilityContactor01CoilRoutine();
  void disableUtilityContactor01CoilRoutine();
  void enableUtilityContactor02CoilRoutine();
  void disableUtilityContactor02CoilRoutine();
  void enableUtilityContactor03CoilRoutine();
  void disableUtilityContactor03CoilRoutine();
  void enableUtilityContactor04CoilRoutine();
  void disableUtilityContactor04CoilRoutine();
  void enableUtilityContactor05CoilRoutine();
  void disableUtilityContactor05CoilRoutine();
  void disengageAllUtilityContactorsRoutine();
  void engageAllUtilityContactorsRoutine();


  Task activateUtilityContactor01Coil(1500, TASK_FOREVER, &enableUtilityContactor01CoilRoutine);
  Task deactivateUtilityContactor01Coil(1500, TASK_FOREVER, &disableUtilityContactor01CoilRoutine);
  Task activateUtilityContactor02Coil(1500, TASK_FOREVER, &enableUtilityContactor02CoilRoutine);
  Task deactivateUtilityContactor02Coil(1500, TASK_FOREVER, &disableUtilityContactor02CoilRoutine);
  Task activateUtilityContactor03Coil(1500, TASK_FOREVER, &enableUtilityContactor03CoilRoutine);
  Task deactivateUtilityContactor03Coil(1500, TASK_FOREVER, &disableUtilityContactor03CoilRoutine);
  Task activateUtilityContactor04Coil(1500, TASK_FOREVER, &enableUtilityContactor04CoilRoutine);
  Task deactivateUtilityContactor04Coil(1500, TASK_FOREVER, &disableUtilityContactor04CoilRoutine);
  Task activateUtilityContactor05Coil(1500, TASK_FOREVER, &enableUtilityContactor05CoilRoutine);
  Task deactivateUtilityContactor05Coil(1500, TASK_FOREVER, &disableUtilityContactor05CoilRoutine);
  Task engageAllUtilityContactors(4500, TASK_FOREVER, &engageAllUtilityContactorsRoutine);
  Task disengageAllUtilityContactors(2500, TASK_FOREVER, &disengageAllUtilityContactorsRoutine);

  Task checkUtilityContactor01Aux(1500, TASK_FOREVER, &utilityContactor01AuxRoutine);
  Task checkUtilityContactor02Aux(1500, TASK_FOREVER, &utilityContactor02AuxRoutine);
  Task checkUtilityContactor03Aux(1500, TASK_FOREVER, &utilityContactor03AuxRoutine);
  Task checkUtilityContactor04Aux(1500, TASK_FOREVER, &utilityContactor04AuxRoutine);
  Task checkUtilityContactor05Aux(1500, TASK_FOREVER, &utilityContactor05AuxRoutine);
  Task checkUtilityPowerAvailable(1500, TASK_FOREVER, &utilityPowerAvailableRoutine);
  Task checkDistribution01PowerAvailable(1500, TASK_FOREVER, &distribution01PowerAvailableRoutine);
  Task checkDistribution02PowerAvailable(1500, TASK_FOREVER, &distribution02PowerAvailableRoutine);
  Task checkDistribution03PowerAvailable(1500, TASK_FOREVER, &distribution03PowerAvailableRoutine);


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  
  int interruptPins[] = {UTILITY_CONTACTOR_01_AUX, UTILITY_CONTACTOR_02_AUX, UTILITY_CONTACTOR_03_AUX, UTILITY_CONTACTOR_04_AUX, UTILITY_CONTACTOR_05_AUX, UTILITY_POWER_AVAILABLE, DISTRIBUTION_01_POWER_AVAILABLE, DISTRIBUTION_02_POWER_AVAILABLE, DISTRIBUTION_03_POWER_AVAILABLE}; 
  for (int i = 0;  i < 9; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }
  int actuatorPins[] = {UTILITY_CONTACTOR_01_COIL, UTILITY_CONTACTOR_02_COIL, UTILITY_CONTACTOR_03_COIL, UTILITY_CONTACTOR_04_COIL, UTILITY_CONTACTOR_05_COIL};
  for (int i = 0;  i < 5; i++) 
  {
    pinMode(actuatorPins[i], OUTPUT);
    digitalWrite(actuatorPins[i], HIGH);  //contactors are triggered with ground
  }
    node06Task();

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
  if (client.connect("Node06")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node06 connected");
    // ... and resubscribe
    client.subscribe("node06", 1);
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

  if ((Value == "E.AUTLC")){ 
     engageAllUtilityContactors.enable(); 
  }else if((Value == "RST")){
     disengageAllUtilityContactors.enable(); 
  }
}


void node06Task() {
  userScheduler.addTask(checkUtilityContactor01Aux);
  checkUtilityContactor01Aux.enable();

  userScheduler.addTask(checkUtilityContactor02Aux);
  checkUtilityContactor02Aux.enable();

  userScheduler.addTask(checkUtilityContactor03Aux);
  checkUtilityContactor03Aux.enable();

  userScheduler.addTask(checkUtilityContactor04Aux);
  checkUtilityContactor04Aux.enable();

  userScheduler.addTask(checkUtilityContactor05Aux);
  checkUtilityContactor05Aux.enable();

  userScheduler.addTask(checkUtilityPowerAvailable);
  checkUtilityPowerAvailable.enable();

  userScheduler.addTask(checkDistribution01PowerAvailable);
  checkDistribution01PowerAvailable.enable();

  userScheduler.addTask(checkDistribution02PowerAvailable);
  checkDistribution02PowerAvailable.enable();

  userScheduler.addTask(checkDistribution03PowerAvailable);
  checkDistribution03PowerAvailable.enable();

  userScheduler.addTask(activateUtilityContactor01Coil);
  userScheduler.addTask(deactivateUtilityContactor01Coil);
  userScheduler.addTask(activateUtilityContactor02Coil);
  userScheduler.addTask(deactivateUtilityContactor02Coil);
  userScheduler.addTask(activateUtilityContactor03Coil);
  userScheduler.addTask(deactivateUtilityContactor03Coil);
  userScheduler.addTask(activateUtilityContactor04Coil);
  userScheduler.addTask(deactivateUtilityContactor04Coil);
  userScheduler.addTask(activateUtilityContactor05Coil);
  userScheduler.addTask(activateUtilityContactor05Coil);
  userScheduler.addTask(engageAllUtilityContactors);
  userScheduler.addTask(disengageAllUtilityContactors);
}
