#include "gh_logger.h"



//---------------------------------------------------------------------
// Constructor
logger::logger(byte pinSD) {
  byte i;
  SDPin = pinSD;
}

logger::logger(byte pinSD, byte pinErrorLed) {
  byte i;
  SDPin = pinSD;
  ErrorLedPin = pinErrorLed;
  pinMode(ErrorLedPin, OUTPUT);
 }

//---------------------------------------------------------------------
// Begin

bool logger::Begin(gh_RTC *gh_rtc) {

  LOG("Logger Begin");
  rtc = gh_rtc;

  // Устанавливаем имя файла для журналов - каждый день новый файл. 
  SetFileNames();

  if (!SD.begin(SDPin)) {
    LOG("Could not initialize SDCard");
    bErrFlag = true;
    return false;
  }
  LOG("SD Card initialized");

  // Открываем журнал активности
  currentFile = SD.open(ActivityLogFile, FILE_WRITE);
  if (!currentFile) {
    LOG("Could not open the file for write");
    return false;
  }
  currentFile.close();

  // Открываем журнал наблюдений
  currentFile = SD.open(SensorsLogFile, FILE_WRITE);
  if (!currentFile) {
    LOG("Could not open the file for write");
    return false;
  }
  currentFile.close();

  LOG("LOG files opened successfully!");
  
  // Делаем запись в журнале активности о запуске
  RecordActivityInt(DEV_BOARD, EVT_BOARD_ON, S_EVT_BOARD_ON_JUSTON, 0, 0);
  /*
   * пробуем наладить работу со строковыми переменными
  String s1 = "qq";
  String s2 = "ww";
  RecordActivityStr(DEV_BOARD, 255, 255, s1, s2);//rtc->now().toString(SensorsLogFileNamePattern),rtc->now().toString(SensorsLogFileNamePattern));
  RecordActivityChar(DEV_BOARD, 255, 255, ActivityLogFile, SensorsLogFile);
  */
  return true;
}


//---------------------------------------------------------------------
// RecordActivityInt

void logger::RecordActivityInt(uint8_t DeviceID, uint8_t EventID, uint8_t SubEventID, int8_t Param1, int8_t Param2) {
  static String Delim = ";";
  String sRecord = "";

  sRecord = (String)DeviceID + Delim + (String)EventID + Delim + (String)SubEventID + Delim;
  sRecord += (String)Param1 + Delim + (String)Param2;

  FileSizeActivity = SaveRecordToFile(ActivityLogFile, FileSizeActivity, sRecord);
 
}

void logger::RecordActivityChar(uint8_t DeviceID, uint8_t EventID, uint8_t SubEventID, char *Param1, char *Param2) {
  static String Delim = ";";
  String sRecord;
  String sParam1 = Param1;
  String sParam2 = Param2;

  //LOG(sParam2.length());
  sRecord = (String)DeviceID + Delim + (String)EventID + Delim + (String)SubEventID + Delim;
  sRecord += sParam1 + Delim;
  sRecord += sParam2;
  LOG(sRecord);
  FileSizeActivity = SaveRecordToFile(ActivityLogFile, FileSizeActivity, sRecord);
  LOG("!!!");
}
void logger::RecordActivityStr(uint8_t DeviceID, uint8_t EventID, uint8_t SubEventID, String Param1, String Param2) {
  static String Delim = ";";
  String sRecord = "";

  sRecord = (String)DeviceID + Delim + (String)EventID + Delim + (String)SubEventID + Delim;
  sRecord += (String)Param1 + Delim + (String)Param2;

  FileSizeActivity = SaveRecordToFile(ActivityLogFile, FileSizeActivity, sRecord);

}
//---------------------------------------------------------------------
// Запись в журнал значений наблюдений

