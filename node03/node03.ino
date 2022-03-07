
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
#define MG_SET_CONTACTOR_COIL 23
#define CONTACTOR_335_COIL 22
#define STABILIZER_BREAKER_TO_DC_GRID_CLOSE 13
#define STABILIZER_BREAKER_TO_DC_GRID_OPEN 12
#define OUTPUT_BREAKER_TO_DATA_CENTER_CLOSE 25
#define OUTPUT_BREAKER_TO_DATA_CENTER_OPEN 33

//INTERRUPTS
#define POWER_AVAILABLE_335_INCOMER 21
#define MG_SET_CONTACTOR_AUX 19
#define CONTACTOR_335_COIL_AUX 15
#define POWER_AVAILABLE_DATA_CENTER_GRID 2
#define MG_SET_POWER_AVAILABLE 4
#define POWER_AVAILABLE_DATA_CENTER_OUTPUT 5
#define DATA_CENTER_OUTPUT_BREAKER_AUX 18
#define STABILIZER_BREAKER_TO_DC_GRID_AUX 14
#define POWER_AVAILABLE_TO_STABILIZER 27
#define POWER_AVAILABLE_FROM_STABILIZER 26
#define OUTPUT_BREAKER_TO_DATA_CENTER_AUX 35


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
IPAddress local_IP(192, 168, 2, 52);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient4;
PubSubClient client(espClient4);

Scheduler userScheduler; // to control your personal task


  //prototypes
  void powerAvailable335InRoutine();
  void mgSetContactorAuxRoutine();
  void contator335CoilAuxRoutine();
  void powerAvailableDCGridRoutine();
  void mgSetPowerAvailableRoutine();
  void powerAvailableDCOutputRoutine();
  void dcOutputBreakerAuxRoutine();
  void stabilizerBreakerToDCAuxRoutine();
  void powerAvailableToStabRoutine();
  void powerAvailableFromStabRoutine();
  void outputBreakerDataCenterAuxRoutine();

  void engageMgSetContactorCoil();
  void disengageMgSetContactorCoil(); 
  void engageContactor335Coil();
  void disengageContactor335Coil(); 
  void closeStabBreakerToDc();
  void openStabBreakerToDc();
  void closeOutBreakerToDatacenter();
  void openOutBreakerToDatacenter();
  void resetAllLoadActuators();
  
  Task checkPowerAvailable335In(1500, TASK_FOREVER, &powerAvailable335InRoutine);
  Task checkMgSetContactorAux(1500, TASK_FOREVER, &mgSetContactorAuxRoutine);
  Task checkContator335CoilAux(1500, TASK_FOREVER, &contator335CoilAuxRoutine);
  Task checkPowerAvailableDCGrid(1500, TASK_FOREVER, &powerAvailableDCGridRoutine);
  Task checkMgSetPowerAvailable(1500, TASK_FOREVER, &mgSetPowerAvailableRoutine);
  Task checkPowerAvailableDCOutput(1500, TASK_FOREVER, &powerAvailableDCOutputRoutine);
  Task checkDcOutputBreakerAux(1500, TASK_FOREVER, &dcOutputBreakerAuxRoutine);
  Task checkStabilizerBreakerToDCAux(1500, TASK_FOREVER, &stabilizerBreakerToDCAuxRoutine);
  Task checkPowerAvailableToStab(1500, TASK_FOREVER, &powerAvailableToStabRoutine);
  Task checkPowerAvailableFromStab(1500, TASK_FOREVER, &powerAvailableFromStabRoutine);
  Task checkOutputBreakerDataCenterAux(1500, TASK_FOREVER, &outputBreakerDataCenterAuxRoutine);
  
  Task activateMgSetContactorCoil(1500, TASK_FOREVER, &engageMgSetContactorCoil);
  Task deactivateMgSetContactorCoil(1500, TASK_FOREVER, &disengageMgSetContactorCoil);
  Task activateContactor335Coil(1500, TASK_FOREVER, &engageContactor335Coil);
  Task deactivateContactor335Coil(1500, TASK_FOREVER, &disengageContactor335Coil);
  Task activateStabBreakerToDc(4500, TASK_FOREVER, &closeStabBreakerToDc);
  Task deactivateStabBreakerToDc(1500, TASK_FOREVER, &openStabBreakerToDc);
  Task activateOutBreakerToDatacenter(4500, TASK_FOREVER, &closeOutBreakerToDatacenter);
  Task deactivateOutBreakerToDatacenter(1500, TASK_FOREVER, &openOutBreakerToDatacenter);
  Task resetAllLoadActuator(4500, TASK_FOREVER, &resetAllLoadActuators);
  

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
 int initPins[] = {POWER_AVAILABLE_335_INCOMER, MG_SET_CONTACTOR_AUX, CONTACTOR_335_COIL_AUX, POWER_AVAILABLE_DATA_CENTER_GRID, MG_SET_POWER_AVAILABLE, POWER_AVAILABLE_DATA_CENTER_OUTPUT, DATA_CENTER_OUTPUT_BREAKER_AUX, STABILIZER_BREAKER_TO_DC_GRID_AUX, POWER_AVAILABLE_TO_STABILIZER, POWER_AVAILABLE_FROM_STABILIZER, OUTPUT_BREAKER_TO_DATA_CENTER_AUX}; 
  for (int i = 0;  i < 11; i++) 
  {
    pinMode(initPins[i], INPUT);
  }

    int actuatorPins1[] = {MG_SET_CONTACTOR_COIL, CONTACTOR_335_COIL};
    for (int i = 0;  i < 2; i++) 
    {
      pinMode(actuatorPins1[i], OUTPUT);
      digitalWrite(actuatorPins1[i], LOW);
    }

    int actuatorPins2[] = {STABILIZER_BREAKER_TO_DC_GRID_CLOSE, STABILIZER_BREAKER_TO_DC_GRID_OPEN, OUTPUT_BREAKER_TO_DATA_CENTER_CLOSE, OUTPUT_BREAKER_TO_DATA_CENTER_OPEN};
    for (int i = 0;  i < 4; i++) 
    {
      pinMode(actuatorPins2[i], OUTPUT);
      digitalWrite(actuatorPins2[i], HIGH);
    }
  node03Task();

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
  if (client.connect("Node03")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node03 connected");
    // ... and resubscribe
    client.subscribe("node03", 1);
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

  if ((Value == "E.MGCC")){ 
     activateMgSetContactorCoil.enable();
  }else if((Value == "E.C335C")){
     activateContactor335Coil.enable();
  }else if((Value == "E.SBTODC")){
      activateStabBreakerToDc.enable();
  }else if((Value == "E.OBTODC")){
      activateOutBreakerToDatacenter.enable();
  }else if((Value == "RST")){
       resetAllLoadActuator.enable();
  }
}

