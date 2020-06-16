#include "gh_Watering.h"


//---------------------------------------------------------------------
// Begin
bool gh_Barrel::Begin(GHBarrelHardwareConfig HwCfg) {

  LOG("gh_Barrel Begin");

	// Загружаем сохраненные настройки работы
	Settings.StartFillingHour = BARREL_START_FILLING_HOUR;
	Settings.StartFillingMinute = BARREL_START_FILLING_MINUTE;
	Settings.MaxFillingMinute = BARREL_MAX_FILLING_MINUTES;

  // Инициализируем пины
  Cfg.RelayPin = HwCfg.RelayPin;
  Cfg.ValveOpenLedPin = HwCfg.ValveOpenLedPin;
  Cfg.ModeLedPin = HwCfg.ModeLedPin;
  Cfg.FullDetectorPin = HwCfg.FullDetectorPin;
  Cfg.EmptyDetectorPin = HwCfg.EmptyDetectorPin;
  
  if( Cfg.RelayPin != 0xFF)           pinMode(Cfg.RelayPin,         OUTPUT);
  if( Cfg.ValveOpenLedPin != 0xFF)    pinMode(Cfg.ValveOpenLedPin,  OUTPUT);
  if( Cfg.ModeLedPin != 0xFF)         pinMode(Cfg.ModeLedPin,       OUTPUT);
  if( Cfg.FullDetectorPin != 0xFF )   pinMode(Cfg.FullDetectorPin,  INPUT_PULLUP);
  if( Cfg.EmptyDetectorPin != 0xFF )  pinMode(Cfg.EmptyDetectorPin, INPUT_PULLUP);

  if(Cfg.RelayPin == 0xFF || Cfg.FullDetectorPin == 0xFF) { 
    // Минимальная конфигурация оборудования для управления бочкой - реле и геркон наполнения
    return false;
  }

	// Устанавливаем автоматический режим работы
	SetManualMode(false);
	
	// Закрываем клапан
	StopFilling();

  // Инициализация переменных
  millisStartFilling = 0;       // таймер ограничения максимального времени наполнения бочки
  
  bIsEmpty = false;
  bIsFull = false;

  return true;  
}

//---------------------------------------------------------------------
// Обработка логики работы бочки
void gh_Barrel::Poll(byte NowHour, byte NowMinute) {
	float DT;
  char buf[100];
  
  // 1. САМЫМ ПЕРВЫМ ДЕЛОМ
  // Если бочка наполнилась, то срочно остановить процесс, поставить состояние, продолжить обработку
  if( digitalRead(Cfg.FullDetectorPin) == LOW ) {
    if(!bIsFull) { // Если мы уже знаем, что бочка наполнена, то и делать ничего больше не будем
      lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_FULL, 0, 0); // Делаем запись в журнале активности
      bIsFull = true;
      StopFilling();      // СРОЧНО ЗАКРЫВАЕМ КЛАПАН - ПРЕКРАЩАЕМ НАПОЛНЯТЬ БОЧКУ!!!
    }
  } else if (bIsFull) { // Может быть уровень упал и пора отменить состояние наполненности??
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_NOTFULL, 0, 0); // Делаем запись в журнале активности
    bIsFull = false;
  }

  // 2. ДАЛЕЕ ОБНОВЛЯЕМ СОСТОЯНИЕ ОПУСТОШЕНИЯ
  if( digitalRead(Cfg.EmptyDetectorPin) == LOW ) { // Сработал датчик пустой бочки
    if( !bIsEmpty ) { // Если мы уже знаем, что бочка пуста, то и делать ничего больше не будем
      lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_EMPTY, 0, 0); // Делаем запись в журнале активности
      bIsEmpty = true;
    }
  } else if (bIsEmpty) { // Может быть уровень повысился и пора отменить состояние пустоты??
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_NOTEMPTY, 0, 0); // Делаем запись в журнале активности
    bIsEmpty = false;
  }
  
  // 3. Если бочка наполняется, то проверяем на предельную продолжительность наполнения, выключаемся если надо. Выходим
  if( IsFilling() and (int)((millis()-millisStartFilling)/1000/60) > Settings.MaxFillingMinute) {
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_MAXFILLTIMEEXCEEDED, 0, 0); // Делаем запись в журнале активности
    StopFilling();      // СРОЧНО ЗАКРЫВАЕМ КЛАПАН - ПРЕКРАЩАЕМ НАПОЛНЯТЬ БОЧКУ!!!
    return;
  }

	// 4. ДАЛЕЕ, Если бочка работает в ручном режиме, то выходим без обработки
	if(IsManualMode()) return;

  // 5. И НАКОНЕЦ ОБРАБОТКА АВТОМАТИЧЕСКОГО РЕЖИМА
  
  // Если бочка опустела, сразу начинаем наполнять никого не спрашиваем. Выходим
  if( bIsEmpty and !IsFilling() ) {
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_FILLEMPTYBARREL, 0, 0); // Делаем запись в журнале активности
    StartFilling(); // Открываем клапан - начинаем наполнять бочку
    return;
  }

  // Проверяем на таймер и начинаем наполнять, если нужно. Выходим
  if( !IsFull() and !IsFilling() )
    if(NowHour == Settings.StartFillingHour and NowMinute == Settings.StartFillingMinute ) {
        lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_STARTFILLBYTIMER, 0, 0); // Делаем запись в журнале активности        
        StartFilling(); // Открываем клапан - начинаем наполнять бочку
        return;
    }
  
  // Если бочка и пуста и полна одновременно, озадаченно выходим
  if( IsEmpty() and IsFull() ) {
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_STATE, S_EVT_BARREL_STATE_ERRFULLANDEMPTY, 0, 0); // Делаем запись в журнале активности
    return;
  }
	
}

