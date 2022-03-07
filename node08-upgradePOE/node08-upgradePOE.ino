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
#include <TrueRMS.h>

#include <ETH.h>


#define POWER_AVAILABLE_ACB_INCOMER 2
#define OVER_AND_UNDER_VOLTAGE 4
#define PHASE_SEQUENCE 14

#define UTILITY_RED_PHASE_VOLTAGE 16
#define UTILITY_BLUE_PHASE_VOLTAGE 15
#define UTILITY_YELLOW_PHASE_VOLTAGE 12

#define DEBUG 1

long lastReconnectAttempt = 0;

// CHANGE THESE SETTINGS FOR YOUR APPLICATION
const char* mqttServerIp = "192.168.2.44"; // IP address of the MQTT broker
const short mqttServerPort = 11883; // IP port of the MQTT broker
const char* mqttClientName = "node08";
const char* mqttUsername = NULL;
const char* mqttPassword = NULL;


IPAddress local_ip(192, 168, 2, 47);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(192, 168, 1, 2);

Scheduler userScheduler;

bool initializationFlag;


enum State {
  DISABLE = 0,
  ENABLE = 1  
 };
 
State currentState;

#define LPERIOD 2000    // loop period time in us. In this case 2.0ms
#define RMS_WINDOW 40   // rms window of 40 samples, means 4 periods @50Hz
//#define RMS_WINDOW 25   // rms window of 25 samples, means 3 periods @60Hz
//#define RMS_WINDOW 50   // rms window of 50 samples, means 6 periods @60Hz


Power redPhaseVoltage;  // create an instance of Power
Power bluePhaseVoltage;
Power yellowPhaseVoltage;

int cnt=0;
float acVoltRange = 700; // peak-to-peak voltage scaled down to 0-5V is 700V (=700/2*sqrt(2) = 247.5Vrms max).
float acCurrRange = 5; // peak-to-peak current scaled down to 0-5V is 5A (=5/2*sqrt(2) = 1.77Arms max).

unsigned long nextLoop;
int acVolt;
int acCurr;

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

void callback(char* topic, byte* payload, unsigned int length) {
  String inValue;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  char payloadString[length+1];
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0';
  inValue = payloadString;
  Serial.println();  
  reciever(inValue);
}

boolean reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqttClientName)) {
    //if (mqttClient.connect(mqttClientName, mqttUsername, mqttPassword) { // if credentials is nedded
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","Node08 connected");
      // ... and resubscribe
//      mqttClient.subscribe("node10", 1);


    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.setSleep(false);
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  
 int pinDeclare[] = {UTILITY_RED_PHASE_VOLTAGE, UTILITY_BLUE_PHASE_VOLTAGE, UTILITY_YELLOW_PHASE_VOLTAGE};
  for (int i = 0; i < 3; i++){
    pinMode(pinDeclare[i], INPUT); 
  }
  pinMode(POWER_AVAILABLE_ACB_INCOMER, INPUT);
  pinMode(PHASE_SEQUENCE, INPUT);
  pinMode(OVER_AND_UNDER_VOLTAGE, INPUT);
  
  analogReadResolution(12);
  // configure for automatic base-line restoration and continuous scan mode:
  redPhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  bluePhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  yellowPhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  
  redPhaseVoltage.start(); //start measuring
  bluePhaseVoltage.start(); //start measuring
  yellowPhaseVoltage.start(); //start measuring
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
  
  initializationFlag = false;
  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  ETH.config(local_ip, gateway, subnet, dns);

  lastReconnectAttempt = 0;
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
      threePhaseHysterisis();
//      userScheduler.execute();
    }

  }

}

char reciever(String Value) {
   Serial.println("From callback: " +Value);

  if ((Value == "E.AUTLC")){ 
//     engageAllUtilityContactors.enable(); 
  }
}
