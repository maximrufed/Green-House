#include "gh_Logger.h"



//---------------------------------------------------------------------
// Constructor
Logger::Logger(byte pinSD) {//, char File[12]) {
  byte i;
  SDPin = pinSD;
  //for(i=0;i<12;i++) {
  //  LogFileSuffix[i] = File[i];
  //}
}

Logger::Logger(byte pinSD, /*char File[12],*/ byte pinErrorLed) {
  byte i;
  SDPin = pinSD;
  //for(i=0;i<12;i++) {
  //  LogFileSuffix[i] = File[i];
  //}
  ErrorLedPin = pinErrorLed;
 }

//---------------------------------------------------------------------
// Begin

bool Logger::Begin(gh_RTC *gh_rtc) {
  
  rtc = gh_rtc;
  if (!SD.begin(SDPin)) {
    LOG("Could not initialize SDCard");
    return false;
  }
  LOG("SD Card initialized");

  // Устанавливаем имя файла - каждый запуск новый файл либо если в течение одного дня, то туда же писать.
  strcpy(ActivityLogFile, rtc->now().toString(ActivityLogFileNamePattern));

  currentFile = SD.open(ActivityLogFile, FILE_WRITE);
  
  if (!currentFile) {
    LOG("Could not open the file for write");
    return false;
  }
  LOG("Log file opened successfully!")
  currentFile.close();
  
  // Делаем запись в журнале активности о запуске
  RecordActivity(EVT_DEV_START, ObjectName, "Start operation");
  return true;
}


//---------------------------------------------------------------------
// RecordActivity

void Logger::RecordActivity(uint16_t EventCode, char *gh_Object, char *EventText) {
  char DTFormat[21] = "YYYY-MM-DD;hh:mm:ss;";
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);

  if (!currentFile) {
    LOG("ERROR!!! Could not open the file for write");
    return;
  }
  currentFile.println(rtc->now().toString(DTFormat) + (String)EventCode + (String)";" + (String)gh_Object + (String)";" + (String)EventText);
  LOG((String)"Recording activity: " + rtc->now().toString(DTFormat) + (String)EventCode + (String)";"  + (String)gh_Object + (String)";" + (String)EventText);
  currentFile.close();
}

//---------------------------------------------------------------------
// Logger Poll

void Logger::LoggerPoll() {
  
}
