
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
//#define _TASK_SLEEP_ON_IDLE_RUN
//#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

//Actuators
#define DEBUG 1

#define ACB_DRIVE_OPEN 2
#define ACB_DRIVE_CLOSE 4
#define INTERLOCK_UTILITY_BREAKER_TO_STAB_CLOSE 23
#define INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN 19
#define GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN 26
#define GEN_GRID_DISRTIBUTION_SIDE_BREAKER_CLOSE 15
#define INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN 14
#define INTERLOCK_GENERATOR_BREAKER_TO_STAB_CLOSE 12

#define ACB_BREAKER_AUX 5
#define POWER_AVAILABLE_ON_UTILITY_GRID 18
#define INTERLOCK_UTILITY_BREAKER_TO_STAB_AUX 21
#define GEN_GRID_DISRTIBUTION_SIDE_BREAKER_AUX 13
#define INTERLOCK_GENERATOR_BREAKER_TO_STAB_AUX 27

#define numberOfIteration 8

const char* ssid = "iomariTPLink";
const char* password = "#InspireD#";

long lastReconnectAttempt = 0;

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.2.44";
IPAddress local_IP(192, 168, 2, 54);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient1;
PubSubClient client(espClient1);

Scheduler userScheduler; // to control your personal task


void acbAuxStateRoutine();
void powerAvailableOnUtilityGridStateRoutine();
void interlockUtilityBreakerToStabAuxStateRoutine();
void genGridDistributionSideBreakerAuxStateRoutine();
void interlockGenBreakerToStabAuxStateRoutine();

void openAcbBreaker();
void closeAcbBreaker();
void closeinterlockUtilityBreakerToStab();
void openinterlockUtilityBreakerToStab(); 
void closeGenGridDistributionSideBreaker();
void openGenGridDistributionSideBreaker();
void closeInterlockGenBreakerToStab();
void openInterlockGenBreakerToStab();
void deactivateAllLoadBreakers();
void deactivateAllUtilityLoadBreakers();
void deactivateAllGenLoadBreakers();

Task checkAcbAuxState(1500, TASK_FOREVER, &acbAuxStateRoutine);
Task checkPowerAvailableOnUtilityGridState(1500, TASK_FOREVER, &powerAvailableOnUtilityGridStateRoutine);
Task checkInterlockUtilityBreakerToStabAuxState(1500, TASK_FOREVER, &interlockUtilityBreakerToStabAuxStateRoutine);
Task checkGenGridDistributionSideBreakerAuxState(1500, TASK_FOREVER, &genGridDistributionSideBreakerAuxStateRoutine);
Task checkInterlockGenBreakerToStabAuxState(1500, TASK_FOREVER, &interlockGenBreakerToStabAuxStateRoutine);

Task deactivateAcbBreaker(1500, TASK_FOREVER, &openAcbBreaker);
Task activateAcbBreaker(1500, TASK_FOREVER, &closeAcbBreaker);
Task activateInterlockUtilityBreakerToStab(1500, TASK_FOREVER, &closeinterlockUtilityBreakerToStab);
Task deactivateInterlockUtilityBreakerToStab(1500, TASK_FOREVER, &openinterlockUtilityBreakerToStab);
Task activateGenGridDistributionSideBreaker(1500, TASK_FOREVER, &closeGenGridDistributionSideBreaker);
Task deactivateGenGridDistributionSideBreaker(1500, TASK_FOREVER, &openGenGridDistributionSideBreaker);
Task activateInterlockGenBreakerToStab(1500, TASK_FOREVER, &closeInterlockGenBreakerToStab);
Task deactivateInterlockGenBreakerToStab(1500, TASK_FOREVER, &openInterlockGenBreakerToStab);
Task restAllLoadBreakers(3500, TASK_FOREVER, &deactivateAllLoadBreakers);
Task restAllUtilityLoadBreakers(3500, TASK_FOREVER, &deactivateAllUtilityLoadBreakers);
Task restAllGenLoadBreakers(3500, TASK_FOREVER, &deactivateAllGenLoadBreakers);

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
  int interruptPins[] = {ACB_BREAKER_AUX, POWER_AVAILABLE_ON_UTILITY_GRID, INTERLOCK_UTILITY_BREAKER_TO_STAB_AUX, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_AUX, INTERLOCK_GENERATOR_BREAKER_TO_STAB_AUX};  
  for (int i = 0;  i < 5; i++) 
  {
    pinMode(interruptPins[i], INPUT);
  }

  int actuatorPins[] = {ACB_DRIVE_OPEN, ACB_DRIVE_CLOSE, INTERLOCK_UTILITY_BREAKER_TO_STAB_CLOSE, INTERLOCK_UTILITY_BREAKER_TO_STAB_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_OPEN, GEN_GRID_DISRTIBUTION_SIDE_BREAKER_CLOSE, INTERLOCK_GENERATOR_BREAKER_TO_STAB_OPEN, INTERLOCK_GENERATOR_BREAKER_TO_STAB_CLOSE};
  for (int i = 0;  i < numberOfIteration; i++) 
  {
    pinMode(actuatorPins[i], OUTPUT);
    digitalWrite(actuatorPins[i], HIGH);
  }
  node01Task();

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
  if (client.connect("Node01")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node01 connected");
    // ... and resubscribe
    client.subscribe("node01", 1);
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
  if ((Value == "C.ACB")){ 
     activateAcbBreaker.enable();
  }else if((Value == "C.IUTIL")){
     activateInterlockUtilityBreakerToStab.enable();
  }else if((Value == "E.IGEN")){
     activateInterlockGenBreakerToStab.enable();      
  }else if((Value == "C.GDSB")){
     activateGenGridDistributionSideBreaker.enable();
  }else if((Value == "RST")){  // sensor = CHGPW msg = 1 this means change of power source to utility. This implies reseting of all gen and utility actuators
     restAllLoadBreakers.enable();  
  }else if((Value == "ggsb")){  // sensor = CHGPW msg = 1 this means change of power source to utility. This implies reseting of all gen and utility actuators
     deactivateGenGridDistributionSideBreaker.enable();  
  }
}


void node01Task(){
    userScheduler.addTask(checkAcbAuxState);
    checkAcbAuxState.enable();
      
    userScheduler.addTask(checkPowerAvailableOnUtilityGridState);
    checkPowerAvailableOnUtilityGridState.enable();
    
    userScheduler.addTask(checkInterlockUtilityBreakerToStabAuxState);
    checkInterlockUtilityBreakerToStabAuxState.enable();
    
    userScheduler.addTask(checkGenGridDistributionSideBreakerAuxState);
    checkGenGridDistributionSideBreakerAuxState.enable();
    
    userScheduler.addTask(checkInterlockGenBreakerToStabAuxState);
    checkInterlockGenBreakerToStabAuxState.enable();
    
    userScheduler.addTask(deactivateAcbBreaker);  
    
    userScheduler.addTask(activateAcbBreaker); 
    
    userScheduler.addTask(activateInterlockUtilityBreakerToStab);  
     
    userScheduler.addTask(deactivateInterlockUtilityBreakerToStab);   
    
    userScheduler.addTask(activateGenGridDistributionSideBreaker);
    
    userScheduler.addTask(deactivateGenGridDistributionSideBreaker);
    
    userScheduler.addTask(activateInterlockGenBreakerToStab);
    
    userScheduler.addTask(deactivateInterlockGenBreakerToStab);

    userScheduler.addTask(restAllLoadBreakers);

    userScheduler.addTask(restAllUtilityLoadBreakers);

    userScheduler.addTask(restAllGenLoadBreakers);
 
}
