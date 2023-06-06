#define BLYNK_TEMPLATE_ID "TMPL3uibYZNoe"
#define BLYNK_DEVICE_NAME "LabAutomation"
#define BLYNK_AUTH_TOKEN "s2291g1WHa7xVg5Sz8MD1Kx2nJ_BfwpL"
#define BLYNK_PRINT Serial

#include<BlynkSimpleEsp8266.h>
#include<ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
WiFiClient client;

const char* ssid  = "HITAM_cloud";
const char* password  = "Hitam4_2023";

int sensor = D5; // Digital pin D5
int state = 0;
int relay = D1;
int relay2 = D2;
int relay3 = D3;
int relay4 = D4;

unsigned long previousmillis = 0;
const long interval = 30000;

int relaystat = LOW;
int relay2stat = LOW;
int relay3stat = LOW;
int relay4stat = LOW;
int automode = 0;

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
}

void setup() {
  Serial.begin(9600);

  pinMode(sensor, INPUT);   // declare sensor as input
  pinMode(relay, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  Blynk.begin(auth, ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.print("connected to ssid");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


void loop() {
  Blynk.run();
  if (!automode) {
    Serial.println("automode");
    unsigned long currentmillis = millis();
    if (currentmillis - previousmillis >= interval) {
      previousmillis = currentmillis;
      //place the code to be executed for every one minute
    }
      sense();
      turnon();
      turnoff();
    }
}

void sense() {
    state = digitalRead(sensor);
}

void turnon() {
  if (state == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(relay, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, HIGH);
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V3, HIGH);
    delay(30000);
  }
  
}

void turnoff() {
  if (state == LOW) {
    Serial.println("Motion absent!");
    digitalWrite(relay, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, LOW);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V3, LOW);
  }
}

BLYNK_WRITE(V4) {
  automode = param.asInt();
}

BLYNK_WRITE(V0) {
  Serial.println("manualmode-relay");
  if (automode) {
    relaystat = param.asInt();
    digitalWrite(relay, !relaystat);
    Serial.println("relaytog");
  }
}

BLYNK_WRITE(V1) {
  Serial.println("manualmode-relay2");
  if (automode) {
    relay2stat = param.asInt();
    digitalWrite(relay2, !relay2stat);
    Serial.println("relay2tog");
  }
}
BLYNK_WRITE(V2) {
  Serial.println("relay3");
  if (automode) {
    relay3stat = param.asInt();
    digitalWrite(relay3, !relay3stat);
    Serial.println("relay3tog");
  }
}

BLYNK_WRITE(V3) {
  Serial.println("manualmode-relay4");
  if (automode) {
    relay4stat = param.asInt();
    digitalWrite(relay4, !relay4stat);
    Serial.println("relay4tog");
  }
}
