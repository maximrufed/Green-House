// -----------------------------------------------------------------------
// Класс Logger - регистрирует любые события в журналах
//
// Устройства:  SDCard, вебсервер (в проекте)
//        индикаторы: ошибка (в проекте)
//
// -----------------------------------------------------------------------

#ifndef GH_LOGGER_H
#define GH_LOGGER_H
#include <Arduino.h>
#include "globals.h"
#include <SD.h>
#include "gh_rtc.h"

class Logger
{
  public:
  Logger(uint8_t pinSD);//, char File[12]);
  Logger(uint8_t pinSD, /*char File[12], */byte pinErrorLed);
  void RecordActivity(uint16_t EventCode, char *gh_Object, char *EventText);            // Запись в журнал
  void LoggerPoll();
  bool Begin(gh_RTC *rtc);                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  //bool Begin();                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  
  private:
  char ObjectName[7] = "Logger";    // используется для логгера
  File currentFile;               // Открытый файл
  gh_RTC *rtc;                    // Ссылка на объект RTC
  uint8_t SDPin;
  uint8_t ErrorLedPin;
  char ActivityLogFile[13];
  char SensorsLogFile[13];
  bool bIsError;
  uint8_t CurrentDayOfMonth;           // Текущий день месяца.
  
  char ActivityLogFileNamePattern[13] = "acYYMMDD.csv";
  char SensorsLogFileNamePattern[13] = "seYYMMDD.csv";
};

#endif
