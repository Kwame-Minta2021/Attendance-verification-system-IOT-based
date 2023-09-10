
#include <Adafruit_Fingerprint.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <SD.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from Arduino (WHITE wire)
// comment these two lines if using hardware serial
const char* NAME;
const char* ID;

String Event_Name = "attendance";
String Key = "medv3dMvYBPLuC82wmM9uEUNxEdLj5U16VRVmmLyAfk";

// Replace with your unique IFTTT URL resource
String resource = "/trigger/" + Event_Name + "/with/key/" + Key;

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Replace with your SSID and Password
const char* ssid = "NEWTON 6019";
const char* password = "37W965d+";

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

#define RST_PIN         22          // Configurable, see typical pin layout above
#define SS_PIN          21         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

File dataFile;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for a valid finger...");

  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: ");
  Serial.print(millis());
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize SD card
  
  if (SD.begin()) {
    Serial.println("SD card initialized!");
  } else {
    Serial.println("SD card initialization failed!");
    while (1);
  }

  // Open file for writing
  dataFile = SD.open("attendance.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening file!");
    while (1);
  }

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  
  lcd.print("Welcome!");

  lcd.setCursor(0, 1);
  lcd.print("Place RFID card");
}

void loop() {

  if (finger.fingerID == 1) {
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    NAME = "Sachin";
    ID = "1";
    if (finger.confidence >= 60) {
      Serial.print("Attendance marked for "); Serial.println(NAME);
      saveToSDCard(NAME, ID);
      makeIFTTTRequest();
      lcd.clear();
      lcd.print("Welcome, ");
      lcd.setCursor(0, 1);
      lcd.print(NAME);
      // digital write - open the door
    }
  }

  if (finger.fingerID == 11 ) {
    Serial.print("!!--");
    Serial.println(finger.fingerID);
    digitalWrite(2, LOW);
    NAME = "JOY";
    ID = "11";
    if (finger.confidence >= 60) {
      Serial.print("Attendance marked for "); Serial.println(NAME);
      saveToSDCard(NAME, ID);
      makeIFTTTRequest();
      lcd.clear();
      lcd.print("Welcome, ");
      lcd.setCursor(0, 1);
      lcd.print(NAME);
      // digital write - open the door
    }
  }

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String rfidUID = getRFIDUID();
    Serial.print("RFID UID: ");
    Serial.println(rfidUID);

    if (rfidUID == "04 3D E6 7D") {
      NAME = "Alice";
      ID = "21";
      Serial.print("RFID card matched for "); Serial.println(NAME);
      saveToSDCard(NAME, ID);
      makeIFTTTRequest();
      lcd.clear();
      lcd.print("Welcome, ");
      lcd.setCursor(0, 1);
      lcd.print(NAME);
      // digital write - open the door
    }
    // Add more RFID card checks here

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
  
  finger.fingerID = 0;
}

String getRFIDUID() {
  String rfidUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      rfidUID += "0";
    }
    rfidUID += String(mfrc522.uid.uidByte[i], HEX);
    rfidUID += " ";
  }
  rfidUID.trim();
  return rfidUID;
}

void saveToSDCard(const char* name, const char* id) {
  if (dataFile) {
    dataFile.print("Name: ");
    dataFile.println(name);
    dataFile.print("ID: ");
    dataFile.println(id);
    dataFile.println();
    dataFile.close();
  } else {
    Serial.println("Error opening file for");
 
  }
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);

  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + NAME + "\",\"value2\":\"" + ID + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 5 * 10; // 5 seconds
  while (!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}
