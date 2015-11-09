int led1 = D6;
int led2 = D5;
int led3 = D4;
int led4 = D3;
int led5 = D2;


// called once on startup
void setup() {
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);

    // Turn off LEDs 1-5
    lightsOff();
    
    Serial.begin(9600);
    
    // Run light test
    lightTest();

    // Lets listen for the hook response
    Serial.println("Requesting Temperature!");

    // Listening for the hook response
    Spark.subscribe("hook-response/get_temp", temperatureData, MY_DEVICES);
}


// called forever every 5 minutes
void loop() {
    // Let's request the weather, but no more than once every 5 minutes.
    Serial.println("Requesting Temperature!");

    // publish the event that will trigger our Webhook
    Spark.publish("get_temp");

    // and wait at least 5 minutes before doing it again
    delay(300000);
}

// This function will get called when weather data comes in
void temperatureData(const char *name, const char *data) {
    double temp = strtod(data,NULL);
    
    Serial.println(temp);
    
    if (temp < 60) {
        lightsOff();
        digitalWrite(led1, HIGH);
    } else if (temp <= 65) {
        lightsOff();
        digitalWrite(led2, HIGH);
    } else if (temp <= 69) {
        lightsOff();
        digitalWrite(led3, HIGH);
    } else if (temp <= 75) {
        lightsOff();
        digitalWrite(led4, HIGH);
    } else {
        lightsOff();
        digitalWrite(led5, HIGH);
    }
}

void lightTest() {
  // To blink the LED, first we'll turn it on...
  digitalWrite(led1, HIGH);
  delay(500);
  digitalWrite(led2, HIGH);
  delay(500);
  digitalWrite(led3, HIGH);
  delay(500);
  digitalWrite(led4, HIGH);
  delay(500);
  digitalWrite(led5, HIGH);

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