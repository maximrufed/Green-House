#include "gh_TSensors.h"

//---------------------------------------------------------------------
// Запрос обновления значений датчиков на шине

bool T_Sensors::TSensorsPoll()
{
	float T;
	unsigned long currentMillis = millis();
  
	// Не пытаемся считывать значения датчиков каждый тик, а ждем истечения заданного Интервала
	if(currentMillis - TSensorsPreviousMillis > (long)TSensorsUpdateIntervalMinutes*60000) { // не забываем перевести Интервал из минут в миллисекунды
		// сохраняем время последнего обновления
		TSensorsPreviousMillis = currentMillis; 
	 
		// Считываем обновление температуры с датчиков (ну да, это температура с прошлого цикла - несколько минут назад..)
		T = (float)DallasTemp.getTempC(TSensorEarthAdr);
		if( T == (float)-127 ) {
			LOG("ОШИБКА СЧИТЫВАНИЯ ЗНАЧЕНИЯ ДАТЧИКА ТЕМПЕРАТУРЫ ЗЕМЛИ!");
		} else {
			TEarth = T;
		}
		T = (float)DallasTemp.getTempC(TSensorAirAdr);
		if( T == (float)-127 ) {
			LOG("ОШИБКА СЧИТЫВАНИЯ ЗНАЧЕНИЯ ДАТЧИКА ТЕМПЕРАТУРЫ ВОЗДУХА!");
		} else {
			TAir = T;
		}
		
		// Запрашиваем шину на обновление
		DallasTemp.requestTemperatures();

		LOG("Обновили значения датчиков!");

		return true; // Значения датчиков обновились
	}

	return false; // Значения датчиков не изменялись
}


//---------------------------------------------------------------------
// Возвращаем температуру земли
float T_Sensors::GetTEarth()
{
	return TEarth;
	//return (float)DallasTemp.getTempC(TSensorEarthAdr);
}

// Возвращаем температуру воздуха
float T_Sensors::GetTAir()
{
	return TAir;
	//return (float)DallasTemp.getTempC(TSensorAirAdr);
}



//---------------------------------------------------------------------
// Constructor
T_Sensors::T_Sensors(byte pin)
	: oneWireTSensors(pin)
	, DallasTemp(&oneWireTSensors)
{
}


//---------------------------------------------------------------------
// Begin
void T_Sensors::Begin(byte UpdateIntervalMinutes)
{
	TSensorsPreviousMillis = -60000;							// храним время последнего обновления значений датчиков
	TSensorsUpdateIntervalMinutes = UpdateIntervalMinutes;		// интервал между обновлениями датчиков температуры на шине
	DallasTemp.setWaitForConversion(false);						// Устанавливаем асинхронный режим запросов к шине
	DallasTemp.begin();

	// Запрашиваем шину на обновление и ждем окончания процесса
	DallasTemp.requestTemperatures();
	delay(1000);

	// Считываем температуру с датчиков
	TEarth = (float)DallasTemp.getTempC(TSensorEarthAdr);
	TAir = (float)DallasTemp.getTempC(TSensorAirAdr);
	

}
