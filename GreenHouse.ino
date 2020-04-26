#include <Arduino.h>
//#include "RTClib.h"
#include <Wire.h>

#include "Globals.h"
#include "gh_menu.h"
#include "gh_TSensors.h"
#include "gh_EarthFan.h"
#include "gh_RTC.h"
#include "gh_windows.h"



//---------------------------------------------------------------------
//----------------------- GLOBAL OBJECTS --------------------------------

T_Sensors TSensors(ONE_WIRE_BUS);
Earth_Fan EarthFan(RELAY_EARTH_FAN, LED_FAN, LED_FAN_MANUAL_MODE );
//RTC_DS3231 rtc;
gh_RTC rtc;
GHWindow Window;
  
//---------------------------------------------------------------------
//----------------------------- SETUP  --------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Green House project. Copyright Anton Kostrov 2019"); 
  Serial.flush();
  delay(3000); // wait for console opening

  TSensors.Begin(1); // Интервал опроса датчиков на шине 1 минута
  EarthFan.Begin();

  // Запускаем часы реального времени
  if (! rtc.Begin()) LOG("Couldn't find RTC");

  // Инициализируем окно
  GHWindowHardwareConfig WinConfig;
  WinConfig.PinRelay1				= RELAY_WINDOW_1;
  WinConfig.PinRelay2				= RELAY_WINDOW_2;
  WinConfig.PinLimitSwitchOpen		= LS_WINDOW_ROOF_OPEN;
  WinConfig.PinLimitSwitchClosed	= LS_WINDOW_ROOF_CLOSED;
  if (! Window.Begin(WinConfig)) LOG("Couldn't initialize window");

  // Инициализация меню
  joystickBtns.begin();
  lcd.begin(20, 4); // Запускаем экран
  lcd.setBacklight(255);
  nav.idleTask = ScreenSaver; //Устанавливаем функцию скринсейвера
  nav.showTitle = false;
  nav.timeOut = 10;   // seconds to start screensaver
  options=&myMenuOptions; // Устанавливаем свои значения глобальных Options для меню

  // Печатаем приветствие
  lcd.setCursor(0, 0);
  //lcd.print("Попробуем?");
  lcd.setCursor(0, 3);
  lcd.print("Does it work?");
  delay(1000);
  
}

long prevmill = 0;

//---------------------------------------------------------------------
//------------------------------ LOOP ---------------------------------


void loop() {
  
  // *****************************************
  // Обработка быстрых устройств - каждый цикл

  // Обработка форточки
  Window.WindowPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());


  

    // ****************************************************
    //Обработка медленных устройств - каждые 100 миллисекунд
    // Отработка меню
    nav.poll();
  
    // Запрос обновления значений датчиков на шине
    if( TSensors.TSensorsPoll() ){
      // Если значения датчиков обновились, что случается не часто, то перерисовать Screensaver
      nav.idleChanged=true;
    }
    
    // Обработка вентилятора - земляного аккумулятора
    EarthFan.TerraAccumulatorPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());

  
  // Если делать delay, то концевые выключатели не успевают нормально отработать.
  //delay( 100 );

  
}

/*
void loop() {
  static unsigned long LastMillis = millis();
  
  // *****************************************
  // Обработка быстрых устройств - каждый цикл

  // Обработка форточки
  Window.WindowPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());


  
  if( millis() - LastMillis > 100) {
    LastMillis = millis();

    // ****************************************************
    //Обработка медленных устройств - каждые 100 миллисекунд
    // Отработка меню
    nav.poll();
  
    // Запрос обновления значений датчиков на шине
    if( TSensors.UpdateSensorsOnBus() ){
      // Перерисовать Screensaver
      nav.idleChanged=true;
    }
    
    // Обработка вентилятора - земляного аккумулятора
    EarthFan.TerraAccumulatorPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());

  }
  
  // Если делать delay, то концевые выключатели не успевают нормально отработать.
  //delay( 100 );

  
}
//--------------------------------------------------------------------------------------

  */
