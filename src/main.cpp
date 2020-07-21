#include <main.hpp>

static struct pt pellet, LCD;
uint8_t windPins []= {3,4,5,6}; //pines del ventilador 
                                //para tener una velocidad cero, poner un pin sin uso al principio

uint8_t power = 2;
estado_e ESTUFA = WAITING;
int count = 0;


//Cambia la velocidad del ventilador 
void windSpeed(uint8_t speed)
{
  for(uint8_t i = 0; i < 4; i++) {
    if( i == speed) digitalWrite(windPins[i], HIGH);
    else digitalWrite(windPins[i], LOW);
  }
}

bool timerExpired(timer_s *timer)
{
  uint32_t time = millis();
  if (time - timer->last >= timer->sleep * 1000U)
  {
    timer->last = time;
    return true;
  }
  return false;
};

void timerSet(timer_s *timer, uint32_t sleep)
{
  timer->sleep = sleep;
};

PT_THREAD(pelletDrop(struct pt *pt, uint8_t interval, uint8_t duration))
{
  PT_BEGIN(pt);
  static timer_s timer = {.last = millis(), .sleep = interval};
  static timer_s turn = {.last = millis(), .sleep = duration};
  while (true)
  {
    turn.last = millis();
    TURN_PELLET_SCREW();
    PT_WAIT_WHILE(pt, !timerExpired(&turn));
    STOP_PELLET_SCREW();

    PT_WAIT_UNTIL(pt, timerExpired(&timer));
  }
  PT_END(pt);
};

PT_THREAD(clock(struct pt *pt))
{
  static uint16_t counter = TOTAL_TIME_IGNITION;
  static timer_s timer = {.last = millis(), .sleep = 1};
  char reloj[9];
  PT_BEGIN(pt);
  while (true)
  {
    PT_WAIT_UNTIL(pt, timerExpired(&timer));
    counter--;
    clock(counter, reloj);
    printMensaje(reloj);
    if(counter == 0U) return PT_ENDED;
  }
  PT_END(pt);
};


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, 1);
  printHello();
  PT_INIT(&pellet);
  PT_INIT(&LCD);
}

void loop()
{

  switch (ESTUFA)
  {
  case WAITING:
    ESTUFA = waiting();
    break;

  case ON:
    ESTUFA = encendido();
    break;

  case CTRL:
    ESTUFA = control();
    break;

  case OFF:
    ESTUFA = apagado();
    break;

  default:
    break;
  }
}

estado_e waiting()
{

  if (readButton() == select)
  {
    return ON;
  }
  delay(100);
  return WAITING;
}

estado_e encendido()
{
  BURNER_ON();
  windSpeed(1);
  pelletDrop(&pellet, PELLET_INTERVAL_START , PELLET_REFUELL_DURATION);
  if (clock(&LCD) == PT_ENDED) return CTRL;
  return ON;
}

estado_e control()
{
  BURNER_OFF();
  windSpeed(power);
  printMensaje("Encendio");
  return CTRL;
}

estado_e apagado()
{

  return OFF;
}