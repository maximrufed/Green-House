// *************************************************************
// Пины на плате Ардуино
// Общего назначения
#define BTN_SEL 6				// Кнопка Ввод
#define BTN_UP 8				// Кнопка Вверх
#define BTN_DOWN 7				// Кнопка Вниз
#define ONE_WIRE_BUS 2			// Шина One Wire

// Группа Концевых выключателей - пины 22-27
#define LS_WINDOW_ROOF_CLOSED	22		// Концевой выключатель форточки на крыше - закрыта
#define LS_WINDOW_ROOF_OPEN		23		// Концевой выключатель форточки на крыше - открыта

// Герконы для поплавка в бочке - пины 28-29
#define GC_BARREL_FULL			28		// Геркон наполненной бочки
#define GC_BARREL_EMPTY			29		// Геркон опустевшей бочки

// Группа светодиодов-индикаторов - пины 38-45
#define LED_FAN 38				// Индикатор работы вентилятора
#define LED_FAN_MANUAL_MODE 40	// Индикатор ручного режима вентилятора
#define L3 42
#define L4 44

// Группа Реле - пины 46-53
#define RELAY_EARTH_FAN		46	// Реле №1 - вентилятор земляного аккумулятора
#define RELAY_BARREL_PUMP	47	// Реле №2 - насос наполнения бочки
#define R3 48
#define R4 49
#define R5 50
#define R6 51
#define RELAY_WINDOW_1		52	// Реле №7 - Реле1 управления окнами
#define RELAY_WINDOW_2		53	// Реле №8 - Реле2 управления окнами


// *************************************************************
// Адреса устройств на шине ONE_WIRE
#define TS_EARTH_ADR { 0x28, 0x9B, 0x12, 0x43, 0x98, 0x01, 0x02, 0xB5 }	// адрес датчика Т-ры Земли
#define TS_AIR_ADR { 0x28, 0xFF, 0xBE, 0xDF, 0x6D, 0x18, 0x01, 0x4F }	// адрес датчика Т-ры Воздуха

// Адреса устройств на шине I2C
#define LCD_ADR 0x27 // 'PCF8574 8-Bit I/O Expander' OR 'LCM1602 LCD Adapter '
#define EEPROM_ADR 0x50 // 'AT24C32 EEPROM'
#define RTC_ADR 0x68 // 'DS3231 real-time clock' Определяем для порядка. На самом деле адрес прошит в базовой библиотеке RTCLib.h

// Глобальные настройки конфигурации
#define WINDOW_MOTOR_MAX_WORK_MILLIS 1500		// Максимальное время работы мотора открывания окна

// Лог в монитор серийного порта для отладки
#define LOGGING  // uncomment to turn on Serial monitor

#ifdef LOGGING
#define LOG(str) Serial.println(str);
#else
#define LOG(str) (str)
#endif
