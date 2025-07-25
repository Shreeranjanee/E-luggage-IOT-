#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// HX711 Pins
#define DT 2
#define SCK 3

// Buzzer Pin
#define BUZZER_PIN 7

// LCD config
LiquidCrystal_I2C lcd(0x27, 16, 2);

// HX711 setup
HX711 scale;
float calibration_factor = -7050.0; // Adjust based on your setup
float referenceWeight = 0.0;
bool referenceCaptured = false;

// Buzzer timer
bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 3000; // 3 seconds

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Buzzer off initially

  scale.begin(DT, SCK);
  scale.set_scale(calibration_factor);
  scale.tare();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weight Scale");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Weight...");
}

void loop() {
  float currentWeight = scale.get_units(5); // average of 5 readings

  // Set the reference weight once
  if (!referenceCaptured && currentWeight > 10) {
    referenceWeight = currentWeight;
    referenceCaptured = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ref Weight Set");
    lcd.setCursor(0, 1);
    lcd.print(referenceWeight, 1);
    lcd.print(" g");
    delay(2000);
    lcd.clear();
  }

  // Display current weight
  lcd.setCursor(0, 0);
  lcd.print("Weight:");
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear line
  lcd.setCursor(0, 1);
  lcd.print(currentWeight, 1);
  lcd.print(" g");

  // Print to Serial
  Serial.print("Current: ");
  Serial.print(currentWeight);
  Serial.print(" g, Ref: ");a
  Serial.println(referenceWeight);

  // Buzzer logic with timeout
  if (!buzzerActive && (currentWeight > 100)) {
    buzzerActive = true;
    buzzerStartTime = millis();
    digitalWrite(BUZZER_PIN, HIGH);
  }

  // Turn off buzzer after duration
  if (buzzerActive && (millis() - buzzerStartTime >= buzzerDuration)) {
    buzzerActive = false;
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);
}
