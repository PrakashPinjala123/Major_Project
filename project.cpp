#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include<Servo.h>
int btn=A0;
int  buz=5;
int iij;
int m1=4;
int m2=7;
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

SoftwareSerial mySerial(2, 3);
#include <TinyGPS.h>
TinyGPS gps;
float flat=0, flon=0;
void read_gps()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }

  if (newData)
  {
    
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

  }
}



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t operation;
uint8_t id;

void detectFingerprintScanner(){
  if (finger.verifyPassword()) {
    delay(10);// Serial.println("Fingerprint sensor detected!");
  } else {
     delay(10);//Serial.println("Did not find fingerprint sensor :(");
     lcd.clear();
     lcd.print("FINGERPRINT ERROR");
     delay(2000);
    if (1) { delay(1); }
  }  
   delay(10);//Serial.println("");
   delay(10);//Serial.println("");
}

void verifyScannerParameters(){
  delay(10);//
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);

pinMode(btn,INPUT_PULLUP);
pinMode(buz,OUTPUT);
pinMode(m1,OUTPUT);
pinMode(m2,OUTPUT);
lcd.print("   WELCOME");

  digitalWrite(m1,1);
  digitalWrite(m2,0);
  while (!Serial);
  delay(100);
   delay(10);//Serial.println("Welcome to EE Securities");
  finger.begin(57600);
 
  detectFingerprintScanner();
  verifyScannerParameters();

if(digitalRead(btn)==0)
{
delay(1000);
  enrollFingerprint();
}
}

uint8_t readUserInput(void) {

   uint8_t num=0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  
  return num;
}

uint8_t getFingerprintEnroll() {
//Scan for First Time
int p = -1;
 delay(10);//Serial.print("Waiting for valid finger to enroll as #"); 
 delay(10);//Serial.println(id);
while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
       delay(10);//Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      delay(10);// Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
       delay(10);//Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
       delay(10);//Serial.println("Imaging error");
      break;
    default:
       delay(10);//Serial.println("Unknown error");
      break;
    }
  }

// OK success!
p = finger.image2Tz(1);
switch (p) {
    case FINGERPRINT_OK:
       delay(10);//Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
       delay(10);//Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
       delay(10);//Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
       delay(10);//Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
       delay(10);//Serial.println("Could not find fingerprint features");
      return p;
    default:
       delay(10);//Serial.println("Unknown error");
      return p;
}
 delay(10);//Serial.println("Remove finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {p = finger.getImage();}
 delay(10);//Serial.println("");
 delay(10);//Serial.print("ID "); Serial.println(id);
p = -1;

//Scan for Second Time
 delay(10);//Serial.println("Place same finger again");
while (p != FINGERPRINT_OK) {
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
       delay(10);//Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
       delay(10);//Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
       delay(10);//Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
       delay(10);//Serial.println("Imaging error");
      break;
    default:
       delay(10);//Serial.println("Unknown error");
      break;
    }
  }
p = finger.image2Tz(2);
switch (p) {
    case FINGERPRINT_OK:
       delay(10);//Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
       delay(10);//Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
       delay(10);//Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
       delay(10);//Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
       delay(10);//Serial.println("Could not find fingerprint features");
      return p;
    default:
       delay(10);//Serial.println("Unknown error");
      return p;
  }
 delay(10);//Serial.println("");

//Creating Model 
 delay(10);//Serial.print("Creating model for #");  Serial.println(id);
p = finger.createModel();
  if (p == FINGERPRINT_OK) {
     delay(10);//Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     delay(10);//Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
     delay(10);//Serial.println("Fingerprints did not match");
    return p;
  } else {
     delay(10);//Serial.println("Unknown error");
    return p;
  }

//Storing Model
 delay(10);//Serial.print("ID "); Serial.println(id);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
 delay(10);//Serial.println("Congratulations! Fingerprint is successfully enrolled.");
 lcd.clear();
 lcd.print("Success");
 delay(1500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     delay(10);//Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
     delay(10);//Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
     delay(10);//Serial.println("Error writing to flash");
    return p;
  } else {
     delay(10);//Serial.println("Unknown error");
    return p;
  }

 delay(10);//Serial.println("");
 delay(10);//Serial.println("");
