
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// Define the pins for the ultrasonic sensor
const int trigPin = D1;
const int echoPin = D2;

// Define the threshold distance for oil level
// const int OilLevelThreshold = 10; // Adjust this value according to your setup
const int OilLevelThreshold = 8;

char ssid[] = "network name";    // Your Wi-Fi network name
char password[] = "network password";  // Your Wi-Fi network password
unsigned long channelID = channel ID;  // Your ThingSpeak channel ID
const char *writeAPIKey = "API key"; // Your ThingSpeak write API key

WiFiClient client;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);  // Initialize ThingSpeak library

  // Define the pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger the ultrasonic sensor to send out a ping
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance based on the speed of sound
  int distance = duration * 0.034 / 2;

  // Print the measured distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if the oil level is below the threshold
  // if (distance < OilLevelThreshold) {
  //   Serial.println("Oil level is low!");
  //   // Do something when the oil level is low
  // }
    if (distance <= 3) {
    Serial.println("Oil level is full!");
  } else if (distance < OilLevelThreshold) {
    Serial.println("Oil level is low!");
  } else  {
    Serial.println("Oil level is empty!");
  }
  // Read the input on analog pin 0
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5 / 1024.0); // Convert the analog reading to voltage
  float percentage = voltage * (100.0 / 5); // Convert voltage to percentage

  // Serial.print("Voltage = ");
  // Serial.println(voltage); // Print the voltage value

  Serial.print("Percentage = ");
  Serial.println(percentage); // Print the percentage value

  if (percentage >= 80.0) {
    Serial.println("Condition: Very Clean");
  } else if (percentage >= 60.0) {
    Serial.println("Condition: Clean");
  } else if (percentage >= 40.0) {
    Serial.println("Condition: Moderate");
  } else if (percentage >= 20.0) {
    Serial.println("Condition: Dirty");
  } else {
    Serial.println("Condition: Very Dirty");
  }

  // Send data to ThingSpeak
  ThingSpeak.setField(1, distance);
  ThingSpeak.setField(2, percentage);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(response);
  }

  delay(2000);
}
