unsigned long _STtime = millis();
int bpm =        140;
#define buzzerPin 16  //D0    
#define sdaPin     4  //D2
#define sclPin     5  //D1


#define pinBUZZER 16    //D0
#define pinSTART 0  // D3

#include "pitches.h"
void start(bool no);

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
    //noTone(pinBUZZER);
    digitalWrite(pinBUZZER, HIGH);
}
void buzzer(unsigned int mel,unsigned char count, unsigned int ontime, unsigned int offtime){
    for(int i=0;i<count;i++){
        tone(pinBUZZER, mel); delay(ontime);
        //digitalWrite(pinBUZZER, HIGH);//noTone(pinBUZZER);    
        delay(offtime);
    }
    digitalWrite(pinBUZZER, HIGH);
}
void melody(unsigned int data){
    (data==0)? noTone(pinBUZZER) : tone(pinBUZZER, data);
    //delay(50);
    //if(data==999){
     //   melody((500+timer_read())*700); //헬리콥터
   // }
    digitalWrite(pinBUZZER, HIGH);
    
}
void boot_melody(int count){
    for(int j=0; j<count; j++){
        for(int i=1500; i<2500; i+=50){
            tone(pinBUZZER, i, 20); delay(10);
        }
    }
    //noTone(pinBUZZER);
    digitalWrite(pinBUZZER, HIGH);
}

void start_melody(unsigned int note){
    int melody[]={NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6,0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 9; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        //digitalWrite(pinBUZZER, HIGH);//noTone(pinBUZZER);
    }
    delay(note);
    digitalWrite(pinBUZZER, HIGH);
}
void end_melody(unsigned int note){
    int melody[]={NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5,0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 9; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        //digitalWrite(pinBUZZER, HIGH);//noTone(pinBUZZER);
    }
    delay(note);
    digitalWrite(pinBUZZER, HIGH);
}

void level_up_melody(unsigned int note){
    int melody[]={NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, 0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 5; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        //digitalWrite(pinBUZZER, HIGH);//noTone(pinBUZZER);
    }
    delay(note);
    digitalWrite(pinBUZZER, HIGH);
}

void level_down_melody(unsigned int note){
    int melody[]={NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5, 0};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 5; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note);
        delay(note);
        //digitalWrite(pinBUZZER, HIGH);//noTone(pinBUZZER);
    }
    delay(note);
    digitalWrite(pinBUZZER, HIGH);
}
