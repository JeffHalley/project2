#include "arduino_secrets.h"

// NewPing - Version: Latest
#include <NewPing.h>

#include <NewPing.h>

#include "thingProperties.h"

WiFiClient client;
const char kHostname[] = "88.80.186.240";


int trigPin = 9;
int echoPin = 8;
int ledPin = 3;
int safePin = 2;
float distance;
int maxValue = 400;
boolean sendEmail;
bool lastState = false;


#define MAX_DISTANCE 400

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  delay(1500);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(ledPin, OUTPUT);
  pinMode(safePin, OUTPUT);
}

void loop() {
  ArduinoCloud.update();
  Serial.print("distance: ");
  Serial.println(" cm");
  distance = sonar.ping_cm(400);
  /*
    distanceMeassured = distance;
  */
  Serial.println(distance);
  stateChange();
  delay(50);
  blink();


}


void blink() {
  if (distance < 50) {

    digitalWrite(safePin, LOW);
    digitalWrite(ledPin, HIGH);

  } else if (distance >= 50 && distance < 100) {

    digitalWrite(safePin, LOW);
    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(ledPin, LOW);
    delay(150);

  } else if (distance >= 100 && distance < 200) {

    digitalWrite(safePin, LOW);
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);

  } else if (distance >= 200 && distance < 300) {

    digitalWrite(safePin, LOW);
    digitalWrite(ledPin, HIGH);
    delay(350);
    digitalWrite(ledPin, LOW);
    delay(350);

  } else if (distance >= 300 && distance <= maxValue) {

    digitalWrite(safePin, LOW);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  } else if ( distance > maxValue){
    digitalWrite(safePin, HIGH);
    digitalWrite(ledPin, LOW);
  }
}


void stateChange() {
  
  // Check the current distance and set the flag accordingly
  if (distance <= 50 && !dangerState) {
    dangerState = true;
    Serial.println("email sent ----------- DANGER DANGER DANGER");
      Serial.println("danger state activated");
      Serial.println("send email now");
      iftttSend();
    
  } else if (distance > 50 && dangerState) {
    dangerState = false;
    Serial.println("safe");
  }
  
  lastState = dangerState;
}


void iftttSend() {
  Serial.println("Alarm triggered !");
  // Connexion au serveur IFTTT
  Serial.println("Starting connection to server...");
  if (client.connect(kHostname,3000)) {
    Serial.println("Connected to server IFTTT, ready to trigger alarm...");
    // Make a HTTP request:
    client.println("GET /prox/message/cr3inEiLwFze8bjjgFXzVD HTTP/1.1"); // Replace EVENT_NAME and KEY with your own IFTTT EVENT NAME and IFTTT key
    client.println();
    Serial.println("IFTTT alarm triggered !");
  }
  else {
    Serial.println("Connection at IFTTT failed");
  }
}

