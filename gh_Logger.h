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
#include "gh_LoggerEvents.h"
#include <SD.h>
#include "gh_rtc.h"

class Logger
{
  public:
  Logger(uint8_t pinSD);//, char File[12]);
  Logger(uint8_t pinSD, /*char File[12], */byte pinErrorLed);
  void RecordActivity(uint8_t DeviceId, uint8_t EventID, uint8_t SubEventID, int8_t Param1, int8_t Param2);   // Запись в журнал активности
  void RecordSensors(float TEarth, float TAir, float TBoard, float TOut, float TEarth2, float T_TAFanIn, float T_TAFanOut); // Запись в журнал наблюдений
  void Poll();
  bool Begin(gh_RTC *rtc);                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  //bool Begin();                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  
  private:
  String FtoS(float val);           // Преобразование Float to String
  
  char ObjectName[7] = "Logger";    // используется для логгера
  File currentFile;               // Открытый файл
  gh_RTC *rtc;                    // Ссылка на объект RTC
  uint8_t SDPin;
  
  bool bErrFlag = false;
  uint8_t ErrorLedPin;
  bool bErrorLedOn = false;
  
  char ActivityLogFile[13];
  char SensorsLogFile[13];
  
  uint8_t CurrentDayOfMonth;           // Текущий день месяца.
  
  char ActivityLogFileNamePattern[13] = "acYYMMDD.csv";
  char SensorsLogFileNamePattern[13] = "seYYMMDD.csv";
};

#endif
