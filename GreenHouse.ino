#include <Arduino.h>
#include <Wire.h>

#include "Globals.h"
#include "gh_menu.h"
#include "gh_TSensors.h"
#include "gh_EarthFan.h"
#include "gh_RTC.h"
#include "gh_windows.h"
#include "gh_logger.h"
#include "gh_Watering.h"
#include "gh_Config.h"

//---------------------------------------------------------------------
//----------------------- GLOBAL OBJECTS --------------------------------

T_Sensors TSensors(ONE_WIRE_BUS);
Earth_Fan EarthFan(RELAY_EARTH_FAN, LED_FAN, LED_FAN_MANUAL_MODE );
gh_RTC rtc;
gh_Barrel WaterTank;
gh_WaterLine WateringLine1;
gh_WaterLine WateringLine2;
GHWindow Window;
logger lg(SDCARD, LED_SD_ERROR);//, LOG_FILE_NAME);
gh_Config ControllerConfiguration;


//---------------------------------------------------------------------
//----------------------------- SETUP  --------------------------------

void setup() {
  String LCDMessage="";

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Green House project. Created by Anton Kostrov, 2020");
  Serial.flush();
  delay(3000);
  char buf[9]="--------";
  LOG((String)buf+(String)buf+(String)buf);
  LOG((String)buf+"SETUP START"+(String)buf);
  LOG((String)buf+(String)buf+(String)buf);

  // Инициализация меню
  joystickBtns.begin();
  //lcd.begin(20, 4); // Запускаем экран
  lcd.init();
  lcd.setBacklight(255);
  nav.idleTask = ScreenSaver; //Устанавливаем функцию скринсейвера
  nav.showTitle = false;
  nav.timeOut = 10;   // seconds to start screensaver
  options = &myMenuOptions; // Устанавливаем свои значения глобальных Options для меню

  // Запускаем часы реального времени
  if (!rtc.begin()) {
    LOG("Couldn't find RTC");  
    LCDMessage = "RTC FAIL!";
  } else {
    LCDMessage = "RTC Ok";
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(LCDMessage);
  delay(1000);

  lcd.setCursor(0, 2);
  if (!lg.Begin(&rtc)) {
    LOG("Couldn't initialize SDCard");   // Инициализация логгера
    LCDMessage = "SDCard FAIL!";
  } else {
    LCDMessage = "SDCard Ok";
  }
  lcd.setCursor(0, 1);
  lcd.print(LCDMessage);
  delay(1000);
  

  // Инициализируем окно
  GHWindowHardwareConfig WinConfig;
  WinConfig.PinRelayPow           = RELAY_WINDOW_POW;
  WinConfig.PinRelay1             = RELAY_WINDOW_1;
  WinConfig.PinRelay2             = RELAY_WINDOW_2;
  WinConfig.PinLimitSwitchClosed  = LS_WINDOW_ROOF_CLOSED;
  WinConfig.PinWindowMotorLed     = LED_WINDOW;
  WinConfig.PinWindowModeLed      = LED_WINDOW_MANUAL_MODE;
  if (! Window.Begin(WinConfig)) {
    LOG("Couldn't initialize SDCard");   // Инициализация логгера
    LCDMessage = "Window FAIL!";
  } else {
    LCDMessage = "Window Ok";
  }
  lcd.setCursor(0, 2);
  lcd.print(LCDMessage);
  delay(1000);

  // Инициализируем бочку
  GHBarrelHardwareConfig WTConfig;
  WTConfig.RelayPin           = RELAY_BARREL_PUMP;
  WTConfig.ModeLedPin         = LED_WT_MANUAL_MODE;
  WTConfig.ValveOpenLedPin    = LED_WT_FILLING;
  WTConfig.FullDetectorPin    = GC_BARREL_FULL;
  WTConfig.EmptyDetectorPin   = GC_BARREL_EMPTY;

  if (! WaterTank.Begin(WTConfig)) {
    LOG("Couldn't initialize WaterTank");   // Инициализация логгера
    LCDMessage = "Water Tank FAIL!";
  } else {
    LCDMessage = "Water Tank Ok";
  }
  lcd.setCursor(0, 3);
  lcd.print(LCDMessage);
  delay(3000);
 
  lcd.clear();
  // Инициализируем линию полива 1
  WaterLineHardwareConfig WLConfig;
  WLConfig.RelayPin           = RELAY_WATER_VALVE1;
  WLConfig.ModeLedPin         = LED_WL1_MANUAL_MODE;
  WLConfig.ValveOpenLedPin    = LED_WL1_WATERING;

  if (! WateringLine1.Begin(WLConfig, rtc.now())) {
    LOG("Couldn't initialize Watering Line 1");   // Инициализация логгера
    LCDMessage = "Water Line1 FAIL!";
  } else {
    LCDMessage = "Water Line1 OK!";
  }

  lcd.setCursor(0, 0);
  lcd.print(LCDMessage);
  delay(1000);

  // Инициализируем линию полива 2
  WLConfig.RelayPin           = RELAY_WATER_VALVE2;
  WLConfig.ModeLedPin         = LED_WL2_MANUAL_MODE;
  WLConfig.ValveOpenLedPin    = LED_WL2_WATERING;

  if (! WateringLine2.Begin(WLConfig, rtc.now())) {
    LOG("Couldn't initialize Watering Line 1");   // Инициализация логгера
    LCDMessage = "Water Line2 FAIL!";
  } else {
    LCDMessage = "Water Line2 OK!";
  }

  lcd.setCursor(0, 1);
  lcd.print(LCDMessage);
  delay(1000);

  TSensors.Begin(1); // Интервал опроса датчиков на шине 1 минута
  EarthFan.Begin();
  ControllerConfiguration.Begin(&Window.Settings, &EarthFan.Settings, &WaterTank.Settings, &WateringLine1.Settings, &WateringLine2.Settings); // Инициализируем объект сохранения конфигурации
  lcd.setCursor(0, 2);
  lcd.print("Other devices OK");
  delay(1000);
  
  lcd.setCursor(0, 3);
  lcd.print("Startup complete");
  delay(3000);
  LOG("-------------------------");
  LOG("-----SETUP COMPLETE!-----");
  LOG("-------------------------");

}

long prevmill = 0;

//---------------------------------------------------------------------
//------------------------------ LOOP ---------------------------------


void loop() {

  // *****************************************
  // Обработка быстрых устройств - каждый цикл

  // Обработка форточки
  Window.WindowPoll((int8_t)TSensors.GetTEarth(), (int8_t)TSensors.GetTAir(), rtc.IsNight());

  // ****************************************************
  //Обработка медленных устройств - каждые 100 миллисекунд - надосделать
  // Отработка меню
  nav.poll();

  // Обработка логгера
  lg.Poll();

  // Запрос обновления значений датчиков на шине
  if ( TSensors.TSensorsPoll() ) {
    // Если значения датчиков обновились, что случается не часто, то перерисовать Screensaver
    nav.idleChanged = true;
  }

  // Обработка вентилятора - земляного аккумулятора
  EarthFan.TerraAccumulatorPoll(TSensors.GetTEarth(), TSensors.GetTAir(), rtc.IsNight());

  // Обработка бочки
  WaterTank.Poll(rtc.now().hour(), rtc.now().minute());

  // Обработка линий полива
  WateringLine1.Poll(rtc.now());
  WateringLine2.Poll(rtc.now());

  // Сохранение конфигурации в EEPROM и SDCard
  ControllerConfiguration.Poll(rtc.now().minute());
  // Если делать delay, то концевые выключатели не успевают нормально отработать.
  //delay( 100 );

}
