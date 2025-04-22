#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define bruh 3  
#define hall 23

// bluetooth variables
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
// color sensor variables
int black, white;
int hit;
int threshR, threshG, threshB;
double tiColor, tfColor;
double speed;
double innerRadius;
double outerRadius;
const double pi = 3.14159;
int counting;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X);
// hall effect variables
int oldVal = 0;
int currVal;
double tiHall = 0.0;
double tfHall;
double rpm;
int risingEdge = 0; // risingEdge = 0 is a falling edge
int lux;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(bruh, INPUT);
  rpm = 0;
  speed = 0;

  BLEDevice::init("Kyle");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection...");

  black = 1;
  white = 0;
  hit = 0;
  threshR = (3+30)/2; // gotta calibrate these first
  threshG = (3+20)/2;
  threshB = (2+10)/2;
  innerRadius = 0.3346; // 8.5mm inner = 0.3346 in 
  outerRadius = 8.917; // 226.5mm outer = 8.917 in
  counting = 0;

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  pinMode(hall,INPUT_PULLUP);
}

void loop() {
  if (deviceConnected) {
    uint16_t r, g, b, c;

    tcs.getRawData(&r, &g, &b, &c);
    lux = tcs.calculateLux(r, g, b);

    if (lux >= 35) {
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
      speed = 0.5/((tfColor-tiColor)*2*pi*innerRadius)*1000*3600*outerRadius/innerRadius/(12*5280); // dist b/t black lines/time
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

    currVal = digitalRead(hall);
    
    if (currVal == 1 && oldVal == 0 && risingEdge == 0) {
    oldVal = 1;
    risingEdge = 1;
    } else if (currVal == 0 && oldVal == 1) {
      risingEdge = 0;
      oldVal = 0;
    } else {
      risingEdge = 0;
    }

    if (risingEdge == 1) {
      tfHall = millis();
      rpm = 1/(tfHall-tiHall)*1000*60/2;
      tiHall = tfHall+0;
      if (rpm >= 1 && rpm < 10000) {
        Serial.println("**************");
        Serial.print("RPM: ");
        Serial.println(rpm);
      } // if
    } // if

    String data = String(rpm) + "," + String(speed);
    
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();

    Serial.println("*********");
    Serial.print("RPM: ");
    Serial.println(rpm);
    Serial.print("Speed: ");
    Serial.println(speed);
    Serial.println("The value: ");
    Serial.println(data);
    Serial.println("*********");

    // i took the delay out monkaS
  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(500);  // Give the Bluetooth stack the chance to get disconnected
    pServer->startAdvertising();  // Restart advertising
    Serial.println("Waiting for a client connection...");
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}