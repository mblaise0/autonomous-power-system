
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

//Actuators
#define DEBUG 1

//ACTUATORS
#define GEN_01_CONTACTOR_01_COIL 4
#define GEN_01_CONTACTOR_02_COIL 5
#define GEN_02_CONTACTOR_01_COIL 21
#define GEN_02_CONTACTOR_02_COIL 22
#define GEN_03_CONTACTOR_01_COIL 14
#define GEN_03_CONTACTOR_02_COIL 12


//INTERRUPTS
#define GEN_02_POWER_AVAILABLE 19
#define GEN_02_CONTACTOR_01_AUX 15
#define GEN_02_CONTACTOR_02_AUX 13
#define GEN_01_POWER_AVAILABLE 2
#define GEN_01_CONTACTOR_01_AUX 18
#define GEN_01_CONTACTOR_02_AUX 23
#define GEN_03_POWER_AVAILABLE 27
#define GEN_03_CONTACTOR_01_AUX 26
#define GEN_03_CONTACTOR_02_AUX 25
#define GEN_GRID_POWER_AVAILABLE 33

#define numberOfIteration 8

const char* ssid = "iomariTPLink";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 53);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient2;
PubSubClient client(espClient2);

Scheduler userScheduler; // to control your personal task


  void gen01PowerAvailableRoutine();
//  void gen01Contactor01AuxRoutine();

  void gen02PowerAvailableRoutine();
  void gen03PowerAvailableRoutine();
  void genGridPowerAvailableRoutine();
  void gen01ContactorsAuxRoutine();
  void gen02ContactorsAuxRoutine();
  void gen03ContactorsAuxRoutine();

  void engageGen01ContactorCoil();
  void engageGen02ContactorCoil(); 
  void engageGen03ContactorCoil();
  void disengageGen01Contactors();
  void disengageGen02Contactors();
  void disengageGen03Contactors(); 
  void ResetAllLoadContactors();
    
  Task activateGen01ContactorCoil(1500, TASK_FOREVER, &engageGen01ContactorCoil);
  Task activateGen02ContactorCoil(1500, TASK_FOREVER, &engageGen02ContactorCoil);
  Task activateGen03ContactorCoil(1500, TASK_FOREVER, &engageGen03ContactorCoil);
  Task deactivateGen01ContactorCoil(1500, TASK_FOREVER, &disengageGen01Contactors);
  Task deactivateGen02ContactorCoil(1500, TASK_FOREVER, &disengageGen02Contactors);
  Task deactivateGen03ContactorCoil(1500, TASK_FOREVER, &disengageGen03Contactors);
  Task deactivateAllLoadContactors(1500, TASK_FOREVER, &ResetAllLoadContactors);

  Task checkGen01PowerAvailableState(1500, TASK_FOREVER, &gen01PowerAvailableRoutine);
  Task checkGen02PowerAvailableState(1500, TASK_FOREVER, &gen02PowerAvailableRoutine);
  Task checkGen03PowerAvailableState(1500, TASK_FOREVER, &gen03PowerAvailableRoutine);
  Task checkGenGridPowerAvailableState(1500, TASK_FOREVER, &genGridPowerAvailableRoutine);

  Task checkGen01ContactorAuxState(1500, TASK_FOREVER, &gen01ContactorsAuxRoutine);
  Task checkGen02ContactorAuxState(1500, TASK_FOREVER, &gen02ContactorsAuxRoutine);
  Task checkGen03ContactorAuxState(1500, TASK_FOREVER, &gen03ContactorsAuxRoutine);

 enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 State currentState;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  int actuatorPins[] = {GEN_01_CONTACTOR_01_COIL, GEN_01_CONTACTOR_02_COIL, GEN_02_CONTACTOR_01_COIL, GEN_02_CONTACTOR_02_COIL, GEN_03_CONTACTOR_01_COIL, GEN_03_CONTACTOR_02_COIL};
  for (int i = 0;  i < numberOfIteration; i++) 
  {
    pinMode(actuatorPins[i], OUTPUT);
    digitalWrite(actuatorPins[i], LOW);
  }
  
  int interruptPins[] = {GEN_02_POWER_AVAILABLE, GEN_02_CONTACTOR_01_AUX, GEN_02_CONTACTOR_02_AUX, GEN_01_POWER_AVAILABLE, GEN_01_CONTACTOR_01_AUX, GEN_01_CONTACTOR_02_AUX, GEN_03_POWER_AVAILABLE, GEN_03_CONTACTOR_01_AUX, GEN_03_CONTACTOR_02_AUX, GEN_GRID_POWER_AVAILABLE};
  for (int i = 0;  i < 10; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }
  node02Task();

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
 Serial.println("Welcome...........");
}

boolean reconnect() {
  if (client.connect("Node02")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node02 connected");
    // ... and resubscribe
    client.subscribe("node02", 1);
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
  if ((Value == "E.GenCN")){ 
      activateGen01ContactorCoil.enable();   
  }else if((Value == "E.G2CN")){
     activateGen02ContactorCoil.enable();   
  }else if((Value == "E.G3CN")){
      activateGen03ContactorCoil.enable();  
  }else if((Value == "RST")){
       deactivateAllLoadContactors.enable();    
  }
}


void node02Task(){
    userScheduler.addTask(checkGen01PowerAvailableState);
    checkGen01PowerAvailableState.enable();
    
    userScheduler.addTask(checkGen02PowerAvailableState);
    checkGen02PowerAvailableState.enable();

    userScheduler.addTask(checkGen03PowerAvailableState);
    checkGen03PowerAvailableState.enable();

    userScheduler.addTask(checkGen01ContactorAuxState);
    checkGen01ContactorAuxState.enable(); 

    userScheduler.addTask(checkGen02ContactorAuxState);
    checkGen02ContactorAuxState.enable(); 

    userScheduler.addTask(checkGen03ContactorAuxState);
    checkGen03ContactorAuxState.enable(); 

    userScheduler.addTask(checkGenGridPowerAvailableState);
    checkGenGridPowerAvailableState.enable();   

    userScheduler.addTask(deactivateGen03ContactorCoil);
    userScheduler.addTask(deactivateGen02ContactorCoil);
    userScheduler.addTask(deactivateGen01ContactorCoil);
    userScheduler.addTask(activateGen03ContactorCoil);
    userScheduler.addTask(activateGen02ContactorCoil);
    userScheduler.addTask(activateGen01ContactorCoil);
    userScheduler.addTask(deactivateAllLoadContactors);
}
