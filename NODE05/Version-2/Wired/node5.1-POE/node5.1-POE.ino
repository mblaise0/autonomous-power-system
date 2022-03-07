/*
 * Node5.1 was formerly together with the node05 but now has an autonomously as the Bypass Node
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

#define BYPASS_BREAKER_OPEN 2
#define BYPASS_BREAKER_CLOSE 4

#define BYPASS_BREAKER_POWER_AVAILABLE_AUX 5
#define BYPASS_BREAKER_AUX 18

enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 
State currentState;

long lastReconnectAttempt = 0;

// CHANGE THESE SETTINGS FOR YOUR APPLICATION
const char* mqttServerIp = "192.168.2.44"; // IP address of the MQTT broker
const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node10";
const char* mqttUsername = NULL;
const char* mqttPassword = NULL;

IPAddress local_ip(192, 168, 2, 57);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(192, 168, 1, 2);

Scheduler userScheduler; 

//prototypes
void bypassBreakerPowerAvailableRoutine();
void bypassBreakerAuxRoutine();

void OpenBypassBreakerRoutine();
void CloseBypassBreakerRoutine();

void hardResetAllLoadBreakers();

Task openBypassBreaker(1500, TASK_FOREVER, &OpenBypassBreakerRoutine);
Task closeBypassBreaker(1500, TASK_FOREVER, &CloseBypassBreakerRoutine);
Task resetAllLoadBreakers(1500, TASK_FOREVER, &hardResetAllLoadBreakers);

Task checkBypassBreakerPowerAvailable(1500, TASK_FOREVER, &bypassBreakerPowerAvailableRoutine);
Task checkBypassBreakerAux(1500, TASK_FOREVER, &bypassBreakerAuxRoutine);

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

  int actuatorPins[] = {BYPASS_BREAKER_OPEN, BYPASS_BREAKER_CLOSE};
  for (int i = 0;  i < 2; i++) 
  {
    pinMode(actuatorPins[i], OUTPUT);
    digitalWrite(actuatorPins[i], HIGH);
  }
  
  int interruptPins[] = {BYPASS_BREAKER_POWER_AVAILABLE_AUX, BYPASS_BREAKER_AUX}; 
  for (int i = 0;  i < 5; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }
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
  if (mqttClient.connect("Node5.1")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    mqttClient.publish("outTopic","Node5.1 connected");
    // ... and resubscribe
    mqttClient.subscribe("node5.1", 1);
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

char reciever(String Value) {
   Serial.println("From callback: " +Value);
   
  if((Value == "C.BYB")){
      closeBypassBreaker.enable();
  }else if((Value == "O.BYB")){
      openBypassBreaker.enable();
  }else if((Value == "RST")){
      resetAllLoadBreakers.enable();
  }
}


void node05Task() {
  userScheduler.addTask(checkBypassBreakerPowerAvailable);
  checkBypassBreakerPowerAvailable.enable();

  userScheduler.addTask(checkBypassBreakerAux);
  checkBypassBreakerAux.enable();

  userScheduler.addTask(openBypassBreaker);
  userScheduler.addTask(closeBypassBreaker);
  userScheduler.addTask(resetAllLoadBreakers);

}
