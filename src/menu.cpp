#include "menu.hpp"
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //LCD pins

button_u readButton() {
  uint16_t adc_key_in = analogRead(analogPin);  //Leemos el pin analogico

  if (adc_key_in > 1000) return none;
  if (adc_key_in < 50)   return right;
  if (adc_key_in < 195)  return  up;
  if (adc_key_in < 380)  return  down;
  if (adc_key_in < 555)  return  left;
  if (adc_key_in < 790)  return select;
  return none;
}

//Funciones de la pantalla (menus)
void printHello(void) {
 lcd.begin(16, 2); //LCD init
  lcd.clear();
  lcd.setCursor(3, 0);  //Cursor culumna/linea
  lcd.print("EcoFlama");
  lcd.setCursor(1, 1);  //Cursor culumna/linea
  lcd.print("by Electro-AR");
  delay(500);
}

void printMensaje( char* mensaje){
 static char CACHE[15];
 if ( strcmp(mensaje,  CACHE) ){
    strcpy(CACHE, mensaje);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((char *)mensaje);
  }
}


void clock (uint32_t time, char * clock){
  uint8_t seg, min, hours = 0;

  seg = time % 60;
  min = (time / 60) % 60 ;
  hours = (time/3600) % 24;

  sprintf(clock, "%02i:%02i:%02i", hours, min, seg);
}