void logger::RecordSensors(float TEarth, float TAir, float TBoard, float TOut, float TEarth2, float T_TAFanIn, float T_TAFanOut) {
  static String Delim = ";";
  String sRecord = "";
  
  sRecord = FtoS(TEarth) + Delim;
  sRecord += FtoS(TAir) + Delim;
  sRecord += FtoS(TBoard) + Delim;
  sRecord += FtoS(TOut) + Delim;
  sRecord += FtoS(TEarth2) + Delim;
  sRecord += FtoS(T_TAFanIn) + Delim;
  sRecord += FtoS(T_TAFanOut);
  
  FileSizeSensors = SaveRecordToFile(SensorsLogFile, FileSizeSensors, sRecord);
  
}

//---------------------------------------------------------------------
// logger Poll

void logger::Poll() {
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

String logger::FtoS(float val) {
    char cBuf[8];
    String sBuf;

    dtostrf(val, 7, 2, cBuf);  // выводим в строку 7 символов, 2 разряда после запятой
    sBuf = (String)cBuf;
    sBuf.setCharAt(4,',');     // меняем десятичный разделитель на запятую
    sBuf.trim();         
    return sBuf;
}

//---------------------------------------------------------------------
// SDReinit - попытка рестарта SDCard

void logger::SDReinit() {
  LOG("Trying to reinitialize SDCard");
  if (!SD.begin(SDPin)) {
    LOG("Could not initialize SDCard");
    setErr(true, S_EVT_BOARD_SD_INITFAIL);
  } else {
    LOG("SD Card reinitialized");
    setErr(false, S_EVT_BOARD_SD_REINIT);
  }
}

//---------------------------------------------------------------------
// setErr

void logger::setErr(bool bErr, uint8_t errLoggerSubEventCode) {
  bErrFlag = bErr;
  //Такой вызов ведет к бесконечной рекурсии RecordActivityInt(DEV_BOARD, EVT_BOARD_SD, errLoggerSubEventCode, 0, 0);
}

//---------------------------------------------------------------------
// SaveRecordToFile

uint32_t logger::SaveRecordToFile(char *FileName, uint32_t prevFileSize, String sRecord) {
  uint32_t newFileSize;
  char DTFormat[20] = "YYYY-MM-DD;hh:mm:ss;";
  String sFullRecord;

  // Добавляем дату и время в запись журнала
  sFullRecord = (String)rtc->now().toString(DTFormat) + sRecord;

  // Если текущая дата не совпадает с датой в имени файла, 
  // то обновляем имена файлов - начинаем новые логи каждый день
  if( FileNameDate != rtc->now().day() ) SetFileNames();
  
  currentFile = SD.open(FileName, FILE_WRITE);
  if (!currentFile) {
    LOG("Failed to open file");
    setErr(true, S_EVT_BOARD_SD_ERRFILEOPEN);
    SDReinit();
    return 0;
  }
  bErrFlag = false;
  if(currentFile.fileSize() != prevFileSize) {
    newFileSize = currentFile.fileSize();
  } else {
    LOG("ERROR!!! File size is not growing");
    LOG("Trying to reinitialize");
    setErr(true, S_EVT_BOARD_SD_ERRFILESIZE);
    SDReinit();
  }

  currentFile.println(sFullRecord);
  LOG((String)"Recording " + FileName + ": " + sFullRecord);

  if(!currentFile.sync()) SDReinit();
  if(!currentFile.close()) SDReinit();

  return newFileSize;
  
}

//---------------------------------------------------------------------
// SetFileNames

void logger::SetFileNames( void ) {
  char buf[13];

  LOG("logger::SetFileNames");
  FileNameDate = rtc->now().day();
  strcpy(buf,ActivityLogFileNamePattern);
  strcpy(ActivityLogFile, rtc->now().toString(buf));
  LOG(ActivityLogFile);
  strcpy(buf,SensorsLogFileNamePattern);
  strcpy(SensorsLogFile, rtc->now().toString(buf));
  LOG(SensorsLogFile);
  
}
