#include "gh_Logger.h"



//---------------------------------------------------------------------
// Constructor
Logger::Logger(byte pinSD) {
  byte i;
  SDPin = pinSD;
}

Logger::Logger(byte pinSD, byte pinErrorLed) {
  byte i;
  SDPin = pinSD;
  ErrorLedPin = pinErrorLed;
  pinMode(ErrorLedPin, OUTPUT);
 }

//---------------------------------------------------------------------
// Begin

bool Logger::Begin(gh_RTC *gh_rtc) {
  
  rtc = gh_rtc;
  if (!SD.begin(SDPin)) {
    LOG("Could not initialize SDCard");
    bErrFlag = true;
    return false;
  }
  LOG("SD Card initialized");

  // Устанавливаем имя файла для журнала активности - каждый запуск новый файл либо если в течение одного дня, то туда же писать.
  strcpy(ActivityLogFile, rtc->now().toString(ActivityLogFileNamePattern));
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);
  if (!currentFile) {
    LOG("Could not open the file for write");
    return false;
  }
  currentFile.close();

  // Устанавливаем имя файла для журнала активности - каждый запуск новый файл либо если в течение одного дня, то туда же писать.
  strcpy(SensorsLogFile, rtc->now().toString(SensorsLogFileNamePattern));
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);
  if (!currentFile) {
    LOG("Could not open the file for write");
    return false;
  }
  currentFile.close();

  LOG("Log files opened successfully!")
  
  // Делаем запись в журнале активности о запуске
  RecordActivity(DEV_BOARD, EVT_BOARD_ON, S_EVT_BOARD_ON_JUSTON, 0, 0);
  return true;
}


//---------------------------------------------------------------------
// RecordActivity

void Logger::RecordActivity(uint8_t DeviceID, uint8_t EventID, uint8_t SubEventID, int8_t Param1, int8_t Param2) {
  char DTFormat[20] = "YYYY-MM-DD;hh:mm:ss";
  static String Delim = ";";
  
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);

  if (!currentFile) {
    LOG("ERROR!!! Could not open the file for write");
    bErrFlag = true;
    return;
  }
  bErrFlag = false;
  currentFile.print(rtc->now().toString(DTFormat) + Delim);
  currentFile.print(DeviceID + Delim + EventID + Delim + SubEventID + Delim);
  currentFile.println(Param1 + Delim + Param2);
  
  LOG((String)"Recording activity: " + rtc->now().toString(DTFormat) + Delim + DeviceID + Delim + EventID + Delim + SubEventID + Delim + Param1 + Delim + Param2);
  currentFile.close();
}

//---------------------------------------------------------------------
// Запись в журнал значений наблюдений

void Logger::RecordSensors(float TEarth, float TAir, float TBoard, float TOut, float TEarth2, float T_TAFanIn, float T_TAFanOut) {
  char DTFormat[20] = "YYYY-MM-DD;hh:mm:ss";
  static String Delim = ";";
  String sRecord = "";
  
  currentFile = SD.open(SensorsLogFile, FILE_WRITE);

  if (!currentFile) {
    LOG("ERROR!!! Could not open the file for write");
    bErrFlag = true;
    return;
  }
  bErrFlag = false;
  //currentFile.print(rtc->now().toString(DTFormat) + Delim);
  sRecord = rtc->now().toString(DTFormat) + Delim;
  sRecord += FtoS(TEarth) + Delim;
  sRecord += FtoS(TAir) + Delim;
  sRecord += FtoS(TBoard) + Delim;
  sRecord += FtoS(TOut) + Delim;
  sRecord += FtoS(TEarth2) + Delim;
  sRecord += FtoS(T_TAFanIn) + Delim;
  sRecord += FtoS(T_TAFanOut);

  currentFile.println(sRecord);
  LOG((String)"Recording sensors: " + sRecord);

  currentFile.close();
  
}

//---------------------------------------------------------------------
// Logger Poll

void Logger::Poll() {
  static unsigned long previousMillis = 0;

  if(bErrFlag) {

    //проверяем не прошла ли 1 секунда
    if(millis() - previousMillis > 1000) {
      // сохраняем время последнего переключения
      previousMillis = millis(); 
   
      // если светодиод не горит, то зажигаем, и наоборот
      bErrorLedOn = !bErrorLedOn;
      digitalWrite(ErrorLedPin, bErrorLedOn);
    }
  } else if (bErrorLedOn) {
    // Если ошибки нет и диод горит, то выключаем его. Такой ситуации быть не должно, но вдруг.
    bErrorLedOn = !bErrorLedOn;
    digitalWrite(ErrorLedPin, bErrorLedOn);
  }
}



//---------------------------------------------------------------------
// FtoS - преобразование из float to String

String Logger::FtoS(float val) {
    char cBuf[8];
    String sBuf;

    dtostrf(val, 7, 2, cBuf);  // выводим в строку 7 символов, 2 разряда после запятой
    sBuf = (String)cBuf;
    sBuf.trim();
    return sBuf;
}
