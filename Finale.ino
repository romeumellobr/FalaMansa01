/*
 *  
 */
//#include <Bounce2.h>
#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
 TMRpcm tmrpcm;   // create an object for use in this sketch
 
#define S_INATIVO 1 //led verde
#define S_ALARMEON 2 //led vermelho
#define S_ALARMEOFF 3 // aguarda luz acesa para desligar após 10 minutos, led ao inves de piscar fica constante aceso vermelho pra indicar o tempo de desligamento da luz
#define S_LUZESON 4 //led azul
#define S_LUZESOFF 5 //apenas delisga luz e o led azul
#define S_TURNOFF 6 //aqui faz o clean de todos, manda tudo pra low e volta para estado inativo

//// Instantiate a Bounce object
//Bounce debouncer1 = Bounce(); 
//
//// Instantiate another Bounce object
//Bounce debouncer2 = Bounce(); 

 class Alarme
{
 // Class Member Variables
  // These are initialized at startup
  int sinalSirene;      // the number of the LED pin
  int releSirene;      // the number of the LED pin
  
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
 
  // These maintain the current state
  int alarmeEstado;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Alarme(int rele, long on, long off)
  {
  releSirene = rele;
  pinMode(releSirene, OUTPUT);     
    
  OnTime = on;
  OffTime = off;
  
  alarmeEstado = LOW; 
  previousMillis = 0;
  }
 
  void Sirene()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((alarmeEstado == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      tmrpcm.stopPlayback();
      alarmeEstado = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(releSirene, alarmeEstado);  // Update the actual LED
    }
    else if ((alarmeEstado == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      alarmeEstado = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(releSirene, alarmeEstado);   // Update the actual LED
      tmrpcm.play("pop.wav");
    }
  }
  void Clean(){
    tmrpcm.stopPlayback();
    digitalWrite(releSirene, LOW);
  }
};

 class Flasher
{
 // Class Member Variables
  // These are initialized at startup
  int ledPin;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
 
  // These maintain the current state
  int ledState;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);     
    
  OnTime = on;
  OffTime = off;
  
  ledState = LOW; 
  previousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);   // Update the actual LED
    }
  }
  void High()
  {
    digitalWrite(ledPin, HIGH);
  }
  void Clean()
  {
    digitalWrite(ledPin, LOW);
  }
};

 class Luzes
{
 // Class Member Variables
  // These are initialized at startup
  int releLuzes;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
 
  // These maintain the current state
  int luzesEstado;                 // ledState used to set the LED
  unsigned long millisAnterior;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Luzes(int pino, long off)
  {
  releLuzes = pino;
  pinMode(releLuzes, OUTPUT);     
    
  OffTime = off;
  
  luzesEstado = LOW; 
  millisAnterior = 0;
  }
 
  void LigarLuzes()
  {
    luzesEstado = HIGH;
    digitalWrite(releLuzes, luzesEstado);
    }
    
  void Desligar()
  {
    luzesEstado = LOW;
    digitalWrite(releLuzes, luzesEstado);
  }
};

Flasher ledLuzes(8, 550,550); //led azul
Flasher ledInativo(7, 800, 700); //led verde
Flasher ledSirene(6, 300, 200); //led vermelho
Luzes luzes(A5, 2000);
Alarme alarme(A3, 6000, 8000); 

void setup(){
//  // Setup the first button with an internal pull-up :
//  pinMode(BUTTON_PIN_1,INPUT_PULLUP);
//  // After setting up the button, setup the Bounce instance :
//  debouncer1.attach(BUTTON_PIN_1);
//  debouncer1.interval(5); // interval in ms
//  
//   // Setup the second button with an internal pull-up :
//  pinMode(BUTTON_PIN_2,INPUT_PULLUP);
//  // After setting up the button, setup the Bounce instance :
//  debouncer2.attach(BUTTON_PIN_2);
//  debouncer2.interval(5); // interval in ms

  pinMode(A4, INPUT_PULLUP); //sinal sirene
  pinMode(A2, INPUT_PULLUP); //sinal luzes
  pinMode(2,  INPUT_PULLUP); //botão Luzes
  pinMode(3,  INPUT_PULLUP); //botão Luzes
  
  
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
 tmrpcm.volume(1);
}
void loop(){
  static int state = S_INATIVO; // initial state is 1, the "idle" state.
  static unsigned long ts;  // To store the "current" time in for delays.
  // Update the Bounce instances :
  //debouncer1.update();
  //debouncer2.update();

  // Get the updated value :
  //int botao1 = debouncer1.read();
  //int botao2 = debouncer2.read();
  
  switch(state)
  {
    case S_INATIVO:
      // We don't need to do anything here, waiting for a forced state change.
      ledInativo.Update();
      if ( digitalRead(3) == HIGH || digitalRead(A4) == HIGH ) { // sinal sirene ou botão
        ledInativo.Clean();
        delay(200);
          state = S_ALARMEON;
      }
      if ( digitalRead(2) == HIGH || digitalRead(A2) == HIGH ) { // sinal luzes ou botão
        ledInativo.Clean();
        delay(200);
          state = S_LUZESON;
      }
      break;
    case S_ALARMEON:
      //debouncer2.update();
      alarme.Sirene();
      luzes.LigarLuzes();
      ledSirene.Update();
      ledLuzes.Update();
      if(digitalRead(3) == HIGH || digitalRead(A4) == LOW) {
        state = S_ALARMEOFF;
      }
      ts = millis();
      break;
    case S_ALARMEOFF:
      // If one second has passed, then move on to the next state
      alarme.Clean();
      ledLuzes.Clean();
      ledSirene.High();
      if(millis() > ts + 600000) // tempo para desligar a luz depois de alarme desativado
      {
        luzes.Desligar();
        ledSirene.Clean();
        state = S_TURNOFF;
      }
      break;
    case S_LUZESON:
      ledLuzes.Update();
      luzes.LigarLuzes();
      if(digitalRead(2) == HIGH || digitalRead(A2) == LOW) {
        delay(200);
        state = S_LUZESOFF;
      }
      break;
    case S_LUZESOFF:
      // If one second has passed, then go back to state 2.
      luzes.Desligar();
      ledLuzes.Clean();  // Turn off the light
      delay(200);
      state = S_INATIVO;
      break;
    case S_TURNOFF:
      // We only get here when forced from outside.
      delay(10);
      state = S_INATIVO;  // Return to the "idle" state.
      break;
    default:
      state = S_INATIVO;
      break;
  }
}
