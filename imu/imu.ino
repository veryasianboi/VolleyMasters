#include <Math.h>

/*
  example1-basic

  This example shows the basic settings and functions for retrieving accelerometer
    and gyroscopic data.

  Written by Elias Santistevan @ SparkFun Electronics, May, 2022

    Products:

    SparkFun 6DoF LSM6DSV16X (Qwiic):
        https://www.sparkfun.com/products/21325

    SparkFun Micro 6DoF LSM6DSV16X (Qwiic):
        https://www.sparkfun.com/products/21336

  Repository:
        https://github.com/sparkfun/SparkFun_LSM6DSV16X_Arduino_Library

  SparkFun code, firmware, and software is released under the MIT
    License	(http://opensource.org/licenses/MIT).
*/

#include "SparkFun_LSM6DSV16X.h"
#include <Wire.h>

SparkFun_LSM6DSV16X myLSM;

// Structs for X,Y,Z data
sfe_lsm_data_t accelData;
sfe_lsm_data_t gyroData;
int magnitude;
int flag;
int arraySize = 10;
int dataDump[10];
int count = 0;
int thresh = 1100;
int startTime;
int endTime;
int speed;

void setup()
{

    Wire.begin();

    Serial.begin(115200);
    while (!Serial)
    {
    }

    Serial.println("LSM6DSV16X Example 1 - Basic Readings I2C");

    if (!myLSM.begin())
    {
        Serial.println("Did not begin, check your wiring and/or I2C address!");
        while (1)
            ;
    }

    // Reset the device to default settings. This if helpful is you're doing multiple
    // uploads testing different settings.
    myLSM.deviceReset();

    // Wait for it to finish reseting
    while (!myLSM.getDeviceReset())
    {
        delay(1);
    }

    Serial.println("Board has been Reset.");
    Serial.println("Applying settings.");

    // Accelerometer and Gyroscope registers will not be updated
    // until read.
    myLSM.enableBlockDataUpdate();

    // Set the output data rate and precision of the accelerometer
    myLSM.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
    myLSM.setAccelFullScale(LSM6DSV16X_16g);

    // Set the output data rate and precision of the gyroscope
    myLSM.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
    myLSM.setGyroFullScale(LSM6DSV16X_2000dps);

    // Enable filter settling.
    myLSM.enableFilterSettling();

    // Turn on the accelerometer's filter and apply settings.
    myLSM.enableAccelLP2Filter();
    myLSM.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

    // Turn on the gyroscope's filter and apply settings.
    myLSM.enableGyroLP1Filter();
    myLSM.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

    Serial.println("Ready.");

    flag = 0;
}

void loop()
{

    // Check if both gyroscope and accelerometer data is available.
    if (myLSM.checkStatus())
    {
        myLSM.getAccel(&accelData);
        myLSM.getGyro(&gyroData);
        
        if ((abs(accelData.xData) >= thresh || abs(accelData.yData) >= thresh || abs(accelData.zData) >= thresh) && flag == 0) {
          Serial.print("Hit!");
          flag = 1;
          count = 0;
          startTime = millis();
          speed = 0;
        } else if (flag == 1 && count < arraySize) {
          //Serial.print("trigger2");
          dataDump[count] = sqrt(pow(accelData.xData,2)+pow(accelData.yData,2)+pow(accelData.zData,2))/1000;
          count++;
        } else if (count >= arraySize) {
          //Serial.print("trigger3");
          endTime = millis();
          int dx = (endTime-startTime)/arraySize;
          count = 0;
          flag = 0;
          for (int i = 0; i < arraySize; i++) {
            speed = speed + dataDump[i]*dx;
          } // for
          Serial.print(" | Speed: ");
          Serial.print(speed/100*2.23);
          Serial.println(" MPH");
        } // if
        /*
        Serial.print("Accelerometer: ");
        Serial.print("X: ");
        Serial.print(accelData.xData);
        Serial.print(" ");
        Serial.print("Y: ");
        Serial.print(accelData.yData);
        Serial.print(" ");
        Serial.print("Z: ");
        Serial.print(accelData.zData);
        Serial.println(" ");
        /*
        Serial.print("Gyroscope: ");
        Serial.print("X: ");
        Serial.print(gyroData.xData);
        Serial.print(" ");
        Serial.print("Y: ");
        Serial.print(gyroData.yData);
        Serial.print(" ");
        Serial.print("Z: ");
        Serial.print(gyroData.zData);
        Serial.println(" ");
        */
    }

}
