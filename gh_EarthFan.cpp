#include "gh_EarthFan.h"


//---------------------------------------------------------------------
// Constructor
Earth_Fan::Earth_Fan(byte pinFan) {
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = 0;
	ModeLedPin = 0;

	// Выключаем вентилятор
	Off();
}

Earth_Fan::Earth_Fan(byte pinFan, byte pinFanLed) {
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = pinFanLed;
	pinMode(FanLedPin, OUTPUT);
	ModeLedPin = 0;

	// Выключаем вентилятор
	Off();
}

Earth_Fan::Earth_Fan(byte pinFan, byte pinFanLed, byte pinFanModeLed){
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = pinFanLed;
	pinMode(FanLedPin, OUTPUT);
	ModeLedPin = pinFanModeLed;
	pinMode(ModeLedPin, OUTPUT);

	// Выключаем вентилятор
	Off();
}



//---------------------------------------------------------------------
// Begin
void Earth_Fan::Begin() {

	// Загружаем сохраненные настройки работы вентилятора
	TASettings.TAirStartCooling = 25;
	TASettings.TAirStopCooling = 20;
	TASettings.TAirStartHeating = 12;
	TASettings.TAirStopHeating = 14;
	TASettings.DTFanOn = 3;
	TASettings.DTFanOff = 1;

	// Устанавливаем автоматический режим работы вентилятора
	SetManualMode(false);
	
	// Выключаем вентилятор
	Off();

}

//---------------------------------------------------------------------
// Обработка логики работы земляного аккумулятора тепла
void Earth_Fan::TerraAccumulatorPoll(float TEarth, float TAir, bool IsNight) {
	float DT;
  char buf[100];

	// Если вентилятор работает в ручном режиме, то выходим без обработки
	if(IsManualMode()) return;
	

	// Вычисляем разницу температур воздуха и земли
	DT = TEarth > TAir ? TEarth-TAir : TAir-TEarth;

  // Если разница температур недостаточна для того, чтобы предпринимать какие-то действия, то просто уходим по-английски
  if(DT < TASettings.DTFanOn ) return;

	if(bIsOn and DT < TASettings.DTFanOff) {
		// Разница температур меньше разумной. Вентилятор ВЫКЛ
    sprintf(buf, "Разница темп-р мала. Вентилятор ВЫКЛ. TEarth: %d; TAir: %d", (int)TEarth , (int)TAir);
    lg.RecordActivity(EVT_FAN_OFF, ObjectName,  buf); // Делаем запись в журнале активности
		Off();
		return;
	}
	
	if (!IsNight){ //День
    if(TAir > TASettings.TAirStartCooling) {
  		// Солнце жарит. Пора начать охлаждение воздуха в теплице.
      sprintf(buf, "Солнце жарит. Вентилятор ВКЛ. TEarth: %d; TAir: %d", (int)TEarth , (int)TAir);
      lg.RecordActivity(EVT_FAN_ON, ObjectName,  buf); // Делаем запись в журнале активности
  		On();
  		return;
  	}
  
  	// Солнце скрылось. Пора остановить охлаждение воздуха в теплице
  	if(TAir < TASettings.TAirStopCooling) {
      sprintf(buf, "Солнце скрылось. Вентилятор ВЫКЛ. TEarth: %d; TAir: %d", (int)TEarth , (int)TAir);
      lg.RecordActivity(EVT_FAN_OFF, ObjectName,  buf); // Делаем запись в журнале активности
  		Off();
  		return;
  	}
	} else { // Ночь
		// Становится холодно. Пора начать нагрев воздуха в теплице
		if(!bIsOn and TAir < TASettings.TAirStartHeating) {
      sprintf(buf, "Ночь, холодно, греемся. Вентилятор ВКЛ. TEarth: %d; TAir: %d", (int)TEarth , (int)TAir);
      lg.RecordActivity(EVT_FAN_ON, ObjectName,  buf); // Делаем запись в журнале активности
			On();
			return;
		}

		// Утро наступило. Пора прекратить нагрев воздуха в теплице
		if(bIsOn and TAir > TASettings.TAirStopHeating) {
      sprintf(buf, "Ночь, не холодно. Вентилятор ВЫКЛ. TEarth: %d; TAir: %d", (int)TEarth , (int)TAir);
      lg.RecordActivity(EVT_FAN_OFF, ObjectName,  buf); // Делаем запись в журнале активности
			Off();
			return;
		}
	}


}

//---------------------------------------------------------------------
// Выключить вентилятор
void Earth_Fan::Off() {
	bIsOn = false;
  lg.RecordActivity(EVT_FAN_OFF, ObjectName,  "Вентилятор ВЫКЛ."); // Делаем запись в журнале активности
	digitalWrite(RelayPin, HIGH);
	if(FanLedPin != 0) digitalWrite(FanLedPin, LOW);
}

//---------------------------------------------------------------------
// Включить вентилятор
void Earth_Fan::On() {
	bIsOn = true;
  lg.RecordActivity(EVT_FAN_ON, ObjectName,  "Вентилятор ВКЛ."); // Делаем запись в журнале активности
	digitalWrite(RelayPin, LOW);
	if(FanLedPin != 0) digitalWrite(FanLedPin, HIGH);
}

//---------------------------------------------------------------------
// Проверка состояния вентилятора
bool Earth_Fan::IsFanOn(){
	return bIsOn;
}

//---------------------------------------------------------------------
// Установить/снять ручной режим работы вентилятора
void Earth_Fan::SetManualMode(bool bMode){
	bIsManualMode = bMode;

	if(ModeLedPin != 0) digitalWrite(ModeLedPin, bMode ? HIGH : LOW);
 
  if( bIsManualMode )
    lg.RecordActivity(EVT_FAN_SET_MANUAL, ObjectName,  "Переходим в ручной режим"); // Делаем запись в журнале активности
  else
    lg.RecordActivity(EVT_FAN_SET_AUTO, ObjectName,  "Переходим в автоматический режим"); // Делаем запись в журнале активности
 
	// Выключить вентилятор при смене режима работы
	Off();
}

//---------------------------------------------------------------------
// Проверить режим работы вентилятора
bool Earth_Fan::IsManualMode(){
	return bIsManualMode;
}
