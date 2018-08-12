#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Wire.h>

#define RXPin 3
#define TXPin 4
#define GPSBaud 9600

// SH1106 OLED display
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

double prevKnots = 0;

/* float to string
 * f is the float to turn into a string
 * p is the precision (number of decimals)
 * return a string representation of the float.
 */
char *f2s(float f, int p) {
    char * pBuff;                        // use to remember which part of the buffer to use for dtostrf
    const int iSize = 10;                // number of bufffers, one for each float before wrapping around
    static char sBuff[iSize][6];         // space for six characters including NULL terminator for each float
    static int iCount = 0;               // keep a tab of next place in sBuff to use
    pBuff = sBuff[iCount];               // use this buffer
    if(iCount >= iSize -1) {             // check for wrap
        iCount = 0;                      // if wrapping start again and reset
    }
    else {
        iCount++;                        // advance the counter
    }
    return dtostrf(f, 0, p, pBuff);      // call the library function
}

/* To get the time from GPS, adjusted to Swedish time */
int getHour() {
  int month = gps.date.month();
  if (month < 10 && month > 3) {
      return gps.time.hour() + 2;
  }
  return gps.time.hour() + 1;
}

void setup() {
    ss.begin(GPSBaud);
    u8g2.begin();
    u8g2.setFont(u8g2_font_logisoso16_tn);   // set the target font
    u8g2.setFontMode(0);    // enable transparent mode, which is faster
    u8g2.firstPage();
    do {
        u8g2.drawStr(0, 63, "...");
    } while ( u8g2.nextPage() );
}

void loop() {
    while (ss.available() > 0) {
        if (gps.encode(ss.read()) && gps.speed.isUpdated()) {
            u8g2.firstPage();
            do {
                u8g2.drawStr(0, 63, f2s(0.5 * (prevKnots + gps.speed.knots()), 1));
                prevKnots = gps.speed.knots()
            } while ( u8g2.nextPage() );
        }
    }
}

