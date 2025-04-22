#include <Wire.h>
#include "Adafruit_TCS34725.h"

int black, white;
int hit;
int threshR, threshG, threshB;
double tiColor, tfColor;
double speed;
int radius;
const double pi = 3.14159;
int counting;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_120MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(115200);

  black = 1;
  white = 0;
  hit = 0;
  threshR = (200+1200)/2; // gotta calibrate these first
  threshG = (200+800)/2;
  threshB = (100+500)/2;
  radius = 1; // 6.5mm inner, 
  counting = 0;

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!
}

void loop(void) {
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);

  if (r <= threshR && g <= threshG && b <= threshB) {
    black = 1;
    white = 0;
  } else {
    white = 1;
    black = 0;
  } // if

  if (white && !counting) {
    counting = 1;
    Serial.println("Counting...");
    tiColor = millis();
  } else if (black && counting) {
    counting = 0;
    tfColor = millis();
    speed = 1/(tfColor-tiColor)*12*1000;
    Serial.print("Speed: ");
    Serial.println(speed);
    Serial.println("*************");
  } // if

// calibration
/*
Serial.print(r);
Serial.print(" ");
Serial.print(g);
Serial.print(" ");
Serial.println(b);
*/


/*
  Serial.println("**********************");
  if (black)
    Serial.println("DEN");
  if (white)
    Serial.println("TRANG");
*/
}
