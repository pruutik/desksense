#include <Servo.h>

// Define pins
const int trigPin = 12;
const int echoPin = 13;
const int ledPin = 7;
const int servoPin = 11; // Servo
const int basherServoPin = 9;

// Create servo object
Servo myServo;
Servo basherServo;

// Constants
const int proximityThreshold = 90; // Distance threshold in centimeters
const int lightThreshold = 50;

// Variables
bool servoMoved = false; // Flag to track if the servo has already moved
int bookCloseDamper = 0; // timeout so that book doesn't close immediately
int smackDamper = 0; // same as above but for smacker

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Attach servo
  myServo.attach(servoPin);
  basherServo.attach(basherServoPin);

  // Start with servo at 180 degrees
  myServo.write(180);
  basherServo.write(180);

  // Initialize Serial for debugging
  Serial.begin(9600);
}

void loop() {
  // Measure distance
  long distance = getDistance();

  // Measure light sensor
  int lightValue = analogRead(A0);

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  // Print light to Serial Monitor
  Serial.print("Light: ");
  Serial.println(lightValue);

  // Check if the distance is below the threshold
  if (distance <= proximityThreshold) {
    bookCloseDamper = 0;
    // Turn on LED
    digitalWrite(ledPin, HIGH);

    myServo.write(30);

    // Move servo to 0 degrees once
    // if (!servoMoved) {
    //   myServo.write(0);
    //   servoMoved = true; // Ensure servo moves only once
  
    // }
  } else {
    bookCloseDamper += 1;
    // Turn off LED
    digitalWrite(ledPin, LOW);
    if(bookCloseDamper > 45) myServo.write(180);
  }

  if (lightValue > lightThreshold) {
    smackDamper = 0;
    basherServo.write(180);
  }else{
    smackDamper += 1;
    if(smackDamper > 40) basherServo.write(90);
  }

  // Small delay to avoid flooding the Serial Monitor
  delay(100);
}

// Function to get distance from the ultrasonic sensor
long getDistance() {
  // Send a 10-microsecond pulse to the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin and calculate distance
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert to centimeters

  return distance;
}