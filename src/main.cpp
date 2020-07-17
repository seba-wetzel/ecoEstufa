#include <Arduino.h>
#include "menu.hpp"

typedef enum {
  ON,
  CTRL,
  OFF, 
  WAITING
} estado_e;

typedef struct
{
  /* data */
  uint32_t current;
  uint32_t last;
  uint32_t stared;
  uint32_t elapse;
  uint32_t sleep;
} time_s;

bool wait (time_s * TIME, uint32_t delay);
bool task (time_s *TIME, uint32_t delay, void (*cb)() ) ;
void toggleLed (){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //loop();
}

estado_e encendido ();
estado_e control ();
estado_e apagado ();
estado_e waiting ();

estado_e ESTUFA =  WAITING;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, 1);
  printHello();

}

void loop() {

    switch (ESTUFA)
    {
    case WAITING: ESTUFA = waiting();
      break;
    
    case ON: ESTUFA = encendido();
      break;
    
    case CTRL: ESTUFA = control();
      break;
    
    case OFF: ESTUFA = apagado();
      break;

    default:
      break;
    }
}

estado_e waiting (){
 
 if( readButton() == select){
   return ON;
 }
  return WAITING;
}

estado_e encendido (){
  char  reloj  [9];
  static time_s TIME = {.current = millis(), .last =millis(), .stared = millis()};
  
  task(&TIME, 10, toggleLed); //Una tarea que parpadea un led cada 10 segundos
  //if((TIME.elapse)>100) return CTRL;
  clock( TIME.elapse, reloj);
  printMensaje(reloj);
  return ON;
}

estado_e control (){
  printMensaje("Encendio");
  return CTRL;
}
estado_e apagado (){
 

  return OFF;
}

//Hace las veces de un systick del estado y cuenta 
//el tiempo entre eventos
//recibe una estructura de tiempo y un valor en segundo para el evento
//cada estado solo puede tener un evento, los demas se deben calcular
//por medio de la funcion de callback de la tarea
bool wait (time_s * TIME, uint32_t delay){
      TIME->current = millis();
      if (TIME->current - TIME->last >= 1000) {      
      TIME->last = TIME->current;
      TIME->elapse = (TIME->current - TIME->stared)/1000;
      }
      if(TIME->elapse - TIME->sleep >= delay) {
        TIME->sleep = TIME->elapse;
        return true;
      }
      else return false;
}

//ejecuta un funcion void dada, despues de un tiempo dado
//si el tiempo no fue alcanzado no bloquea a la continuidad 
//del estado de la maquina
bool task (time_s *TIME, uint32_t delay, void (*cb)() ) {
  //Se llama a wait para calcular el tiempo 
   if(wait((time_s *)TIME, delay)){
     (*cb)(); //Funcion de callback
     return true;
   }
   else return false;
}