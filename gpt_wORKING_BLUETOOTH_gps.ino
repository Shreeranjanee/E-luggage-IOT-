#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(4, 3);  // GPS
SoftwareSerial btSerial(8, 9);   // Bluetooth

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

      // Send Maps link
      String mapsLink = "https://www.google.com/maps?q=" + String(lat, 6) + "," + String(lng, 6);
      btSerial.println(mapsLink);
      Serial.println(mapsLink); // for debugging

      delay(3000); // Avoid spamming
    }
  }
}
