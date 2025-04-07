#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define bruh 3        

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
int rpm;
int speed;

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
}

void loop() {
  if (deviceConnected) {
    rpm++;
    speed++;

    String data = String(rpm%100) + "," + String(speed%100);
    
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();

    Serial.println("*********");
    Serial.print("RPM: ");
    Serial.println(rpm%100);
    Serial.print("Speed: ");
    Serial.println(speed%100);
    Serial.println("The value: ");
    Serial.println(data);
    Serial.println("*********");

    delay(2000);  // Update values every 2 seconds
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