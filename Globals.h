// *************************************************************
// Пины на плате Ардуино
// Общего назначения
#define BTN_SEL 48//6 //48				// Кнопка Ввод
#define BTN_UP 50//7 //50				// Кнопка Вверх
#define BTN_DOWN 52//8 //52				// Кнопка Вниз
#define ONE_WIRE_BUS 38			// Шина One Wire

// Группа Концевых выключателей - пины 22-27
#define LS_WINDOW_ROOF_CLOSED	44		// Концевой выключатель форточки на крыше - закрыта
#define LS_WINDOW_ROOF_OPEN		46		// Концевой выключатель форточки на крыше - открыта
// ---------------------------------------------------------------------------------------SWAP-----------------------------------------------------------------

// Герконы для поплавка в бочке - пины 28-29
#define GC_BARREL_FULL			40		// Геркон наполненной бочки
#define GC_BARREL_EMPTY			42		// Геркон опустевшей бочки
// ---------------------------------------------------------------------------------------SWAP-----------------------------------------------------------------

// Группа светодиодов-индикаторов - пины 38-45
#define LED_FAN 41				// Индикатор работы вентилятора
#define LED_FAN_MANUAL_MODE 39  // Индикатор ручного режима вентилятора
#define L3 43
#define L4 45

// Группа Реле - пины 46-53
#define RELAY_EARTH_FAN		3	// Реле №1 - вентилятор земляного аккумулятора
#define RELAY_BARREL_PUMP	4	// Реле №2 - насос наполнения бочки
#define R3 5
#define R4 6
#define R5 7
#define R6 8
#define RELAY_WINDOW_1		1	// Реле №7 - Реле1 управления окнами
#define RELAY_WINDOW_2		2	// Реле №8 - Реле2 управления окнами


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
