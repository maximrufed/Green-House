#include "gh_windows.h"


//---------------------------------------------------------------------
// Begin
bool GHWindow::Begin(GHWindowHardwareConfig HWConfig) {
	WinCfg.PinRelay1			= HWConfig.PinRelay1;
	WinCfg.PinRelay2			= HWConfig.PinRelay2;
	WinCfg.PinWindowMotorLed	= HWConfig.PinWindowMotorLed;
	WinCfg.PinWindowModeLed		= HWConfig.PinWindowModeLed;
	WinCfg.PinWindowAlarmLed	= HWConfig.PinWindowAlarmLed;
	WinCfg.PinLimitSwitchOpen	= HWConfig.PinLimitSwitchOpen;
	WinCfg.PinLimitSwitchClosed	= HWConfig.PinLimitSwitchClosed;

	if( WinCfg.PinRelay1            != 0 ) pinMode(WinCfg.PinRelay1,            OUTPUT);
	if( WinCfg.PinRelay2            != 0 ) pinMode(WinCfg.PinRelay2,            OUTPUT);
	if( WinCfg.PinWindowMotorLed    != 0 ) pinMode(WinCfg.PinWindowMotorLed,    OUTPUT);
	if( WinCfg.PinWindowModeLed     != 0 ) pinMode(WinCfg.PinWindowModeLed,     OUTPUT);
	if( WinCfg.PinWindowAlarmLed    != 0 ) pinMode(WinCfg.PinWindowAlarmLed,    OUTPUT);
	if( WinCfg.PinLimitSwitchOpen   != 0 ) pinMode(WinCfg.PinLimitSwitchOpen,   INPUT_PULLUP);
	if( WinCfg.PinLimitSwitchClosed != 0 ) pinMode(WinCfg.PinLimitSwitchClosed, INPUT_PULLUP);

	// ИЦнициализация переменных
	WindowStatus = CLOSED;
	bIsMotorOn = false;
	bIsAlarm = false;
	bIsManualMode = false;
	millisInOperation = 0;

	WinSettings.MotorMaxWorkMillis = WINDOW_MOTOR_MAX_WORK_MILLIS;
  WinSettings.TAirOpen = 30;      // Температура воздуха, при достижении которой 
  WinSettings.TAirClose = 18;     // Температура воздуха, при достижении которой 

	if(WinCfg.PinRelay1 == 0 || WinCfg.PinRelay2 == 0) { 
		// Минимальная конфигурация оборудования для управления окном - два реле мотора
		SetAlarm(true);
		return false;
	}

	// На всякий пожарный выключаем мотор
	StopMotor();

	return true;
}


//---------------------------------------------------------------------
// Открыть окно
void GHWindow::Open() {
    LOG("GHWindow::Open()");

	if( WindowStatus == OPEN || WindowStatus == OPENING ) return;
	
	if( WinCfg.PinLimitSwitchOpen != 0 && digitalRead(WinCfg.PinLimitSwitchOpen) == LOW ) { 
	// Если концевой выключатель открытого окна уже замкнут, то нельзя включать мотор
		WindowStatus = OPENING_FAILED;
		// Ставим сигнал тревоги и выходим
		SetAlarm (true);
		return;
	}

	// Сбрасываем сигнал тревоги
	SetAlarm (false);


	WindowStatus = OPENING;

	StartMotorToOpen();

	// Сюда можно вставить вывод на экран чего-нибудь типа "открываюсь..."
}

//---------------------------------------------------------------------
// Закрыть окно
void GHWindow::Close() {
	if( WindowStatus == CLOSED|| WindowStatus == CLOSING) return;
	
	if( WinCfg.PinLimitSwitchClosed != 0 && digitalRead(WinCfg.PinLimitSwitchClosed) == LOW ) { 
	// Если концевой выключатель закрытого окна уже замкнут, то нельзя включать мотор
		WindowStatus = CLOSING_FAILED;
		// Ставим сигнал тревоги и выходим
		SetAlarm (true);
		return;
	}

	// Сбрасываем сигнал тревоги
	SetAlarm (false);

	WindowStatus = CLOSING;
	StartMotorToClose();
	
	// Сюда можно вставить вывод на экран чего-нибудь типа "открываюсь..."
}

//---------------------------------------------------------------------
// Экстренная остановка мотора, включение индикатора тревоги
void GHWindow::HaltMotor() {
	LOG("GHWindow::HaltMotor");
	LOG("Экстренная остановка");
	StopMotor();
	switch ( WindowStatus ) {
		case OPENING:
			WindowStatus = OPENING_FAILED;
			// Включить режим тревоги
			SetAlarm(true);
			break;
		case CLOSING:
			WindowStatus = CLOSING_FAILED;
			// Включить режим тревоги
			SetAlarm(true);
			break;
	}

}

//---------------------------------------------------------------------
// Обработка логики работы умного окна
void GHWindow::WindowPoll(float TEarth, float TAir, bool IsNight) {
	// Если окно сейчас работает, нужно вовремя выключить мотор
	if( WindowStatus == OPENING || WindowStatus == CLOSING ) {
		CompleteOperationByTimerOrLS();
	}

	// Обработка автоматического режима
	if( !IsManualMode() ) {
    if( TAir > WinSettings.TAirOpen and WindowStatus == CLOSED and !IsNight) {
      LOG("Открываем окно, т.к. достигнута температура открытия");
      Open();
    }
    if( TAir < WinSettings.TAirOpen and WindowStatus == OPEN) {
      LOG("Закрываем окно, т.к. достигнута температура закрытия");
      Close();
    }
    
	}

}