return true;
}

void enrollFingerprint(){
//Serial.println("Ready to enroll a fingerprint!");
while(iij==0)
{
  lcd.clear();
  lcd.print("ENROL..");
Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
id = readUserInput();
if(id==0)
{
  iij=1;
  goto xx;
}
if (id == 0) {return;}
 delay(10);//Serial.print("Enrolling ID #");
 delay(10);//Serial.println(id);
while (! getFingerprintEnroll());
}
xx:
delay(1);
}


void deleteFingerprint(){
 delay(10);//Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
uint8_t id = readUserInput();
if (id == 0) {return;}
 delay(10);//Serial.print("Deleting ID #");
 delay(10);//Serial.println(id);
uint8_t p = -1;
p = finger.deleteModel(id);
if (p == FINGERPRINT_OK) {
     delay(10);//Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     delay(10);//Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    delay(10);// Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    delay(10);// Serial.println("Error writing to flash");
  } else {
     delay(10);//Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }
 delay(10);//Serial.println("");
 delay(10);//Serial.println("");
return p;
}

void deleteDatabase(){
 delay(10);//Serial.println("Do you want to delete all fingerprint templates!");
 delay(10);//Serial.println("Press 'Y' key to continue");
while (1) {
    if (Serial.available() && (Serial.read() == 'Y')) {
      break;
    }
  }
finger.emptyDatabase();
//Serial.println("Now the database is empty.)");
//Serial.println("");
//Serial.println("");
}

void loop() {
  //read_gps();
  lcd.setCursor(0,0);
  lcd.print("                 ");
  lcd.setCursor(0,0);
  lcd.print("SCAN THUMB..");

   lcd.setCursor(0,1);
  lcd.print("                 ");
  //lcd.setCursor(0,1);
  //lcd.print(String(flat,4)+" " +String(flon,4));
  

uint8_t p = finger.getImage();

if (p == FINGERPRINT_NOFINGER)  delay(10); 
else if (p == FINGERPRINT_OK) {
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
{

 lcd.setCursor(0,0);
  lcd.print("                 ");
  lcd.setCursor(0,0);
  lcd.print("UNAUTHORIZED..");
 digitalWrite(buz,1);
  Serial.print("2916734,G175Z0NQ27Z5TL40,2916736,8L0ZS3CTVU5QZ6JR,SRC 24G,src@internet,"+String(1)+ ","+String(flat,6)+ ","+String(flon,1)+",0\r\n");
  digitalWrite(m1,1);
  
delay(1000);
digitalWrite(buz,0);

  }
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  
  {
  
  lcd.setCursor(0,0);
  lcd.print("                 ");
  lcd.setCursor(0,0);
  lcd.print("UNAUTHORIZED..");
    digitalWrite(m1,1);
  digitalWrite(m2,0);
  digitalWrite(buz,1);
 Serial.print("2916734,G175Z0NQ27Z5TL40,2916736,8L0ZS3CTVU5QZ6JR,SRC 24G,src@internet,"+String(0)+ ","+String(flat,6)+ ","+String(flon,1)+",0\r\n");

delay(1000);
digitalWrite(buz,0);

  }
 // Serial.print("Welcome! Your ID is ");
  //Serial.print(finger.fingerID);
  else
  {
  
  //Serial.print("2");
  lcd.setCursor(0,0);
  lcd.print("                 ");
  lcd.setCursor(0,0);
// digitalWrite(buz,1);
  lcd.print("Authorized:"+String(finger.fingerID));
  digitalWrite(m1,0);
  digitalWrite(m2,0);
   Serial.print("2916734,G175Z0NQ27Z5TL40,2916736,8L0ZS3CTVU5QZ6JR,SRC 24G,src@internet,"+String(finger.fingerID)+ ","+String(flat,6)+ ","+String(flon,1)+",0\r\n");

  delay(300);
    digitalWrite(buz,0);
  }
  
  delay(1000);
  
  return;  
}

if(Serial.available())
{
  int x=Serial.read();
  if(x=='1')
  {
     digitalWrite(m1,0);
  }

  if(x=='2')
  {
     digitalWrite(m1,1);
  }
}
}