//---------------------------------------------------------------------
// Начать наполнение бочки
void gh_Barrel::StartFilling() {
  bIsValveOpen = true;
  lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_VALVE, S_EVT_BARREL_VALVE_OPEN, 0, 0); // Делаем запись в журнале активности
  digitalWrite(Cfg.RelayPin, LOW);
  if(Cfg.ValveOpenLedPin != 0xFF) digitalWrite(Cfg.ValveOpenLedPin, HIGH);
  millisStartFilling = millis();
}

//---------------------------------------------------------------------
// Закончить наполнение бочки
void gh_Barrel::StopFilling() {
  bIsValveOpen = false;
  lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_VALVE, S_EVT_BARREL_VALVE_VCLOSE, 0, 0); // Делаем запись в журнале активности
  digitalWrite(Cfg.RelayPin, HIGH);
  if(Cfg.ValveOpenLedPin != 0xFF) digitalWrite(Cfg.ValveOpenLedPin, LOW);
  millisStartFilling = 0;
}

//---------------------------------------------------------------------
// Установить/снять ручной режим работы
void gh_Barrel::SetManualMode(bool bMode){
  bIsManualMode = bMode;

  if(Cfg.ModeLedPin != 0xFF) digitalWrite(Cfg.ModeLedPin, bMode ? HIGH : LOW);
 
  if( bIsManualMode )
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_SETMODE, S_EVT_BARREL_SETMODE_MANUAL, 0, 0); // Делаем запись в журнале активности
  else
    lg.RecordActivityInt(DEV_BARREL, EVT_BARREL_SETMODE, S_EVT_BARREL_SETMODE_AUTO, 0, 0); // Делаем запись в журнале активности
 
  // Остановить наполнение при смене режима работы
  StopFilling();
}


//---------------------------------------------------------------------
// Бочка пуста?
bool gh_Barrel::IsEmpty() {
  return bIsEmpty;
}

//---------------------------------------------------------------------
// Бочка наполнена?
bool gh_Barrel::IsFull() {
  return bIsFull;
}

//---------------------------------------------------------------------
// Бочка наполняется?
bool gh_Barrel::IsFilling() {
  return bIsValveOpen;
}

