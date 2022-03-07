
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>
#include <TrueRMS.h>

#define DEBUG 1

//interrupts
#define POWER_AVAILABLE_ACB_INCOMER 27
#define OVER_AND_UNDER_VOLTAGE 25
#define PHASE_SEQUENCE 26

#define UTILITY_RED_PHASE_VOLTAGE 32
#define UTILITY_BLUE_PHASE_VOLTAGE 33
#define UTILITY_YELLOW_PHASE_VOLTAGE 34

bool initializationFlag;


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
IPAddress local_IP(192, 168, 2, 47);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 2);

IPAddress subnet(255, 255, 252, 0);

WiFiClient espClient6;
PubSubClient client(espClient6);

Scheduler userScheduler; // to control your personal task

//prototypes
void utilityPowerAvailable();
Task utilityPowerAvailability(100, TASK_FOREVER, &utilityPowerAvailable);


#define LPERIOD 2000    // loop period time in us. In this case 2.0ms
#define ADC_VIN 35       // define the used ADC input channel for voltage
#define ADC_IIN 1       // define the used ADC input channel for current
//#define RMS_WINDOW 20   // rms window of 20 samples, means 2 periods @50Hz
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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);

  int pinDeclare[] = {UTILITY_RED_PHASE_VOLTAGE, UTILITY_BLUE_PHASE_VOLTAGE, UTILITY_YELLOW_PHASE_VOLTAGE};
  for (int i = 0; i < 3; i++){
    pinMode(pinDeclare[i], INPUT); 
  }
  pinMode(POWER_AVAILABLE_ACB_INCOMER, INPUT);
  pinMode(PHASE_SEQUENCE, INPUT);
  pinMode(OVER_AND_UNDER_VOLTAGE, INPUT);
  
  analogReadResolution(12);

  userScheduler.addTask(utilityPowerAvailability);
  utilityPowerAvailability.enable();
  
  // configure for automatic base-line restoration and continuous scan mode:
  redPhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  bluePhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  yellowPhaseVoltage.begin(acVoltRange, acCurrRange, RMS_WINDOW, ADC_10BIT, BLR_ON, CNT_SCAN);
  
  redPhaseVoltage.start(); //start measuring
  bluePhaseVoltage.start(); //start measuring
  yellowPhaseVoltage.start(); //start measuring
  
  nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
  
  initializationFlag = false;
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
  if (client.connect("Node08")) {
    // Once connected, publish an announcement...
    Serial.print("Attempting MQTT connection...");
    Serial.println("connected");
    client.publish("outTopic","Node08 connected");
    // ... and resubscribe
//    client.subscribe("utilAn", 1);
//    client.subscribe("rstTopic", 1);

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
      threePhaseHysterisis();
      userScheduler.execute();
  }
}

char reciever(String Value) {
   Serial.println("From callback: " +Value);

  if ((Value == "E.AUTLC")){ 
//     engageAllUtilityContactors.enable(); 
  }
}
