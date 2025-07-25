#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID Module Pins
#define SS_PIN 10   // SDA pin (SDA)
#define RST_PIN 9   // Reset pin (RST)
MFRC522 mfrc522(SS_PIN, RST_PIN); // RFID instance

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27, 16 columns, 2 rows

// ✅ Authorized RFID Card UID
byte authorizedUID[] = {0x2B, 0x62, 0xC0, 0x01};  // "2B 62 C0 01"
const int uidLength = 4; // Length of the UID (4 bytes)

// Buzzer Pin
#define BUZZER_PIN 7  

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is OFF at start

  SPI.begin();       // Initialize SPI
  mfrc522.PCD_Init(); // Initialize RFID module

  lcd.setCursor(0, 0);
  lcd.print(" RFID Access Sys ");
  lcd.setCursor(0, 1);
  lcd.print("Scan RFID Card");
}

void loop() {
  // Check if an RFID card is present
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Card UID:");

  Serial.print("Card UID: ");

  // Read UID and display it
  bool isAuthorized = true;
  for (byte i = 0; i < uidLength; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");

    lcd.setCursor(i * 3, 1);
    lcd.print(mfrc522.uid.uidByte[i], HEX);
    lcd.print(" ");

    // Compare scanned UID with the authorized UID
    if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
      isAuthorized = false;  // If any byte does not match, deny access
    }
  }
  Serial.println();

  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);

  if (isAuthorized) {
    Serial.println("Access Granted!");
    lcd.print("Access Granted");
    digitalWrite(BUZZER_PIN, LOW);  // Turn OFF buzzer
  } else {
    Serial.println("Access Denied!");
    lcd.print("Access Denied");
    digitalWrite(BUZZER_PIN, HIGH); // Turn ON buzzer
    delay(2000);  // Keep buzzer ON for 2 seconds
    digitalWrite(BUZZER_PIN, LOW);  // Turn OFF buzzer
  }

  delay(3000); // Show result for 3 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");
  mfrc522.PICC_HaltA(); // Stop reading
}