//---------------------------------------------------------------------
// Установить/снять ручной режим работы окна
void GHWindow::SetManualMode(bool bMode) {
	bIsManualMode = bMode;

	if(WinCfg.PinWindowModeLed != 0) digitalWrite(WinCfg.PinWindowModeLed, bMode ? HIGH : LOW);
}

//---------------------------------------------------------------------
// Возврат True если мотор включен
bool GHWindow::IsMotorOn() {
	return bIsMotorOn; 
}

//---------------------------------------------------------------------
// Проверить режим работы окна
bool GHWindow::IsManualMode() {
	return bIsManualMode;
}

//---------------------------------------------------------------------
// Возврат True если окно находится в режиме тревоги
bool GHWindow::IsAlarmOn() {
	return bIsAlarm;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
// PRIVATE FUNCTIONS

//---------------------------------------------------------------------
// Завершить операцию закрытия/открытия по таймеру или концевому выключателю
void GHWindow::CompleteOperationByTimerOrLS() {

    //LOG("GHWindow::CompleteOperationByTimerOrLS()");

	if( WinCfg.PinLimitSwitchOpen != 0 && digitalRead(WinCfg.PinLimitSwitchOpen) == LOW ) { 
		// Сработал концевой выключатель открытого окна
		LOG("Сработал концевой выключатель открытого окна");
		switch( WindowStatus ) {
			case OPENING:
				// Все нормально, окно успешно открылось
				LOG("Все нормально, окно успешно открылось");
				WindowStatus = OPEN;
				StopMotor();
				return;
			case CLOSING:
				// Теоретически мы как раз стартуем закрытие из положения замкнутого выключателя открытия
				LOG("Окно CLOSING, но Теоретически мы как раз стартуем закрытие из положения замкнутого выключателя открытия");
				break;
			default:
				// Любой другой статус - это непорядок, халтимся
				LOG("Окно не OPENING и не CLOSING - это непорядок, халтимся");
				HaltMotor();
				return;
		}
	}

	// Сработал концевой выключатель закрытого окна
	if( WinCfg.PinLimitSwitchClosed != 0 && digitalRead(WinCfg.PinLimitSwitchClosed) == LOW ) { 
		LOG("Сработал концевой выключатель закрытого окна");
		switch( WindowStatus ) {
			case CLOSING:
				// Все нормально, окно успешно закрылось
				LOG("Все нормально, окно успешно закрылось");
				WindowStatus = CLOSED;
				StopMotor();
				return;
			case OPENING:
				// Теоретически мы как раз стартуем открытие из положения замкнутого выключателя закрытия
				LOG("Окно OPENING, но Теоретически мы как раз стартуем открытие из положения замкнутого выключателя закрытия");
				break;
			default:
				// Любой другой статус - это непорядок, халтимся
				LOG("Окно не OPENING и не CLOSING - это непорядок, халтимся");
				HaltMotor();
				return;
		}
	}

	if( millis() - millisInOperation > WinSettings.MotorMaxWorkMillis ) {
		// Превышено время работы мотора выше лимита. Пора заканчивать
		LOG("Превышено время работы мотора выше лимита. Пора заканчивать и ставить статус OPEN или CLOSED");
		if( WindowStatus == OPENING ) {
			WindowStatus = OPEN;
		} else { // Любой другой статус может быть только CLOSING
			WindowStatus = CLOSED;
		}

		StopMotor();
		LOG("GHWindow::CompleteOperationByTimerOrLS   ----- RETURN");
		return;

	}

}


//---------------------------------------------------------------------
// Включить мотор на открывание
void GHWindow::StartMotorToOpen() {
	LOG("GHWindow::StartMotorToOpen");
	LOG("Включаем мотор на открывание");
	digitalWrite(WinCfg.PinRelay1, LOW);
	digitalWrite(WinCfg.PinRelay2, HIGH);
	bIsMotorOn = true;
	millisInOperation = millis();
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, HIGH);
}

//---------------------------------------------------------------------
// Включить мотор на закрывание
void GHWindow::StartMotorToClose() {
	LOG("GHWindow::StartMotorToClose");
	LOG("Включаем мотор на закрывание");
	digitalWrite(WinCfg.PinRelay1, HIGH);
	digitalWrite(WinCfg.PinRelay2, LOW);
	bIsMotorOn = true;
	millisInOperation = millis();
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, HIGH);
}

//---------------------------------------------------------------------
// Выключить мотор
void GHWindow::StopMotor() {
	LOG("GHWindow::StopMotor");
	LOG("Выключаем мотор");
	digitalWrite(WinCfg.PinRelay1, HIGH);
	LOG("1");
	digitalWrite(WinCfg.PinRelay2, HIGH);
	LOG("2");
	millisInOperation = 0;
	LOG("3");
	bIsMotorOn = false;
	LOG("3");
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, LOW);
	LOG("4");
}

//---------------------------------------------------------------------
// Установить/сбросить режим тревоги
void GHWindow::SetAlarm(bool bAlarm) {
	
	bIsAlarm = bAlarm;

	// Включаем или выключаем индикатор в зависимости от значения bAlarm
	if(WinCfg.PinWindowAlarmLed != 0) digitalWrite(WinCfg.PinWindowAlarmLed, bAlarm ? HIGH : LOW);
}
