#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define RXPin 3
#define TXPin 4
#define GPSBaud 9600
#define PIN 8
#define STRIPSIZE 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

double prevKnots = 0;

void setup() {
    ss.begin(GPSBaud);
    strip.begin();
    strip.setBrightness(100);  // Lower brightness and save eyeballs!
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    while (ss.available() > 0) {
        if (gps.encode(ss.read()) && gps.speed.isUpdated()) {
            int knots = (int)(0.5 * (prevKnots + gps.speed.knots()));
            for(int i = 0; i < strip.numPixels(); i++) {
                if (i <= knots) {
                    strip.setPixelColor(i, strip.Color(64, 64, 64));
                } else {
                    strip.setPixelColor(i, strip.Color(0, 0, 0));
                }
            }
            strip.show();
        }
    }
}

