#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
 TMRpcm tmrpcm;   // create an object for use in this sketch
 //definindo as portas do projeto
int releSirene = 8;
int sinalSirene = 6;
int releLuzes = 5;
int sinalLuzes = 7;
void setup(){
  //Serial.begin(9600);
  //definindo modos de portas
  pinMode(releSirene, OUTPUT);
  pinMode(releLuzes, OUTPUT);
  pinMode(sinalSirene, INPUT_PULLUP);
  pinMode(sinalLuzes, INPUT_PULLUP);
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
 tmrpcm.volume(9999);
 //tmrpcm.play("pop.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
 //estado normal dos reles
 digitalWrite(releLuzes, LOW);
 digitalWrite(releSirene, HIGH);
}
 void loop(){  
  if (digitalRead(sinalLuzes) == HIGH){ 
  digitalWrite(releLuzes, HIGH);
  delay(100);
}
  if (digitalRead(sinalSirene) == HIGH) { 
delay(100); // delay to debounce switch
  tmrpcm.play("pop.wav");
     delay(6000);
     digitalWrite(releSirene, LOW);
     delay(8000);
     digitalWrite(releSirene, HIGH);
}  
}
