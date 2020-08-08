#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "SSID GOES HERE"  // Update this
#define STAPSK  "PASSWORD GOES HERE" // Update this
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "api.openweathermap.org";
const int httpsPort = 443;

int led1 = 13; // D7: white #1
int led2 = 12; // D6: red #2
int led3 = 4;  // D2: yellow #3
int led4 = 5;  // D1: green #4
int led5 = 14; // D5: blue #5

// called once on startup
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  // Turn off LEDs 1-5
  lightsOff();

  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);

  // Run light test
  lightTest();
}

void loop() {
  // Let's request the temperature, but no more than once every 5 minutes.
  Serial.println("Requesting Temperature!");
  getTemp();

  // and wait at least 5 minutes before doing it again
  delay(300000);
}

void lightTest() {
  digitalWrite(led5, HIGH);
  delay(150);
  digitalWrite(led5, LOW);
  digitalWrite(led4, HIGH);
  delay(150);
  digitalWrite(led4, LOW);
  digitalWrite(led3, HIGH);
  delay(150);
  digitalWrite(led3, LOW);
  digitalWrite(led2, HIGH);
  delay(150);
  digitalWrite(led2, LOW);
  digitalWrite(led1, HIGH);
  delay(150);

  lightsOn();

  // We'll leave it on for 1 second...
  delay(1000);

  // Then we'll turn it off...
  lightsOff();
}

void lightsOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}

void lightsOn() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
}

void getTemp() {
  WiFiClientSecure client;
  client.setInsecure(); // This allows us to connect to https without actual security
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/data/2.5/weather?appid=62f6f8ad27ed4ad5ff31825d92efe431&id=5331835&units=imperial";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: DEFCON\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String json = client.readStringUntil('\n');
    if (json == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String json = client.readStringUntil('\n');
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(json);
  Serial.println("==========");

  // Allocate JsonBuffer
  // Used arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_ARRAY_SIZE(1) + 2 * JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(14);
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  deserializeJson(doc, json);

  // Extract raw value
  float temperature = doc["main"]["temp"]; // e.g. 57.87
  int roundedTemp = round(temperature); // e.g. 58

  Serial.println("Temp: ");
  Serial.println(roundedTemp);

  // Set temperature value on DEFCON
  setTemp(roundedTemp);
}

// This function will get called when weather data comes in
void setTemp(int temp) {
  Serial.println("Setting temp...");
  Serial.println(temp);

  if (temp <= 69) { // Light up #5 if in the 60s
    lightsOff();
    digitalWrite(led5, HIGH);
  } else if (temp <= 79) { // Light up #4 if in the 70s
    lightsOff();
    digitalWrite(led4, HIGH);
  } else if (temp <= 89) { // Light up #3 if in the 80s
    lightsOff();
    digitalWrite(led3, HIGH);
  } else if (temp <= 99) { // Light up #2 if in the 90s
    lightsOff();
    digitalWrite(led2, HIGH);
  } else if (temp >= 100) { // Light up #1 if in the 100s
    lightsOff();
    digitalWrite(led1, HIGH);
  }
}
