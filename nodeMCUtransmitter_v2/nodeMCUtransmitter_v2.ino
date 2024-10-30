//개별 화분용 송신기

#include <ESP8266WiFi.h>
#include <espnow.h>
#include "Function.h"

//const int buttonPin=0;
//const int ledPin = 0;
//const int buzzerPin=2;
const int sensorPin=3; // rx

uint8_t broadcastAddress[] = {0xFC, 0xF5, 0xC4, 0x96, 0xAD, 0x4C}; //수신보드 주소
#define BOARD_ID 3 //1~5번
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

/*************************************/
// 전송되는 데이터 구조체 
/*************************************/
typedef struct struct_message {
    int id;
    int moisture;
} struct_message;
struct_message myData;




/***************************************************************/
// ESP-NOW 통신용 콜백함수
/**************************************************************/
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    //play(sol4,n4); 
    Serial.println("Delivery fail");
  }
}


/***************************************************************/
// 아두이노 SETUP 함수
/**************************************************************/
void setup() {       
    start(0);
    pinMode(sensorPin,INPUT_PULLUP);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);

  // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }   
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // Set ESP-NOW role 
    esp_now_register_send_cb(OnDataSent); //espbow 콜백함수 지정
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0); // Register peer
}


/***************************************************************/
// 아두이노 LOOP 반복 함수
/**************************************************************/
void loop() {
  if ((millis() - lastTime) > timerDelay) { //지정시간이 되면
    myData.id = BOARD_ID;    
    myData.moisture = digitalRead(sensorPin);
    if(myData.moisture==1){
        play(NOTE_G5,n16);play(NOTE_G5,n16);
    }    
    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();    
  }
  delay(10);
}
