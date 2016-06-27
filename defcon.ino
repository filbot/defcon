int led1 = D4; //white #1
int led2 = D3; //red #2
int led3 = D2; // yellow #3
int led4 = D1; // green #4
int led5 = D0; // blue #5

// This function will get called when weather data comes in
void temperatureData(const char *event, const char *data) {
  double temp = strtod(data,NULL);

  Serial.println(temp);

  if (temp <= 69.9) {
    lightsOff();
    digitalWrite(led5, HIGH);
  } else if (temp <= 79.9) {
    lightsOff();
    digitalWrite(led4, HIGH);
  } else if (temp <= 89.9) {
    lightsOff();
    digitalWrite(led3, HIGH);
  } else if (temp <= 99.9) {
    lightsOff();
    digitalWrite(led2, HIGH);
  } else {
    lightsOff();
    digitalWrite(led1, HIGH);
  }
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

  delay(5000);

  // Run light test
  lightTest();

  // Listening for the hook response
  Particle.subscribe("hook-response/get_temp", temperatureData , MY_DEVICES);
}

void loop() {
  // Let's request the temperature, but no more than once every 5 minutes.
  Serial.println("Requesting Temperature!");

  // publish the event that will trigger our Webhook
  bool success;
  success = Particle.publish("get_temp");
  if (!success) {
    // get here if event publish did not work
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
  }

  // and wait at least 5 minutes before doing it again
  delay(300000);
}
