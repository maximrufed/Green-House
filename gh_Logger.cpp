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
  RecordActivity(DEV_BOARD, EVT_BOARD_ON, S_EVT_BOARD_ON_JUSTON, 0, 0);
  return true;
}


//---------------------------------------------------------------------
// RecordActivity

void Logger::RecordActivity(uint8_t DeviceID, uint8_t EventID, uint8_t SubEventID, int8_t TEarth, int8_t TAir) {
  char DTFormat[20] = "YYYY-MM-DD;hh:mm:ss";
  static String Delim = ";";
  
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);

  if (!currentFile) {
    LOG("ERROR!!! Could not open the file for write");
    return;
  }
  //currentFile.println(rtc->now().toString(DTFormat) + (String)EventCode + (String)";" + (String)gh_Object + (String)";" + (String)EventText);
  currentFile.print(rtc->now().toString(DTFormat) + Delim);
  currentFile.print(DeviceID + Delim);
  currentFile.print(EventID + Delim);
  currentFile.print(SubEventID + Delim);
  currentFile.print(TEarth + Delim);
  currentFile.println(TAir);
  

  //LOG((String)"Recording activity: " + rtc->now().toString(DTFormat) + (String)EventCode + (String)";"  + (String)gh_Object + (String)";" + (String)EventText);
  LOG((String)"Recording activity: " + rtc->now().toString(DTFormat) + Delim + DeviceID + Delim + EventID + Delim + SubEventID + Delim + TEarth + Delim + TAir);
  currentFile.close();
}

//---------------------------------------------------------------------
// Logger Poll

void Logger::LoggerPoll() {
  
}
