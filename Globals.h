// *************************************************************
// Пины на плате Ардуино
// *************************************************************

//#define PRODUCTION_ENV  // uncomment to switch to TEST environment

#ifdef PRODUCTION_ENV
  // **************************************************
  // Пины для продуктива
  // **************************************************

  // Плата управления
  #define BTN_UP                  8  // Кнопка Вверх
  #define BTN_DOWN                7    // Кнопка Вниз
  #define BTN_SEL                 6  // Кнопка Ввод
  
  // Плата светодиодов
  #define LED_FAN_MANUAL_MODE     28  // Индикатор ручного режима вентилятора
  #define LED_FAN                 28  // Индикатор работы вентилятора
  #define LED_WINDOW_MANUAL_MODE  28  // Индикатор ручного режима окна
  #define LED_WINDOW              28  // Индикатор работы мотора окна
  #define L3                      28
  #define L3                      49
  #define L3                      51
  #define L3                      53
  
  // Плата вспомогательная бортовая - к ней коммунируется UTP8 с датчиками и форточкой
  #define GC_BARREL_FULL          23    // Геркон наполненной бочки
  #define GC_BARREL_EMPTY         25    // Геркон опустевшей бочки
  #define LS_WINDOW_ROOF_CLOSED   27    // Концевой выключатель форточки на крыше - закрыта
  #define LS_WINDOW_ROOF_OPEN     29    // Концевой выключатель форточки на крыше - открыта
  #define ONE_WIRE_BUS            31    // Шина One Wire
  #define UTP_GREEN_WHITE         33    // ********* Пока не задействован
  
  
  // Плата Реле 
  #define R1						40 // Похоже, что это реле сгорело :(
  #define RELAY_WINDOW_1          46 // Реле №2 - Реле1 управления окнами
  #define RELAY_WINDOW_2          47 // Реле №3 - Реле2 управления окнами
  #define RELAY_EARTH_FAN		    43 // Реле №4 - вентилятор земляного аккумулятора
  #define RELAY_BARREL_PUMP	    44 // Реле №5 - насос наполнения бочки
  #define RELAY_WATER_VALVE1      41 // Реле №6 - клапан полива линии №1
  #define RELAY_WATER_VALVE2      42 // Реле №7 - клапан полива линии №2
  #define R8 47

#else
  // **************************************************
  // Пины для тестового контура
  // **************************************************

  // Плата управления
  #define BTN_UP                  8  // Кнопка Вверх
  #define BTN_DOWN                7  // Кнопка Вниз
  #define BTN_SEL                 6  // Кнопка Ввод
  
  // Плата светодиодов
  #define LED_FAN_MANUAL_MODE     30  // Индикатор ручного режима вентилятора
  #define LED_FAN                 32  // Индикатор работы вентилятора
  #define LED_WINDOW_MANUAL_MODE  36  // Индикатор ручного режима окна
  #define LED_WINDOW              34  // Индикатор работы мотора окна
  #define L3                      31
  #define L3                      31
  #define L3                      31
  #define L3                      31
  
  // Плата вспомогательная бортовая - к ней коммунируется UTP8 с датчиками и форточкой
  #define GC_BARREL_FULL          23    // Геркон наполненной бочки
  #define GC_BARREL_EMPTY         25    // Геркон опустевшей бочки
  #define LS_WINDOW_ROOF_CLOSED   27    // Концевой выключатель форточки на крыше - закрыта
  #define LS_WINDOW_ROOF_OPEN     29    // Концевой выключатель форточки на крыше - открыта
  #define ONE_WIRE_BUS            31    // Шина One Wire
  #define UTP_GREEN_WHITE         33    // ********* Пока не задействован
  
  
  // Плата Реле 
  #define R1                      40 
  #define RELAY_WATER_VALVE1      41 // Реле №2 - клапан полива линии №1
  #define RELAY_WATER_VALVE2      42 // Реле №3 - клапан полива линии №2
  #define RELAY_EARTH_FAN         43 // Реле №4 - вентилятор земляного аккумулятора
  #define RELAY_BARREL_PUMP       44 // Реле №5 - насос наполнения бочки
  #define RELAY_WINDOW_POW        45 // Реле №6 - Подача питания на мотор окна
  #define RELAY_WINDOW_1          46 // Реле №7 - Реле1 управления реверсом окна
  #define RELAY_WINDOW_2          47 // Реле №8 - Реле2 управления реверсом окна

  // SDCard
  #define SDCARD                  4  // Модуль SD Карточки
  #define LOG_FILE_NAME           "gh_" // Начало имени файла для журнала - из него получится вот такое имя gh_20200525.csv
#endif


// *************************************************************
// *************************************************************
// Адреса устройств на шине ONE_WIRE
#define TS_EARTH_ADR { 0x28, 0x74, 0x23, 0x43, 0x98, 0x0E, 0x00, 0x55 }	// адрес датчика Т-ры Земли Грядка1
#define TS_AIR_ADR { 0x28, 0xFF, 0x0A, 0xF0, 0x6D, 0x18, 0x01, 0xF5 }  // адрес датчика Т-ры Воздуха
#define TS_BOARD_ADR { 0x28, 0xFF, 0xBE, 0xDF, 0x6D, 0x18, 0x01, 0x4F }  // адрес датчика Т-ры прибора
#define TS_OUT_ADR { 0x28, 0xFF, 0x67, 0xE3, 0x6D, 0x18, 0x01, 0xB8 }  // адрес датчика Т-ры Снаружи
#define TS_EARTH_ADR2 { 0x28, 0x9B, 0x12, 0x43, 0x98, 0x01, 0x02, 0xB5 }  // адрес датчика Т-ры Земли Грядка2
#define TS_TA_FAN_IN_ADR { 0x28, 0xFF, 0x6A, 0xC9, 0x6D, 0x1F, 0x01, 0x3F }  // адрес датчика Т-ры Земляного Аккумулятора на входе
#define TS_TA_FAN_OUT_ADR { 0x28, 0xFF, 0xC1, 0xDD, 0x6D, 0x18, 0x01, 0x4A }  // датчика Т-ры Земляного Аккумулятора на выходе

// Адреса устройств на шине I2C
#define LCD_ADR 0x27 // 'PCF8574 8-Bit I/O Expander' OR 'LCM1602 LCD Adapter '
#define EEPROM_ADR 0x50 // 'AT24C32 EEPROM'
#define RTC_ADR 0x68 // 'DS3231 real-time clock' Определяем для порядка. На самом деле адрес прошит в базовой библиотеке RTCLib.h

// Глобальные настройки конфигурации
#define WINDOW_MOTOR_MAX_WORK_MILLIS 1500		// Максимальное время работы мотора открывания окна

// Лог в монитор серийного порта для отладки
#define LOGGING  // uncomment to turn on Serial monitor
//#define LOGGING_EARTH_FAN						// Снять комментарии для включения логирования объекта EARTH_FAN





#ifdef LOGGING
#define LOG(str) Serial.println(str);
#else
#define LOG(str) (str)
#endif

#ifdef LOGGING_EARTH_FAN
#define LOG_EA(str) Serial.println(str);
#else
#define LOG_EA(str) (str)
#endif
