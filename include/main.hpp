#include <Arduino.h>
#include "menu.hpp"
#include "pt.h"

#define SCREW_PIN 13
#define TURN_PELLET_SCREW() digitalWrite(SCREW_PIN,1)
#define STOP_PELLET_SCREW() digitalWrite(SCREW_PIN,0)

#define BURNER_PIN 3
#define BURNER_ON()  digitalWrite(BURNER_PIN,1)
#define BURNER_OFF() digitalWrite(BURNER_PIN,0)

#define TOTAL_TIME_IGNITION 240
#define PELLET_INTERVAL_START  60 //Intervalo en segundos para dropear pellets al 
                               //comienzo del fuego

#define PELLET_REFUELL_DURATION 5

typedef enum {
  ON,
  CTRL,
  OFF, 
  WAITING
} estado_e;

typedef struct
{
  /* data */
  uint32_t last;
  uint32_t sleep;
} timer_s;

estado_e encendido ();
estado_e control ();
estado_e apagado ();
estado_e waiting ();
