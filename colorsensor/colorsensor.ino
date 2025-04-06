
/*
  ISL29125 RGB sensor test
  isl29125-test.ino
  Displays RGB values for ISL29125 RGB sensor
  Uses values obtained with Sparkfun ISL29125 RGB sensor basic demo
  Uses Sparkfun ISL29125 Library
 
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/
 
// Include I2C Library
#include <Wire.h>
 
// Include Sparkfun ISL29125 Library
#include "SFE_ISL29125.h"
 
// Declare sensor object
SFE_ISL29125 RGB_sensor;
 
// Calibration values
 
unsigned int redlow = 0;
unsigned int redhigh = 0;
unsigned int greenlow = 0;
unsigned int greenhigh = 0;
unsigned int bluelow = 0;
unsigned int bluehigh = 0;
 
// Declare RGB Values
int redVal = 0;
int greenVal = 0;
int blueVal = 0;
 
 
void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
 
  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
}
 
 
void loop()
{
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();
  
  // Convert to RGB values
  int redV = map(red, redlow, redhigh, 0, 255);
  int greenV = map(green, greenlow, greenhigh, 0, 255);
  int blueV = map(blue, bluelow, bluehigh, 0, 255);
  
  // Constrain to values of 0-255
  redVal = constrain(redV, 0, 255);
  greenVal = constrain(greenV, 0, 255);
  blueVal = constrain(blueV, 0, 255);
     
  Serial.print("Red: "); 
  Serial.print(redVal);
  Serial.print(" - Green: ");
  Serial.print(greenVal);
  Serial.print(" - Blue: "); 
  Serial.println(blueVal);
  
  // Delay for sensor to stabilize
  delay(2000);
}