#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/* float to string
 * f is the float to turn into a string
 * p is the precision (number of decimals)
 * return a string representation of the float.
 */
char *f2s(float f, int p){
  char * pBuff;                         // use to remember which part of the buffer to use for dtostrf
  const int iSize = 10;                 // number of bufffers, one for each float before wrapping around
  static char sBuff[iSize][6];         // space for 20 characters including NULL terminator for each float
  static int iCount = 0;                // keep a tab of next place in sBuff to use
  pBuff = sBuff[iCount];                // use this buffer
  if(iCount >= iSize -1){               // check for wrap
    iCount = 0;                         // if wrapping start again and reset
  }
  else{
    iCount++;                           // advance the counter
  }
  return dtostrf(f, 0, p, pBuff);       // call the library function
}

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
}

void loop() {
    while (ss.available() > 0) {
        if (gps.encode(ss.read()) && gps.speed.isUpdated()) {
            u8g2.firstPage();
            do {
                u8g2.drawStr(0, 63, f2s(gps.speed.knots(), 1));
            } while ( u8g2.nextPage() );
        }
    }
}

