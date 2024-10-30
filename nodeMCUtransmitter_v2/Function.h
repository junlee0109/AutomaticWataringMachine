#include "pitches.h"
#define  bpm_offset  1995 //1550//올라갈수록 느려짐  1995 120
#define pinSTART    0   // D3  
#define pinBUZZER   2   // D0 풀업 led
#define pinRX  3 	    // RX
#define pinTX  1 	    // TX



unsigned long _STtime = millis();
int bpm =        140;

void _timer(int _offset);
    unsigned long _timer_read(void);
void play(unsigned int sound, unsigned int note); //박자수만큼 음을 연주
void buzzer(unsigned int mel,unsigned char count, unsigned int ontime, unsigned int offtime);//부져음 횟수와 온타임오프타임지정
void melody(unsigned int data);
void boot_melody(int count);                // 부팅멜로디 함수 
void start_melody(unsigned int note);       // 도레미파솔라시도 5옥타브 
void end_melody(unsigned int note);         // 도시라솔파미레도 5옥타브
void level_up_melody(unsigned int note);    //도미솔도
void level_down_melody(unsigned int note);  //도솔미도

void start(bool waitmode);

void _boot_melody(int count){
    for(int j=0; j<count; j++){
        for(int i=1500; i<2500; i+=50){
            tone(pinBUZZER, i, 20); delay(10);
        }
    }
    noTone(pinBUZZER);
    digitalWrite(pinBUZZER ,HIGH);
}

void start(bool waitmode){
    pinMode(pinBUZZER, OUTPUT);
    //digitalWrite(pinBUZZER ,HIGH);delay(100);
    pinMode(pinSTART, INPUT_PULLUP);

	_boot_melody(1); //시작음을 1회 소리내기
    while(1){
        pinMode(pinSTART,INPUT); delay(10);
        if(digitalRead(pinSTART)==0){
            pinMode(pinSTART,OUTPUT); delay(10);
            break;
        }else{
            pinMode(pinSTART,OUTPUT); delay(10);
            pinMode(pinBUZZER, OUTPUT);  delay(10);
            digitalWrite(pinBUZZER, LOW); delay(50);
            digitalWrite(pinSTART, HIGH); delay(50);
            digitalWrite(pinBUZZER, HIGH); delay(50);
            digitalWrite(pinSTART, LOW); delay(50);
        }
        if(waitmode==0){
            pinMode(pinSTART,OUTPUT); 
            break;
        }
    }
    _boot_melody(2); //스타트버튼이 눌리면 시작음 2회 소리내기
}

void _timer(int _offset){
    _STtime = millis()+_offset;
}

unsigned long _timer_read(void){
    return millis()-_STtime;
}
void play(unsigned int sound, unsigned int note){
    
    note*=(bpm_offset/bpm);
    tone(pinBUZZER, sound, note);
    //tone(pinBUZZER, sound);
    delay(note*1.30);    
    noTone(pinBUZZER);
    digitalWrite(pinBUZZER ,HIGH);
    //pinMode(pinBUZZER, INPUT); 
}
void play_(unsigned int sound, unsigned int note){
    if(sound==0) delay(note*1.00);
    else{        
        note*=(bpm_offset/bpm);
        tone(pinBUZZER, sound);
        delay(note*1.00);
    }
    
}
void buzzer(unsigned int mel,unsigned char count, unsigned int ontime, unsigned int offtime){
    for(int i=0;i<count;i++){
        tone(pinBUZZER, mel); delay(ontime);
        noTone(pinBUZZER);    delay(offtime);
    }
    //digitalWrite(pinBUZZER ,HIGH);
    //pinMode(pinBUZZER, INPUT); 
    digitalWrite(pinBUZZER ,HIGH);
}
void melody(unsigned int data){
    if(data==0){
	noTone(pinBUZZER);
	//digitalWrite(pinBUZZER ,HIGH);
	//pinMode(pinBUZZER, INPUT); 
    }else{
 	tone(pinBUZZER, data);
    }
    //(data==0)? noTone(pinBUZZER) : tone(pinBUZZER, data);
    //if(data==1) melody((500+timer_read())*700); //헬리콥터
    digitalWrite(pinBUZZER ,HIGH);
    
}

void boot_melody(int count){
    for(int j=0;j<count;j++){
      for(int i=1500;i<2500;i=i+50){  
        tone(pinBUZZER,i,20);delay(10);
        //TimerFreeTone(pinBUZZER,i,20);delay(10);
      }
    }
    melody(0);
    digitalWrite(pinBUZZER ,HIGH);
}
void start_melody(unsigned int note){
    int melody[]={NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6,0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 9; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        noTone(pinBUZZER);
    }
    digitalWrite(pinBUZZER ,HIGH);
    //pinMode(pinBUZZER, INPUT); 
}
void end_melody(unsigned int note){
    int melody[]={NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5,0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 9; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        noTone(pinBUZZER);
    }
    digitalWrite(pinBUZZER ,HIGH);
    //pinMode(pinBUZZER, INPUT); 
}

void level_up_melody(unsigned int note){
    int melody[]={NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, 0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 5; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        noTone(pinBUZZER);
    }
    digitalWrite(pinBUZZER ,HIGH);
    //pinMode(pinBUZZER, INPUT); 
}

void level_down_melody(unsigned int note){
    int melody[]={NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5, 0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 5; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        noTone(pinBUZZER);
    }
    pinMode(pinBUZZER, INPUT); 
    //digitalWrite(pinBUZZER ,HIGH);
}
