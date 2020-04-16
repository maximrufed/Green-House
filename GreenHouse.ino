#include <Arduino.h>
//#include "RTClib.h"
#include <Wire.h>

#include "Globals.h"
#include "gh_menu.h"
#include "gh_TSensors.h"
#include "gh_EarthFan.h"
#include "gh_RTC.h"



//---------------------------------------------------------------------
//----------------------- GLOBAL OBJECTS --------------------------------

T_Sensors TSensors(ONE_WIRE_BUS);
Earth_Fan EarthFan(RELAY_EARTH_FAN, LED_FAN, LED_FAN_MANUAL_MODE );
//RTC_DS3231 rtc;
gh_RTC rtc;

  
//---------------------------------------------------------------------
//----------------------- SETUP & LOOP --------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Green House project. Copyright Anton Kostrov 2019"); 
  Serial.flush();
  delay(3000); // wait for console opening

  TSensors.Begin();
  EarthFan.Begin();

  // Запускаем часы реального времени
  if (! rtc.begin()) LOG("Couldn't find RTC");


  // Инициализация меню
  joystickBtns.begin();
  lcd.begin(20, 4); // Запускаем экран
  nav.idleTask = ScreenSaver; //Устанавливаем функцию скринсейвера
  nav.showTitle = false;
  nav.timeOut = 10;   // seconds to start screensaver
  options=&myMenuOptions; // Устанавливаем свои значения глобальных Options для меню

  // Печатаем приветствие
  lcd.setCursor(0, 0);
  //lcd.print("Попробуем?");
  lcd.setCursor(0, 3);
  lcd.print("Does it work?");
  delay(3000);
  
}

long prevmill = 0;

void loop() {

  // Отработка меню
  nav.poll();
  
  // Запрос обновления значений датчиков на шине
  if( TSensors.UpdateSensorsOnBus() ){
    // Перерисовать Screensaver
    nav.idleChanged=true;
  }

  // Обработка алгоритма работы вентилятора - земляного аккумулятора
  EarthFan.TerraAccumulatorPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());
  
  /*long currmill = millis();
  if( currmill - prevmill > 6000) {
    prevmill = currmill;
    
    float cAir = TSensors.GetTAir();
    //float cAir = TSensorAir.getT();
    LOG("Air temp1 in C = ");
    LOG(cAir );
    //float cEarth = TSensorEarth.getT();
    float cEarth = TSensors.GetTEarth();
    LOG("Earth temp1 in C = ");
    LOG(cEarth );
    LOG("------------");
    LOG("");

  }*/
  


  delay( 100 );

  
}

//--------------------------------------------------------------------------------------

  
