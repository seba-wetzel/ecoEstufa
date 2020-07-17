#include <Arduino.h>
#include <LiquidCrystal.h>

#define analogPin A0

typedef enum {
  right,
  up,
  down,
  left,
  select,
  none
} button_u;


button_u readButton();
void printHello(void);
void printMensaje( char* );
void clock (uint32_t time, char * clock);