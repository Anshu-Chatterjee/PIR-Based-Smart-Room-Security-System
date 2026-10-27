#include <LiquidCrystal.h>

const int pirPin = 2;

const int lcdD4 = 5;
const int lcdD5 = 4;
const int lcdD6 = 3;
const int lcdD7 = 6;

const int redLED = 8;
const int greenLED = 9;

const int buzzer = 10;

const int lcdEnable = 11;
const int lcdRS = 12;

const int resetButton = 7;

LiquidCrystal lcd(
  lcdRS,
  lcdEnable,
  lcdD4,
  lcdD5,
  lcdD6,
  lcdD7
);

bool alarmActive = false;

unsigned long lastMotionTime = 0;

const unsigned long alarmDuration = 5000;

void setup() {

  pinMode(pirPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(resetButton, INPUT_PULLUP);

  Serial.begin(9600);

  lcd.begin(16, 2);

  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(buzzer, LOW);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART SECURITY");

  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");

  delay(2000);

  showSecure();
}

void loop() {

  int motionDetected = digitalRead(pirPin);

  if (digitalRead(resetButton) == LOW) {

    alarmActive = false;

    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(buzzer, LOW);

    showSecure();

    Serial.println("Alarm Reset");

    delay(300);
  }

  if (motionDetected == HIGH && !alarmActive) {

    alarmActive = true;

    lastMotionTime = millis();

    triggerAlarm();
  }

  if (alarmActive) {

    if (millis() - lastMotionTime >= alarmDuration) {

      alarmActive = false;

      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      digitalWrite(buzzer, LOW);

      showSecure();

      Serial.println("Room Secure");
    }
  }
}

void triggerAlarm() {

  digitalWrite(greenLED, LOW);

  digitalWrite(redLED, HIGH);

  digitalWrite(buzzer, HIGH);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("!! WARNING !!");

  lcd.setCursor(0, 1);
  lcd.print("INTRUSION!");

  Serial.println("--------------------");
  Serial.println("INTRUSION DETECTED!");
  Serial.println("--------------------");
}

void showSecure() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ROOM STATUS:");

  lcd.setCursor(0, 1);
  lcd.print("SECURE");
}