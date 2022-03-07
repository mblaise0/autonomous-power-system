#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

#define BOARD_ID "node09"

#define GENERATOR_01_START_AND_STOP 12
#define GENERATOR_01_COIL_OPEN_AND_CLOSE 14
#define OUTPUT_BREAKER_CLOSE 33
#define OUTPUT_BREAKER_OPEN 32
#define GENERATOR_02_START_AND_STOP 13
#define GENERATOR_02_COIL_OPEN_AND_CLOSE 2

#define POWER_AVAILABLE_ON_GENERATOR_01 27
#define GENERATOR_01_AUX 26
#define POWER_AVAILABLE_ON_GENERATOR_02 15
#define GENERATOR_02_AUX 4
#define POWER_AVAILABLE_ON_MAIN_GRID 34
#define POWER_AVAILABLE_OUTGOING_BREAKER_TO_DC 25

#define DEBUG 1

const char* ssid = "iomariTPLINK";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 46);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient10;
PubSubClient client(espClient10);

Scheduler userScheduler; 

enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 
State currentState;

void startGen01();
void stopGen01();
void engage335Gen01ContactorCoil();
void disengage335Gen01ContactorCoil();
void closeOutBreaker(); 
void openOutputBreaker();
void startGen02();
void stopGen02();
void engage335Gen02ContactorCoil();
void resetAllLoadActuators();

void powerAvailableOnGenerator01Routine();
void generatot01AuxRoutine();
void powerAvailableGenerator02Routine();
void generator02AuxRoutine();
void powerAvailableOnMainGridRoutine();
void powerAvailableOutgoingBreakerToDatacenterRoutine();

Task checkPowerAvailableOnGenerator01(1500, TASK_FOREVER, &powerAvailableOnGenerator01Routine);
Task checkPowerAvailableOnGenerator02(1500, TASK_FOREVER, &powerAvailableGenerator02Routine);
Task checkGeneratot01Aux(1500, TASK_FOREVER, &generatot01AuxRoutine);
Task checkGeneratot02Aux(1500, TASK_FOREVER, &generator02AuxRoutine);
Task checkPowerAvailableOnMainGrid(1500, TASK_FOREVER, &powerAvailableOnMainGridRoutine);
Task checkPowerAvailableOutgoingBreakerToDatacenter(1500, TASK_FOREVER, &powerAvailableOutgoingBreakerToDatacenterRoutine);
   
Task activateStartGen01(1500, TASK_FOREVER, &startGen01);
Task activateStopGen01(1500, TASK_FOREVER, &stopGen01);

Task activate335Gen01ContactorCoil(1500, TASK_FOREVER, &engage335Gen01ContactorCoil);
Task deactivate335Gen01ContactorCoil(1500, TASK_FOREVER, &disengage335Gen01ContactorCoil);
Task activateOutBreaker(1500, TASK_FOREVER, &closeOutBreaker);
Task deactivateOutBreaker(1500, TASK_FOREVER, &openOutputBreaker);
Task activateStartGen02(1500, TASK_FOREVER, &startGen02);
Task activateStopGen02(1500, TASK_FOREVER, &stopGen02);
Task activate335Gen02ContactorCoil(1500, TASK_FOREVER, &engage335Gen02ContactorCoil);
Task activateResetForAllLoadActuators(1500, TASK_FOREVER, &resetAllLoadActuators);



void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
  
  int contactorsPins[] = {GENERATOR_01_COIL_OPEN_AND_CLOSE, GENERATOR_02_COIL_OPEN_AND_CLOSE, GENERATOR_01_START_AND_STOP, GENERATOR_02_START_AND_STOP};
  for (int i = 0; i < 4; i++){
    pinMode(contactorsPins[i], OUTPUT);
    digitalWrite(contactorsPins[i], HIGH);
  }

  int breaker[] = {OUTPUT_BREAKER_CLOSE, OUTPUT_BREAKER_OPEN};
  for (int i = 0; i < 2; i++){
    pinMode(breaker[i], OUTPUT);
    digitalWrite(breaker[i], HIGH);
  }

  int interruptsPin[] = {POWER_AVAILABLE_ON_GENERATOR_01, GENERATOR_01_AUX, POWER_AVAILABLE_ON_GENERATOR_02, GENERATOR_02_AUX, POWER_AVAILABLE_ON_MAIN_GRID, POWER_AVAILABLE_OUTGOING_BREAKER_TO_DC};
  for (int i = 0; i < 6; i++){
    pinMode(interruptsPin[i], INPUT);
//    digitalWrite(interruptsPin[i], HIGH);
  }
  userScheduler.addTask(activateStartGen01);
  userScheduler.addTask(activateStopGen01);
  userScheduler.addTask(activate335Gen01ContactorCoil);
  userScheduler.addTask(deactivate335Gen01ContactorCoil);
  userScheduler.addTask(activateOutBreaker);
  userScheduler.addTask(deactivateOutBreaker);
  userScheduler.addTask(activateStartGen02);
  userScheduler.addTask(activateStopGen02);
  userScheduler.addTask(activate335Gen02ContactorCoil);
  userScheduler.addTask(activateResetForAllLoadActuators);

  userScheduler.addTask(checkPowerAvailableOnGenerator01);
  checkPowerAvailableOnGenerator01.enable();

  userScheduler.addTask(checkPowerAvailableOnGenerator02);
  checkPowerAvailableOnGenerator02.enable();

  userScheduler.addTask(checkGeneratot01Aux);
  checkGeneratot01Aux.enable();

  userScheduler.addTask(checkGeneratot02Aux);
  checkGeneratot02Aux.enable();

  userScheduler.addTask(checkPowerAvailableOnMainGrid);
  checkPowerAvailableOnMainGrid.enable();

  userScheduler.addTask(checkPowerAvailableOutgoingBreakerToDatacenter);
  checkPowerAvailableOutgoingBreakerToDatacenter.enable();
  
  lastReconnectAttempt = 0;
  
  digitalWrite(GENERATOR_01_START_AND_STOP,  HIGH);
  digitalWrite(GENERATOR_02_START_AND_STOP,  HIGH);

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
  if (client.connect("Node09")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node09 connected");
    // ... and resubscribe
    client.subscribe("node09", 1);
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


int startGenSignalCount = 2;
int startGenSignal = 0; 

int startGenSignalCount02 = 5;
int startGenSignal02 = 0; 


int stopGenSignalCount = 2;
int stopGenSignal = 0; 

int stopGenSignalCount2 = 5;
int stopGenSignal2 = 0; 



char reciever(String Value) {
   Serial.println("From callback: " +Value);

  if((Value == "STRT335")){
    startGenSignal++;
    startGenSignal02++;
    if (startGenSignal == startGenSignalCount){
      activateStartGen01.enable();
    }else if (startGenSignal02 == startGenSignalCount02){
     activateStartGen02.enable();
   }  
  }else if((Value == "STP335G")){
    stopGenSignal++;
    stopGenSignal2++;
    if (stopGenSignal == stopGenSignalCount){
      activateStopGen01.enable();
    }else if (stopGenSignal2 == stopGenSignalCount2){
      activateStopGen02.enable();
    }
  }else if((Value == "E.335G01C")){
    activate335Gen01ContactorCoil.enable();
  }else if((Value == "E.335OB")){
    activateOutBreaker.enable();
  }else if((Value == "E.335G02C")){
   activate335Gen02ContactorCoil.enable();
  }else if((Value == "RST")){
   activateResetForAllLoadActuators.enable();
  }
}
