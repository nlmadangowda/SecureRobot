// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// #include "main.h"

// #define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// BLEServer* pServer = NULL;
// BLEServiceMap BleService;
// BLECharacteristic* pCharacteristic = NULL;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;
// uint32_t value = 0;

// char g_config_data[360]={0};
// static int ParseConfigJson(std::string str){
//   if (str.length() > 0) {
//     memset(g_config_data,0,sizeof(g_config_data));
//     Serial.print("New value: ");
//     for (int i = 0; i < str.length(); i++){
//       g_config_data[i]=str[i];
//     }
//     Serial.println(g_config_data);
//     Serial.println("*********");
//     return 1;
//   }
//   return 0;
// }

// class MyServerCallbacks: public BLEServerCallbacks {
//     void onConnect(BLEServer* pServer) {
//       deviceConnected = true;
//       Serial.println("Client Device Connected");
//       BLEDevice::startAdvertising();
//     };

//     void onDisconnect(BLEServer* pServer) {
//       Serial.println("Client Device DisConnected");
//       deviceConnected = false;
//     }
// };

// class MyCallbacks: public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *pCharacteristic) {
//       std::string value = pCharacteristic->getValue();
//       ParseConfigJson(value);
//     }
// };

// void StartBLE(){
//   BLEDevice::init("ESP32");

//   // Create the BLE Server
//   pServer = BLEDevice::createServer();
//   pServer->setCallbacks(new MyServerCallbacks());

//   // Create the BLE Service
//   BLEService *pService = pServer->createService(SERVICE_UUID);

//   // Create a BLE Characteristic
//   pCharacteristic = pService->createCharacteristic(
//                       CHARACTERISTIC_UUID,
//                       BLECharacteristic::PROPERTY_READ   |
//                       BLECharacteristic::PROPERTY_WRITE  |
//                       BLECharacteristic::PROPERTY_NOTIFY |
//                       BLECharacteristic::PROPERTY_INDICATE
//                     );
//   pCharacteristic->setCallbacks(new MyCallbacks());

//   pCharacteristic->addDescriptor(new BLE2902());

//   pCharacteristic->setValue("Hello World");

//   // Start the service
//   pService->start();

//   // Start advertising
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID);
//   pAdvertising->setScanResponse(false);
//   pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
//   BLEDevice::startAdvertising();
//   Serial.println("Waiting a client connection to notify...");
// }

// void BLETask(void *pvParameters){
//   (void) pvParameters;
//   StartBLE();
//   while(1){
//     if (!deviceConnected && oldDeviceConnected) {
//         delay(500); // give the bluetooth stack the chance to get things ready
//         pServer->startAdvertising(); // restart advertising
//         Serial.println("start advertising");
//         oldDeviceConnected = deviceConnected;
//     }
//     if (deviceConnected && !oldDeviceConnected) {
//         // do stuff here on connecting
//         oldDeviceConnected = deviceConnected;
//     }
//   }
// }
