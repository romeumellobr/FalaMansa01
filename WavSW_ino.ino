#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
 TMRpcm tmrpcm;   // create an object for use in this sketch
 //definindo as portas do projeto
int releSirene = A3;
int sinalSirene = A4;
int releLuzes = A5;
int sinalLuzes = A2;
int botaoSirene = 3;
int botaoLuzes = 2;
int buzzer = 10;
int ledR = 6;
int ledG = 7;
int ledB = 8;
void setup(){
  //Serial.begin(9600);
  //definindo modos de portas
  pinMode(buzzer, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(releSirene, OUTPUT);
  pinMode(releLuzes, OUTPUT);
  pinMode(sinalSirene, INPUT_PULLUP);
  pinMode(sinalLuzes, INPUT_PULLUP); 
  pinMode(botaoSirene, INPUT_PULLUP);
  pinMode(botaoLuzes, INPUT_PULLUP);
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
 tmrpcm.volume(9999);
 //tmrpcm.play("pop.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
 //estado normal dos reles
 //digitalWrite(releLuzes, LOW);
 digitalWrite(releSirene, HIGH);
 
}
 void loop(){  

// if ((digitalRead(A)==1) && (digitalRead(B)==1) && (millis()>=2000) {
 //}
 // precionar os 2 push button para mudar a gravação
//Serial.println(digitalRead(sinalSirene));
//  delay(330);

  if (digitalRead(sinalLuzes) == HIGH || digitalRead(botaoLuzes) == HIGH ){ 
  digitalWrite(releLuzes, HIGH);
  digitalWrite(ledB, HIGH);
  tone(buzzer, 440);
  delay(10);
}


if (digitalRead(sinalSirene) == LOW && digitalRead(sinalLuzes) == LOW && digitalRead(botaoLuzes) == LOW ){ 
  digitalWrite(releLuzes, LOW);
  digitalWrite(ledB, LOW);
  noTone(buzzer);
  delay(100);
}

 if (digitalRead(sinalSirene) == HIGH || digitalRead(botaoSirene) == HIGH) {
  digitalWrite(releLuzes, HIGH); 
  tmrpcm.play("pop.wav");
     delay(6000);
     digitalWrite(releSirene, LOW);
     delay(8000);
     digitalWrite(releSirene, HIGH);
} 
}
