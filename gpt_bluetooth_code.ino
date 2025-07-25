#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// GPS module on pins 4 (RX), 3 (TX)
SoftwareSerial gpsSerial(4, 3);

// Bluetooth module on pins 8 (RX), 9 (TX)
SoftwareSerial btSerial(8, 9);

TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);       
  gpsSerial.begin(9600);    
  btSerial.begin(9600);     

  Serial.println("System Initialized");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());

    if (gps.location.isUpdated()) {
      float lat = gps.location.lat();
      float lng = gps.location.lng();

      // Send to phone via Bluetooth
      btSerial.print("Latitude: ");
      btSerial.println(lat, 6);
      btSerial.print("Longitude: ");
      btSerial.println(lng, 6);

      // Also print on serial monitor
      Serial.print("Latitude: ");
      Serial.println(lat, 6);
      Serial.print("Longitude: ");
      Serial.println(lng, 6);
    }
  }
}