void node03Task() {
  userScheduler.addTask(checkPowerAvailable335In);
  checkPowerAvailable335In.enable(); 
  
  userScheduler.addTask(checkMgSetContactorAux);
  checkMgSetContactorAux.enable();
  
  userScheduler.addTask(checkContator335CoilAux);
  checkContator335CoilAux.enable();
  
  userScheduler.addTask(checkPowerAvailableDCGrid);
  checkPowerAvailableDCGrid.enable();
  
  userScheduler.addTask(checkMgSetPowerAvailable);
  checkMgSetPowerAvailable.enable();
  
  userScheduler.addTask(checkPowerAvailableDCOutput);
  checkPowerAvailableDCOutput.enable();
  
  userScheduler.addTask(checkDcOutputBreakerAux);
  checkDcOutputBreakerAux.enable();

  userScheduler.addTask(checkStabilizerBreakerToDCAux);
  checkStabilizerBreakerToDCAux.enable();

  userScheduler.addTask(checkPowerAvailableToStab);
  checkPowerAvailableToStab.enable();

  userScheduler.addTask(checkPowerAvailableFromStab);
  checkPowerAvailableFromStab.enable();

  userScheduler.addTask(checkOutputBreakerDataCenterAux);
  checkOutputBreakerDataCenterAux.enable();

  userScheduler.addTask(activateMgSetContactorCoil);
  userScheduler.addTask(deactivateMgSetContactorCoil);
  userScheduler.addTask(deactivateContactor335Coil);
  userScheduler.addTask(activateContactor335Coil);
  userScheduler.addTask(activateStabBreakerToDc);
  userScheduler.addTask(deactivateStabBreakerToDc);
  userScheduler.addTask(activateOutBreakerToDatacenter);
  userScheduler.addTask(deactivateOutBreakerToDatacenter);
  userScheduler.addTask(resetAllLoadActuator);
}
