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
	TASettings.TAirStartCooling = 30;
	TASettings.TAirStopCooling = 25;
	TASettings.TAirStartHeating = 10;
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

	// Если вентилятор работает в ручном режиме, то выходим без обработки
	if(IsManualMode()) return;
	

	// Вычисляем разницу температур воздуха и земли
	DT = TEarth > TAir ? TEarth-TAir : TAir-TEarth;

	// Если разница температур меньше разумной, то выключить вентилятор
	if(DT < TASettings.DTFanOff) {
		Off();
		return;
	}
	
	// Если разница температур недостаточна для того, чтобы предпринимать какие-то действия, то просто уходим по-английски
	if(DT < TASettings.DTFanOn ) return;

	if (!IsNight){ //День
		// Солнце жарит. Пора начать охлаждение воздуха в теплице.
		if(TAir > TASettings.TAirStartCooling ) {
			On();
			return;
		}

		// Солнце скрылось. Пора остановить охлаждение воздуха в теплице
		if(TAir < TASettings.TAirStopCooling) {
			Off();
			return;
		}
	} else { // Ночь
		// Становится холодно. Пора начать нагрев воздуха в теплице
		if(TAir < TASettings.TAirStartHeating) {
			On();
			return;
		}

		// Утро наступило. Пора прекратить нагрев воздуха в теплице
		if(TAir > TASettings.TAirStopHeating) {
			Off();
			return;
		}
	}


}

//---------------------------------------------------------------------
// Выключить вентилятор
void Earth_Fan::Off() {
	bIsOn = false;
	digitalWrite(RelayPin, HIGH);
	if(FanLedPin != 0) digitalWrite(FanLedPin, LOW);
}

//---------------------------------------------------------------------
// Включить вентилятор
void Earth_Fan::On() {
	bIsOn = true;
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
	// Выключить вентилятор при смене режима работы
	Off();
}

//---------------------------------------------------------------------
// Проверить режим работы вентилятора
bool Earth_Fan::IsManualMode(){
	return bIsManualMode;
}
