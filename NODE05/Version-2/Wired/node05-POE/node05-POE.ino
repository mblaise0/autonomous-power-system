/*
 * Node05 was formerly together with the Bypass panel but now has been seperated
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

#define DEBUG 1

#define UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE 23

#define UTILITY_INPUT_TO_STAB_POWER_AVAILABLE 19
#define UTILITY_OR_GEN_CONTACTOR_AUX 22
#define MAIN_BUS_BAR_POWER_AVAILABLE 21

enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 
State currentState;

long lastReconnectAttempt = 0;

// CHANGE THESE SETTINGS FOR YOUR APPLICATION
const char* mqttServerIp = "192.168.2.44"; // IP address of the MQTT broker
const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node05";
const char* mqttUsername = NULL;
const char* mqttPassword = NULL;

IPAddress local_ip(192, 168, 2, 50);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(192, 168, 1, 2);

Scheduler userScheduler; 

//prototypes
void utilityInputToStabPowerAvailableRoutine();
void mainBusBarPowerAvailableRoutine();
void utilityOrGenContactorAuxRoutine();

void CloseUtilityOrGenContactorRoutine();
void OpenUtilityOrGenContactorRoutine();
void hardResetAllLoadBreakers();

Task closeUtilityOrGenContactor(1500, TASK_FOREVER, &CloseUtilityOrGenContactorRoutine);
Task openUtilityOrGenContactor(1500, TASK_FOREVER, &OpenUtilityOrGenContactorRoutine);
Task resetAllLoadBreakers(1500, TASK_FOREVER, &hardResetAllLoadBreakers);

Task checkUtilityInputToStabPowerAvailable(1500, TASK_FOREVER, &utilityInputToStabPowerAvailableRoutine);
Task checkMainBusBarPowerAvailable(1500, TASK_FOREVER, &mainBusBarPowerAvailableRoutine);
Task checkUtilityOrGenContactorAux(1500, TASK_FOREVER, &utilityOrGenContactorAuxRoutine);

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


void setup()
{
  Serial.begin(115200);
  WiFi.setSleep(false);
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);

  int interruptPins[] = {UTILITY_INPUT_TO_STAB_POWER_AVAILABLE, MAIN_BUS_BAR_POWER_AVAILABLE, UTILITY_OR_GEN_CONTACTOR_AUX}; 
  for (int i = 0;  i < 5; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }
  pinMode(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE, OUTPUT);
  digitalWrite(UTILITY_OR_GEN_CONTACTOR_OPEN_AND_CLOSE, LOW);
  node05Task();

  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  ETH.config(local_ip, gateway, subnet, dns);
  
  lastReconnectAttempt = 0;

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
  if (mqttClient.connect("Node05")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    mqttClient.publish("outTopic","Node05 connected");
    // ... and resubscribe
    mqttClient.subscribe("node05", 1);
    mqttClient.subscribe("rstTopic", 1);

  }
   else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  return mqttClient.connected();
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
  }else {  
      mqttClient.loop();
      userScheduler.execute();
  }
}


char reciever(String Value) {
   Serial.println("From callback: " +Value);
   
  if((Value == "E.UOGC")){
      closeUtilityOrGenContactor.enable();
  }else if((Value == "RST")){
      resetAllLoadBreakers.enable();
  }
}


void node05Task() {

  userScheduler.addTask(checkUtilityInputToStabPowerAvailable);
  checkUtilityInputToStabPowerAvailable.enable();

  userScheduler.addTask(checkMainBusBarPowerAvailable);
  checkMainBusBarPowerAvailable.enable();

  userScheduler.addTask(checkUtilityOrGenContactorAux);
  checkUtilityOrGenContactorAux.enable();

  userScheduler.addTask(closeUtilityOrGenContactor);
  userScheduler.addTask(openUtilityOrGenContactor);
  userScheduler.addTask(resetAllLoadBreakers);

}

  
