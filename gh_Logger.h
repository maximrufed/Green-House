#include <BlockDriver.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <sdios.h>
#include <SysCall.h>

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
#include <SPI.h>
#include <SdFat.h>
#include "gh_rtc.h"
//extern gh_RTC rtc;

  
class Logger
{
  public:
  Logger(uint8_t pinSD);//, char File[12]);
  Logger(uint8_t pinSD, /*char File[12], */byte pinErrorLed);

  
  void RecordActivityInt(uint8_t DeviceId, uint8_t EventID, uint8_t SubEventID, int8_t Param1, int8_t Param2);   // Запись в журнал активности
  void RecordActivityChar(uint8_t DeviceId, uint8_t EventID, uint8_t SubEventID, char *Param1, char *Param2);   // Запись в журнал активности
  void RecordActivityStr(uint8_t DeviceId, uint8_t EventID, uint8_t SubEventID, String Param1, String Param2);   // Запись в журнал активности
  void RecordSensors(float TEarth, float TAir, float TBoard, float TOut, float TEarth2, float T_TAFanIn, float T_TAFanOut); // Запись в журнал наблюдений
  void Poll();
  bool Begin(gh_RTC *rtc);                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  //bool Begin();                                                     // False если не удалось открыть файл и сделать в него запись о начале работы
  
  private:
  void SDReinit();
  void setErr(bool bErr, uint8_t errLoggerSubEventCode);
  uint32_t SaveRecordToFile(char *FileName, uint32_t prevFileSize, String sRecord);
  void SetFileNames();

  SdFat SD;
  File currentFile;               // Открытый файл

  String FtoS(float val);           // Преобразование Float to String
  
  char ObjectName[7] = "Logger";    // используется для логгера
  gh_RTC *rtc;                    // Ссылка на объект RTC
  uint8_t SDPin;
  
  bool bErrFlag = false;
  uint8_t ErrorLedPin;
  bool bErrorLedOn = false;
  
  char ActivityLogFile[13];
  char SensorsLogFile[13];
  uint32_t FileSizeActivity = 0;
  uint32_t FileSizeSensors = 0;
  
  uint8_t CurrentDayOfMonth;           // Текущий день месяца.
  
  char ActivityLogFileNamePattern[13] = "acYYMMDD.csv";
  char SensorsLogFileNamePattern[13] = "seYYMMDD.csv";
  byte FileNameDate;
};

#endif
