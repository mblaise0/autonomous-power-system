
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

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

const char* ssid = "iomariTPLink";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 57);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient4;
PubSubClient client(espClient4);

Scheduler userScheduler; // to control your personal task


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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
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
  if (client.connect("Node5.1")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node5.1 connected");
    // ... and resubscribe
    client.subscribe("node5.1", 1);
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
