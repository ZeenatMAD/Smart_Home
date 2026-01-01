//ONLY DUSTBIN CODE 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// -------- LCD --------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- DUSTBIN --------
Servo lidServo;

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

bool isOpen = false;
unsigned long lastSeenTime = 0;

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 20000);
  int dist = duration * 0.034 / 2;

  if (dist == 0 || dist > 200)
    return 200;

  return dist;
}

void setup() {
  Serial.begin(9600);

  // LCD setup
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.print("Parking System");
  // lcd.setCursor(0, 1);
  // lcd.print("Smart Dustbin");

  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo
  lidServo.attach(7);
  lidServo.write(0);  // closed
}

void loop() {
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  // Object detected
  if (distance < 5) {
    if (!isOpen) {
      lidServo.write(90); // open lid
      isOpen = true;
    }
    lastSeenTime = millis();
  }

  // Auto close after delay
  if (isOpen && (millis() - lastSeenTime > 1200)) {
    lidServo.write(0); // close lid
    isOpen = false;
  }

  delay(50);
}