//---------------------------------------------------------------------
// Проверить режим работы бочки
bool gh_Barrel::IsManualMode(){
	return bIsManualMode;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// Класс WaterLine - клапан полива одной грядки 
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

bool gh_WaterLine::Begin(WaterLineHardwareConfig HwCfg) {

  LOG("gh_WaterLine Begin. Valve: " + (String)HwCfg.RelayPin);

  // Загружаем сохраненные настройки работы
  Settings.IntervalDays           = SEEDBAD1_INTERVAL_DAYS;                // интервал полива в днях
  Settings.StartWateringHour      = SEEDBAD1_WATERING_START_HOUR;         // время начала полива
  Settings.StartWateringMin       = SEEDBAD1_WATERING_START_MIN;         // время начала полива
  Settings.DurationMins   = SEEDBAD1_WATERING_DURATION_MINS;                // продолжительность полива (мин)
  
  Settings.StartDate              = rtc.now() - TimeSpan(0,rtc.now().hour(),rtc.now().minute(),rtc.now().second()); // первая дата полива - ставим в сегодня 0:00:00
  // Плановая дата следующего полива - это не настраивается в меню, но должно сохраняться в EEPROM
  Settings.NextWateringDateTime   = Settings.StartDate + TimeSpan(Settings.IntervalDays, 0, 0, 0);  

  LOG((String)Settings.StartDate.timestamp() );
  LOG((String)Settings.StartWateringHour);
  LOG((String)Settings.StartWateringMin);
  LOG((String)Settings.IntervalDays);
  LOG((String)Settings.NextWateringDateTime.timestamp());

  // Инициализируем пины
  Cfg.RelayPin = HwCfg.RelayPin;
  Cfg.ValveOpenLedPin = HwCfg.ValveOpenLedPin;
  Cfg.ModeLedPin = HwCfg.ModeLedPin;
  
  if( Cfg.RelayPin != 0xFF)           pinMode(Cfg.RelayPin,         OUTPUT);
  if( Cfg.ValveOpenLedPin != 0xFF)    pinMode(Cfg.ValveOpenLedPin,  OUTPUT);
  if( Cfg.ModeLedPin != 0xFF)         pinMode(Cfg.ModeLedPin,       OUTPUT);

  if(Cfg.RelayPin == 0xFF) { 
    // Минимальная конфигурация оборудования для управления бочкой - реле и геркон наполнения
    return false;
  }

  // Устанавливаем автоматический режим работы
  SetManualMode(false);
  
  // Закрываем клапан
  StopWatering();

  // Инициализация переменных
  millisStartWatering = 0;       // таймер ограничения максимального времени полива

  return true;  
}

void gh_WaterLine::Poll(DateTime Now) {
  float DT;
  char buf[100];
  
  // 1. САМЫМ ПЕРВЫМ ДЕЛОМ
  // Если время полива вышло, то срочно остановить процесс, поставить состояние, выйти
  if( IsWatering() and (int)((millis()-millisStartWatering)/1000/60) > Settings.DurationMins) {
    lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_STATE, S_EVT_WATERING_LINE_STATE_STOPBYTIMER, Cfg.RelayPin, 0); // Делаем запись в журнале активности
    StopWatering();      // СРОЧНО ЗАКРЫВАЕМ КЛАПАН - ПРЕКРАЩАЕМ ПОЛИВАТЬ!!!
    return;
  }

  // 2. ДАЛЕЕ, Если бочка работает в ручном режиме, то выходим без обработки
  if(IsManualMode()) return;

  // 3. И НАКОНЕЦ ОБРАБОТКА АВТОМАТИЧЕСКОГО РЕЖИМА
  // Проверяем на таймер и начинаем поливать, если нужно. Выходим
  if( !IsWatering() )
    if( Now >= (Settings.NextWateringDateTime + TimeSpan(0, 0, Settings.StartWateringHour, Settings.StartWateringMin)) ) {
        lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_STATE, S_EVT_WATERING_LINE_STATE_STARTBYTIMER, Cfg.RelayPin, 0); // Делаем запись в журнале активности        
        StartWatering(); // Открываем клапан - начинаем поливать
        return;
    }
  
}

// Сбросить расписание полива - установить на текущую дату
void gh_WaterLine::ResetWateringDate() {
  
  Settings.StartDate              = rtc.now() - TimeSpan(0,rtc.now().hour(),rtc.now().minute(),rtc.now().second()); // первая дата полива - ставим в сегодня 0:00:00
  
  // Плановая дата следующего полива
  //Settings.NextWateringDateTime   = Settings.StartDate + TimeSpan(Settings.IntervalDays, 0, 0, 0);  
  Settings.NextWateringDateTime   = Settings.StartDate;
  
}
  
void gh_WaterLine::StartWatering() {
  bIsValveOpen = true;
  lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_VALVE, S_EVT_WATERING_LINE_VALVE_OPEN, Cfg.RelayPin, 0); // Делаем запись в журнале активности
  digitalWrite(Cfg.RelayPin, LOW);
  if(Cfg.ValveOpenLedPin != 0xFF) digitalWrite(Cfg.ValveOpenLedPin, HIGH);
  millisStartWatering = millis();

}

void gh_WaterLine::StopWatering() {
  bIsValveOpen = false;
  lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_VALVE, S_EVT_WATERING_LINE_VALVE_VCLOSE, Cfg.RelayPin, 0); // Делаем запись в журнале активности
  digitalWrite(Cfg.RelayPin, HIGH);
  if(Cfg.ValveOpenLedPin != 0xFF) digitalWrite(Cfg.ValveOpenLedPin, LOW);
  millisStartWatering = 0;

  Settings.NextWateringDateTime = Settings.NextWateringDateTime + TimeSpan(Settings.IntervalDays, 0, 0, 0);

}

void gh_WaterLine::SetManualMode(bool bMode) {
  bIsManualMode = bMode;

  if(Cfg.ModeLedPin != 0xFF) digitalWrite(Cfg.ModeLedPin, bMode ? HIGH : LOW);
 
  if( bIsManualMode )
    lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_SETMODE, S_EVT_WATERING_LINE_SETMODE_MANUAL, Cfg.RelayPin, 0); // Делаем запись в журнале активности
  else
    lg.RecordActivityInt(DEV_WATERING_LINE, EVT_WATERING_LINE_SETMODE, S_EVT_WATERING_LINE_SETMODE_AUTO, Cfg.RelayPin, 0); // Делаем запись в журнале активности
 
  // Остановить полив при смене режима работы
  StopWatering();
  
}
  
bool gh_WaterLine::IsWatering() {
  return bIsValveOpen;
}
bool gh_WaterLine::IsManualMode() {
  return bIsManualMode;
}

  
  unsigned long millisStartWatering;    // Количество миллисекунд, которые уже длится поливание
