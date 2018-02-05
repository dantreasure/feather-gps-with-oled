// Test code for Ultimate GPS Using Hardware Serial (e.g. GPS Flora or FeatherWing)
//
// This code shows how to listen to the GPS module via polling. Best used with
// Feathers or Flora where you have hardware Serial and no interrupt
//
// Tested and works great with the Adafruit GPS FeatherWing
// ------> https://www.adafruit.com/products/3133
// or Flora GPS
// ------> https://www.adafruit.com/products/1059
// but also works with the shield, breakout
// ------> https://www.adafruit.com/products/1272
// ------> https://www.adafruit.com/products/746
// 
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada
     
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false;

uint32_t timer = millis();

#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#define LED      13

void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  oled.init();
  oled.setTextSize(1);
  oled.println("Ready to establish");
  oled.println("");
  oled.println("GPS FIX");
  oled.display();
  // connect at 115200 so we can read the GfPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  Serial.println("Adafruit GPS library basic test!");
     
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
     
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

int displayMode = 3;

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  // if a sentence is received, we can check the checksum, parse it...
  
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
      Serial.println(GPS.lastNMEA()); 
//    Serial.print("\nTime: ");
//    Serial.print(GPS.hour, DEC); Serial.print(':');
//    Serial.print(GPS.minute, DEC); Serial.print(':');
//    Serial.print(GPS.seconds, DEC); Serial.print('.');
//    Serial.println(GPS.milliseconds);
//    Serial.print("Date: ");
//    Serial.print(GPS.day, DEC); Serial.print('/');
//    Serial.print(GPS.month, DEC); Serial.print("/20");
//    Serial.println(GPS.year, DEC);
//    Serial.print("Fix: "); Serial.print((int)GPS.fix);
//    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
//      Serial.print("Location: ");
//      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
//      Serial.print(", ");
//      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
//      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
//      Serial.print("Angle: "); Serial.println(GPS.angle);
//      Serial.print("Altitude: "); Serial.println(GPS.altitude);
//      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

      if (displayMode == 1) {
        displayPosition();
      }
    
      if (displayMode == 2) {
        displaySpeed();
      }
    
      if (displayMode == 3) {
        displayTime();
      } 
    } else {
      displayWaiting();
    }
  }

  if (! digitalRead(BUTTON_A)) displayMode = 1;
  if (! digitalRead(BUTTON_B)) displayMode = 2;
  if (! digitalRead(BUTTON_C)) displayMode = 3;
  delay(10);
  yield();
}

void displayWaiting() {
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.println("Waiting for");
  oled.println("GPS fix");
  oled.display();
}

void displayPosition() {
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.println("Position mode");
  oled.print("Lat: ");
  oled.print(GPS.latitude, 4); oled.print(GPS.lat);
  oled.println("");
  oled.print("Lon: "); 
  oled.print(GPS.longitude, 4); oled.println(GPS.lon);
  oled.display();
}

void displaySpeed() {
  float mph = GPS.speed * 1.152;
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.setTextSize(2);
  oled.print(mph);
  oled.display();
}

void displayTime() {
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.print(GPS.hour, DEC); oled.print(':');
  oled.print(GPS.minute, DEC); oled.print(':');
  oled.print(GPS.seconds, DEC); 
  oled.display();
}

