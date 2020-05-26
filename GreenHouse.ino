#include <Arduino.h>
#include <Wire.h>
#include <SD.h>

#include "Globals.h"
#include "gh_menu.h"
#include "gh_TSensors.h"
#include "gh_EarthFan.h"
#include "gh_RTC.h"
#include "gh_windows.h"
#include "gh_Logger.h"


//---------------------------------------------------------------------
//----------------------- GLOBAL OBJECTS --------------------------------

T_Sensors TSensors(ONE_WIRE_BUS);
Earth_Fan EarthFan(RELAY_EARTH_FAN, LED_FAN, LED_FAN_MANUAL_MODE );
gh_RTC rtc;
GHWindow Window;
Logger lg(SDCARD);//, LOG_FILE_NAME);


//---------------------------------------------------------------------
//----------------------------- SETUP  --------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Green House project. Created by Anton Kostrov, 2020"); 
  Serial.flush();
  delay(3000); // wait for console opening

  TSensors.Begin(1); // Интервал опроса датчиков на шине 1 минута
  EarthFan.Begin();
  if (! rtc.begin()) LOG("Couldn't find RTC");  // Запускаем часы реального времени


  if (! lg.Begin(&rtc)) LOG("Couldn't initialize SDCard");   // Инициализация логгера
//  if (! lg.Begin()) LOG("Couldn't initialize SDCard");
    
  // Инициализация меню
  joystickBtns.begin();
  lcd.begin(20, 4); // Запускаем экран
  lcd.setBacklight(255);
  nav.idleTask = ScreenSaver; //Устанавливаем функцию скринсейвера
  nav.showTitle = false;
  nav.timeOut = 10;   // seconds to start screensaver
  options=&myMenuOptions; // Устанавливаем свои значения глобальных Options для меню

  // Инициализируем окно
  GHWindowHardwareConfig WinConfig;
  WinConfig.PinRelayPow           = RELAY_WINDOW_POW;
  WinConfig.PinRelay1             = RELAY_WINDOW_1;
  WinConfig.PinRelay2             = RELAY_WINDOW_2;
  WinConfig.PinLimitSwitchOpen    = LS_WINDOW_ROOF_OPEN;
  WinConfig.PinLimitSwitchClosed  = LS_WINDOW_ROOF_CLOSED;
  WinConfig.PinWindowMotorLed     = LED_WINDOW;
  WinConfig.PinWindowModeLed      = LED_WINDOW_MANUAL_MODE;
  if (! Window.Begin(WinConfig)) LOG("Couldn't initialize window");

  // Печатаем приветствие
  lcd.setCursor(0, 0);
  //lcd.print("Попробуем?");
  lcd.setCursor(0, 2);
  lcd.print("Does it work?");
  delay(1000);

  lcd.setCursor(0, 3);
  lcd.print("Seems, yes!");
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
    
    // Обработка логгера
    //lg.LoggerPoll();
    
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
