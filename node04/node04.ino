
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

//Actuators
#define STABILIZER_CONTACTOR_OPEN_CLOSE 12
#define GEN_INPUT_TO_STABILIZER_OPEN 33
#define GEN_INPUT_TO_STABILIZER_CLOSE 32
#define UTILITY_TO_STABILIZER_BREAKER_CLOSE 5
#define UTILITY_TO_STABILIZER_BREAKER_OPEN 18

//interrupts
#define INPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE 27
#define OUTPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE 14
#define STABILIZER_CONTACTOR_AUX 26
#define GEN_INPUT_AVAILABLE 13
#define GEN_OUTPUT_AVAILABLE 15
#define GEN_AUX 4
#define UTILITY_TO_STABILIZER_POWER_AVAILABLE 19
#define UTILITY_TO_STABILIZER_BREAKER_AUX 21


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
IPAddress local_IP(192, 168, 2, 51);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient4;
PubSubClient client(espClient4);

Scheduler userScheduler; // to control your personal task


 //prototypes
  void inputFromStabilizerContactorAvailableRoutine();
  void outputFromStabilizerContactorAvailableRoutine();
  void stabilizerContactorAuxRoutine();
  void genInputAvailableRoutine();
  void genOutputAvailableRoutine();
  void genAuxRoutine();
  void utilityToStabilizerPowerAvailableRoutine();
  void utilityToStabilizerBreakerAuxRoutine();

  void enableStabilizerContactorOpenAndCloseRoutine();
  void disableStabilizerContactorOpenAndCloseRoutine();
  void openGenInputToStabilizerRoutine();
  void closeGenInputToStabilizerRoutine();
  void closeUtilityToStabilizerBreakerRoutine();
  void openUtilityToStabilizerBreakerRoutine();
  void hardResetAllLoadBreakersRoutine();

  Task enableStabilizerContactorOpenAndClose(1500, TASK_FOREVER, &enableStabilizerContactorOpenAndCloseRoutine);
  Task disableStabilizerContactorOpenAndClose(1500, TASK_FOREVER, &disableStabilizerContactorOpenAndCloseRoutine);
  Task openGenInputToStabilizer(1500, TASK_FOREVER, &openGenInputToStabilizerRoutine);
  Task closeGenInputToStabilizer(1500, TASK_FOREVER, &closeGenInputToStabilizerRoutine);
  Task closeUtilityToStabilizerBreaker(1500, TASK_FOREVER, &closeUtilityToStabilizerBreakerRoutine);
  Task openUtilityToStabilizerBreaker(1500, TASK_FOREVER, &openUtilityToStabilizerBreakerRoutine);
  Task hardResetAllLoadBreakers(1500, TASK_FOREVER, &hardResetAllLoadBreakersRoutine);
  
  Task checkInputFromStabilizerContactorAvailable(1500, TASK_FOREVER, &inputFromStabilizerContactorAvailableRoutine);
  Task checkOutputFromStabilizerContactorAvailable(1500, TASK_FOREVER, &outputFromStabilizerContactorAvailableRoutine);
  Task checkStabilizerContactor(1500, TASK_FOREVER, &stabilizerContactorAuxRoutine);
  Task checkGenInputAvailable(1500, TASK_FOREVER, &genInputAvailableRoutine);
  Task checkGenOutputAvailable(1500, TASK_FOREVER, &genOutputAvailableRoutine);
  Task checkGenAux(1500, TASK_FOREVER, &genAuxRoutine);
  Task checkUtilityToStabilizerPowerAvailable(1500, TASK_FOREVER, &utilityToStabilizerPowerAvailableRoutine);
  Task checkUtilityToStabilizerBreakerAux(1500, TASK_FOREVER, &utilityToStabilizerBreakerAuxRoutine);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
 int initPins[] = {STABILIZER_CONTACTOR_AUX,GEN_AUX,UTILITY_TO_STABILIZER_BREAKER_AUX}; 
  for (int i = 0;  i < 3; i++) 
  {
    pinMode(initPins[i], INPUT);
  }

  int actuatorPins[] = {GEN_INPUT_TO_STABILIZER_OPEN, GEN_INPUT_TO_STABILIZER_CLOSE, UTILITY_TO_STABILIZER_BREAKER_CLOSE, UTILITY_TO_STABILIZER_BREAKER_OPEN};
  for (int i = 0;  i < 4; i++) 
  {
    pinMode(actuatorPins[i], OUTPUT);
    digitalWrite(actuatorPins[i], HIGH);
  }

  int interruptPins[] = {INPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE, OUTPUT_FROM_STABILIZER_CONTACTOR_AVAILABLE, STABILIZER_CONTACTOR_AUX, GEN_INPUT_AVAILABLE, GEN_OUTPUT_AVAILABLE, GEN_AUX, UTILITY_TO_STABILIZER_POWER_AVAILABLE, UTILITY_TO_STABILIZER_BREAKER_AUX};   
  for (int i = 0;  i < 8; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }
    //for the contactor
    pinMode(STABILIZER_CONTACTOR_OPEN_CLOSE, OUTPUT);
    digitalWrite(STABILIZER_CONTACTOR_OPEN_CLOSE, LOW);
  node04Task();

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
  if (client.connect("Node04")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node04 connected");
    // ... and resubscribe
    client.subscribe("node04", 1);
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
   
  if ((Value == "E.STBC")){ 
      enableStabilizerContactorOpenAndClose.enable();
  }else if((Value == "E.GITOSTB")){
      closeGenInputToStabilizer.enable();
  }else if((Value == "E.UTOSTB")){
      closeUtilityToStabilizerBreaker.enable();
  }else if((Value == "RST")){
      hardResetAllLoadBreakers.enable();
  }else if((Value == "D.STBC")){
    disableStabilizerContactorOpenAndClose.enable();
  }else if((Value == "D.GITOSTB")){
    openGenInputToStabilizer.enable();
  }else if((Value == "D.UTOSTB")){
    openUtilityToStabilizerBreaker.enable();
  }
}


void node04Task() {
  userScheduler.addTask(checkInputFromStabilizerContactorAvailable);
  checkInputFromStabilizerContactorAvailable.enable();

  userScheduler.addTask(checkOutputFromStabilizerContactorAvailable);
  checkOutputFromStabilizerContactorAvailable.enable();

  userScheduler.addTask(checkStabilizerContactor);
  checkStabilizerContactor.enable();

  userScheduler.addTask(checkGenInputAvailable);
  checkGenInputAvailable.enable();

  userScheduler.addTask(checkGenOutputAvailable);
  checkGenOutputAvailable.enable();

  userScheduler.addTask(checkGenAux);
  checkGenAux.enable();

  userScheduler.addTask(checkUtilityToStabilizerPowerAvailable);
  checkUtilityToStabilizerPowerAvailable.enable();

  userScheduler.addTask(checkUtilityToStabilizerBreakerAux);
  checkUtilityToStabilizerBreakerAux.enable();

  userScheduler.addTask(enableStabilizerContactorOpenAndClose);
  userScheduler.addTask(disableStabilizerContactorOpenAndClose);
  userScheduler.addTask(openGenInputToStabilizer);
  userScheduler.addTask(closeGenInputToStabilizer);
  userScheduler.addTask(closeUtilityToStabilizerBreaker);
  userScheduler.addTask(openUtilityToStabilizerBreaker);
  userScheduler.addTask(hardResetAllLoadBreakers);
//  userScheduler.addTask(taskSendMessage);
//  taskSendMessage.enable();
}
