
#include <Arduino.h>

// Motor pins (example) - change as per your wiring
const int ENA = 5; // PWM left
const int IN1 = 6;
const int IN2 = 7;
const int ENB = 9; // PWM right
const int IN3 = 8;
const int IN4 = 10;

// Ultrasonic pins - change if needed
const int TRIG_PIN = 11;
const int ECHO_PIN = 12;

// Behavior parameters
const int CRUISE_PWM = 200;
const int AVOID_DISTANCE_CM = 25;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  Serial.println("Surveillance Robot started");
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}

void forwardMotors(int pwm) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, pwm);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void reverseMotors(int pwm, unsigned long ms) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, pwm);
  delay(ms);
}

void turnRight(int pwm, unsigned long ms) {
  // left forward, right backward for sharper turn
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, pwm);
  delay(ms);
}

void loop() {
  float dist = readDistanceCm();
  if (dist > 0 && dist < AVOID_DISTANCE_CM) {
    Serial.print("Obstacle at ");
    Serial.print(dist);
    Serial.println(" cm - avoiding");
    stopMotors();
    delay(100);
    reverseMotors(200, 300);
    turnRight(200, 350);
  } else {
    forwardMotors(CRUISE_PWM);
  }
  delay(50);
}
