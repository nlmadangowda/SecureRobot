#include <Crypto.h>
#include <AES.h>
#include <string.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//#include "main.h"

//key[16] cotain 16 byte key(128 bit) for encryption
byte key[16]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
//plaintext[16] contain the text we need to encrypt
uint8_t plaintext[5]={'S','T','A','R','T'};
uint8_t stoptext[5]={'S','T','O','P'};
//cypher[16] stores the encrypted text
uint8_t cypher[16];
//decryptedtext[16] stores decrypted text after decryption
uint8_t decryptedtext[16];
//creating an object of AES128 class
AES128 aes128;


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLEServiceMap BleService;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

uint8_t g_config_data[360]={0};



#define LHS_IR 22
#define RHS_IR 21

#define LHS_W_IN1 23
#define LHS_W_IN2 19

#define RHS_W_IN1 25
#define RHS_W_IN2 33

uint32_t flag = 0;

static int ParseConfigJson(std::string str){
  if (str.length() > 0) {
    memset(g_config_data,0,sizeof(g_config_data));
    for (int i = 0; i < str.length(); i++){
      g_config_data[i]=str[i];
    }
    aes128.decryptBlock(decryptedtext,g_config_data);
    if(!strcmp((const char *)decryptedtext,(const char *)plaintext)){
      flag = 1;
      Serial.println("START");
    }else if(!strcmp((const char *)decryptedtext,(const char *)stoptext)){
      flag = 2;
      Serial.println("STOP");
    }else{
      flag = 0;
      Serial.println("INVAL");
    }
    return 1;
  }
  return 0;
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Client Device Connected");
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Client Device DisConnected");
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      ParseConfigJson(value);
    }
};

void StartBLE(){
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic->setValue("Hello World");

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}


void setup() {
    Serial.begin(115200);
    pinMode(LHS_IR,INPUT);
    pinMode(RHS_IR,INPUT);

    pinMode(LHS_W_IN1, OUTPUT);
    pinMode(LHS_W_IN2, OUTPUT);

    pinMode(RHS_W_IN1, OUTPUT);
    pinMode(RHS_W_IN2, OUTPUT); 
    aes128.setKey(key,16);// Setting Key for AES
    Serial.println();
    Serial.println();
    Serial.println();

    Serial.print("Before Encryption:");
    for(int i=0; i<sizeof(stoptext); i++){
        Serial.printf("0x%02x",stoptext[i]);
    }
   
    aes128.encryptBlock(cypher,stoptext);//cypher->output block and plaintext->input block
    Serial.println();
    Serial.print("After Encryption:");
    for(int j=0;j<sizeof(cypher);j++){
        Serial.printf("%02x",cypher[j]);
    }
      Serial.println();
    aes128.decryptBlock(decryptedtext,cypher);
    
    Serial.println();
    Serial.print("After Dencryption:");
    for(int i=0; i<sizeof(decryptedtext); i++){
      Serial.write(decryptedtext[i]);
    }
     Serial.println();
  StartBLE();
}

void loop() {
 if (!deviceConnected && oldDeviceConnected) {
  delay(500); // give the bluetooth stack the chance to get things ready
  pServer->startAdvertising(); // restart advertising
  Serial.println("start advertising");
  oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
  }
    if(flag == 1){
      int LHS_IR_VAL = digitalRead(LHS_IR);
      int RHS_IR_VAL = digitalRead(RHS_IR);
      
      if (RHS_IR_VAL == LOW && LHS_IR_VAL == LOW)
      {
          digitalWrite(RHS_W_IN1, HIGH);
          digitalWrite(RHS_W_IN2, LOW);   
          digitalWrite(LHS_W_IN1, HIGH );
          digitalWrite(LHS_W_IN2, LOW ); 
      }
      //If right sensor detects black line, then turn right
      else if (RHS_IR_VAL == HIGH && LHS_IR_VAL == LOW )
      {
          digitalWrite(RHS_W_IN1, LOW);
          digitalWrite(RHS_W_IN2, HIGH);   
          digitalWrite(LHS_W_IN1, LOW );
          digitalWrite(LHS_W_IN2, LOW ); 
      }
      //If left sensor detects black line, then turn left  
      else if (RHS_IR_VAL == LOW && LHS_IR_VAL == HIGH )
      {
          digitalWrite(RHS_W_IN1, LOW);
          digitalWrite(RHS_W_IN2, LOW);   
          digitalWrite(LHS_W_IN1, LOW );
          digitalWrite(LHS_W_IN2, HIGH ); 
      } 
      //If both the sensors detect black line, then stop 
      else 
      {
          digitalWrite(RHS_W_IN1, LOW);
          digitalWrite(RHS_W_IN2, LOW);   
          digitalWrite(LHS_W_IN1, LOW );
          digitalWrite(LHS_W_IN2, LOW ); 
      }
    }else if(flag == 2){
        digitalWrite(RHS_W_IN1, LOW);
      digitalWrite(RHS_W_IN2, LOW);   
      digitalWrite(LHS_W_IN1, LOW );
      digitalWrite(LHS_W_IN2, LOW ); 
    }else{
      // NOP
    }
}


// // https://github.com/un0038998/LineFollowerRobot/blob/main/LineFollowerRobot/LineFollowerRobot.ino
