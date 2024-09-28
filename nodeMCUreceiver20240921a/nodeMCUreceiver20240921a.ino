#define REMOTEXY_MODE__SOFTSERIAL
// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX D3   //GPIO 0
#define REMOTEXY_SERIAL_TX D4   //GPIO 2
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_ACCESS_PASSWORD "1234"


#include <ESP8266WiFi.h>
#include <espnow.h>
#include "EasyAPI8266.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <elapsedMillis.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>

#define PUMP_ON digitalWrite(pump_Pin,HIGH) //15 D8
#define PUMP_OFF digitalWrite(pump_Pin,LOW)
#define PUMP_TIME 10000

void Init_Servo(void);
void water_action(int no);

int potTime_interval=60000;
int Analog_pin = A0;
int waterSensor_pin = D5;
int led = 2;
int limit_pin = D6;
int Servo_pin = D7;
int pump_Pin = D8;
int serMin=30;
int serMax=150;
Servo myservo;                      // 화분 위치 이동용 360 회전 서보
LiquidCrystal_I2C lcd(0x3F, 16, 2); //객채 생성
elapsedMillis potTime[5];           //이전 급수 시간 타이머 저장 
unsigned int potMoveTime[]={1900, 4800, 7300, 10050, 12600};

typedef struct struct_message {
    int id;
    int moisture;
} struct_message;

struct_message myData;
struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;
struct_message board5;

struct_message boardsStruct[5] = {board1, board2, board3, board4, board5};



// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 139 bytes
  { 255,5,0,0,0,132,0,18,0,0,0,31,1,106,200,1,1,8,0,130,
  233,246,133,217,27,21,1,11,42,36,36,0,2,31,80,111,116,49,0,1,
  58,42,36,36,0,2,31,80,111,116,50,0,1,11,87,36,36,0,2,31,
  80,111,116,51,0,1,59,88,36,36,0,2,31,80,111,116,52,0,1,35,
  133,36,36,0,2,31,80,111,116,53,0,129,6,15,94,9,64,8,65,117,
  116,111,32,87,97,116,101,114,105,110,103,32,77,97,99,104,105,110,101,0,
  129,60,188,41,7,64,24,76,101,101,32,74,117,110,32,87,111,111,0 };

struct {
  uint8_t button_01; // =1 if button pressed, else =0
  uint8_t button_02; // =1 if button pressed, else =0
  uint8_t button_03; // =1 if button pressed, else =0
  uint8_t button_04; // =1 if button pressed, else =0
  uint8_t button_05; // =1 if button pressed, else =0
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)


void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
    // Update the structures with the new incoming data
    boardsStruct[myData.id-1].moisture = myData.moisture;
    Serial.printf("moisture value: %d \n", boardsStruct[myData.id-1].moisture);
    Serial.println();
}
 
void setup() {
    pinMode(pump_Pin,OUTPUT); PUMP_OFF;
    pinMode(led,OUTPUT);
    pinMode(Analog_pin,INPUT);
    pinMode(limit_pin,INPUT_PULLUP);
    pinMode(waterSensor_pin,INPUT); //D5 14
    pinMode(pinBUZZER, OUTPUT);     
    boot_melody(2); // 시작음 2회 소리내기
    
    Serial.begin(115200);
    Serial.println();
    Serial.print("Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(WiFi.macAddress());
    
    myservo.attach(Servo_pin,544,2400);
    
    Init_Servo();
    
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
    
    for(int i=0;i<5;i++){
        potTime[i]= 0;
    }

    RemoteXY_Init(); 
}

void loop(){
    RemoteXY_Handler ();  
    int a = RemoteXY.button_01; //평상시 0 신호있으면 1
    int b = RemoteXY.button_02;
    int c = RemoteXY.button_03;
    int d = RemoteXY.button_04;
    int e = RemoteXY.button_05;
    if(a==1 || b==1 || c==1 || d==1 || e==1){
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("Mobile Mode ON");
        Serial.println("Mobile Mode ON");  
        level_up_melody(n16);
        if(a==1) boardsStruct[0].moisture=1;
        else if(b==1) boardsStruct[1].moisture=1;
        else if(c==1) boardsStruct[2].moisture=1;
        else if(d==1) boardsStruct[3].moisture=1;
        else if(e==1) boardsStruct[4].moisture=1;
    }
    
    
    
    if(digitalRead(limit_pin)==LOW){//테스트버튼으로 펌프제어하기
        level_up_melody(n32);
        PUMP_ON;
    }else{
        PUMP_OFF;
    } 
       
    for(int i=0;i<5;i++){        
        if(boardsStruct[i].moisture){
            if(potTime[i]> potTime_interval){
                potTime[i] = 0;
                Serial.print("Run ");
                Serial.print(i+1);
                Serial.println(" Position");
                lcd.clear();
                lcd.setCursor(0,0); 
                lcd.print("Run ");    
                lcd.print(i+1);  
                lcd.print(" Position  ");  
                water_action(i+1);
            }else{
                Serial.print("Run ");
                Serial.print(i+1);
                Serial.println(" Time Limit ");
                lcd.clear();
                lcd.setCursor(0,0); 
                lcd.print("Run ");    
                lcd.print(i+1);  
                lcd.print(" Time Limit");  
            }
            boardsStruct[i].moisture=0;
        }
    }
    water_tank_check();
    delay(100);
}

void Init_Servo(void){
    lcd.setCursor(0,1); lcd.print("Return Position ");  
    PUMP_OFF;
    myservo.write(serMin); delay(300);  //반시계방향 
    myservo.write(90);  delay(100); 
    myservo.write(serMax); 
    while(1){
        if(digitalRead(limit_pin)==LOW){
            myservo.write(90);   
            level_up_melody(n32); 
            break;
        }else{
            delay(1);
        }
    }
    myservo.write(serMin); 
    while(1){
        if(digitalRead(limit_pin)!=LOW){
            myservo.write(90);   
            level_up_melody(n32); 
            break;
        }else{
            delay(1);
        }
    }
    lcd.setCursor(0,1); lcd.print("Return Complete ");
}

void water_tank_check(void){
    int val=digitalRead(waterSensor_pin);
    //Serial.println(val);
    if(val==0){
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.setCursor(0,1); lcd.print("Water Tank Error"); 
        level_up_melody(n32);
        level_up_melody(n8);
    }
}

void water_action(int no){
    lcd.setCursor(0,1); lcd.print("Move Position   "); 
    myservo.write(10);  delay(potMoveTime[no-1]); myservo.write(90);
    delay(1000);
    level_up_melody(n32); level_up_melody(n32); 
    lcd.setCursor(0,1); lcd.print("Start Watering  "); 
    PUMP_ON;   delay(PUMP_TIME);   PUMP_OFF; 
    lcd.setCursor(0,1); lcd.print("Stop Watering   "); 
    level_up_melody(n16); level_up_melody(16);
    delay(1000);   
    lcd.setCursor(0,1); lcd.print("Return Position ");   
    Init_Servo();
    lcd.setCursor(0,1); lcd.print("Return Complete ");
    delay(1000);
    lcd.setCursor(0,1); 
    lcd.print("POT ");
    lcd.print(no);
    lcd.print(" End              ");
}
