#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define BLUETOOTH_NAME          "ESP32_Door"
#define SERVICE_UUID            "c5816eb9-773f-4478-aa3b-c466da9ec23d"
#define CHARACTERISTIC_UUID     "a290ac53-b59f-4254-b910-c5044e1942c8"
#define CHARACTERISTIC_ID_UUID  "bcd9bccb-52f7-45a8-81b1-ae95ecac8e69"

#define BUTTON_PIN              25

BLEServer *pServer;
BLEService *pService;
BLEAdvertising *pAdvertising;
BLECharacteristic *pCharacteristic;
BLECharacteristic *pCharacteristicID;
int status = 0x00;
int num = 0x05;
int devicesConnected = 0;


void openDoor() {
  Serial.println("open");
  digitalWrite(BUTTON_PIN, HIGH);
  delay(1000);
  Serial.println("close");
  digitalWrite(BUTTON_PIN, LOW);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected!");
      devicesConnected++;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected");
      devicesConnected--;
      BLEDevice::startAdvertising();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      Serial.println("Write recieved");
      std::__cxx11::string rxValue = pCharacteristic->getValue();

      if (rxValue != "Write to Open") {
        pCharacteristic->setValue("Write to Open");
        pCharacteristic->notify(true);
        openDoor();
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up BLE!");

  pinMode(BUTTON_PIN, OUTPUT);
  digitalWrite(BUTTON_PIN, LOW);

  // Create the BLE Device
  BLEDevice::init(BLUETOOTH_NAME);
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->setValue("Write to Open");

  pCharacteristicID = pService->createCharacteristic(
                      CHARACTERISTIC_ID_UUID,
                      BLECharacteristic::PROPERTY_READ
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristicID->setValue(num);

  pService->start();



  pAdvertising = BLEDevice::getAdvertising();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pServer->setCallbacks(new MyServerCallbacks());
  BLEDevice::startAdvertising();
  Serial.printf("BLE setup complete - name: %s\n", BLUETOOTH_NAME);
}

void loop() {
}
