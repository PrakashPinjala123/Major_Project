#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
const int btnPin = A0;
const int buzzer = 5;
const int motor1 = 4;
const int motor2 = 7;
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
SoftwareSerial fingerSerial(2, 3);
Adafruit_Fingerprint finger(&fingerSerial);
uint8_t id, enrollStep;
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  lcd.print("WELCOME");
  while (!Serial);
  finger.begin(57600);
  if (!finger.verifyPassword()) {
    lcd.clear(); lcd.print("FINGERPRINT ERR"); delay(2000);
  }
  if (digitalRead(btnPin) == LOW) {
    delay(1000);
    enrollFingerprint();
  }
}
uint8_t readNumber() {
  while (!Serial.available());
  return Serial.parseInt();
}
bool capturePrint(uint8_t slot) {
  uint8_t p;
 while ((p = finger.getImage()) != FINGERPRINT_OK);
  if (finger.image2Tz(slot) != FINGERPRINT_OK) return false;
  return true;
}
void enrollFingerprint() {
  lcd.clear(); lcd.print("ENROLLING...");
  Serial.println("Enter ID (1–127):");
  id = readNumber();
  if (!id) return;
  // first scan
  while (!capturePrint(1));
  delay(1000); while (finger.getImage() != FINGERPRINT_NOFINGER);
  // second scan
  while (!capturePrint(2));
  if (finger.createModel() != FINGERPRINT_OK) return;
  if (finger.storeModel(id) == FINGERPRINT_OK) {
    lcd.clear(); lcd.print("ENROLL OK"); delay(1500);
  }
}
void loop() {
  lcd.setCursor(0,0); lcd.print("SCAN THUMB...");
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK) {
    if (finger.image2Tz() != FINGERPRINT_OK) {
      lcd.clear(); lcd.print("UNAUTH"); buzz();
      report(1);
    } else if (finger.fingerFastSearch() != FINGERPRINT_OK) {
      lcd.clear(); lcd.print("UNAUTH"); buzz();
      report(0);
    } else {
      lcd.clear(); lcd.print("AUTHORIZED ");
      lcd.print(finger.fingerID);
      digitalWrite(motor1, LOW);
      report(finger.fingerID);
    }
    delay(1000);
  }
  // optional serial control of motor1
  if (Serial.available()) {
    char c = Serial.read();
    digitalWrite(motor1, c=='1' ? LOW : HIGH);
  }
}
void buzz() {
  digitalWrite(buzzer, HIGH); delay(500);
  digitalWrite(buzzer, LOW);
}
void report(uint8_t code) {
  Serial.print("DEVICEID,");
  Serial.print(code);
  Serial.println();
}
