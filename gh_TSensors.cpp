#include "gh_TSensors.h"

//---------------------------------------------------------------------
// Запрос обновления значений датчиков на шине
bool T_Sensors::UpdateSensorsOnBus()
{
  unsigned long currentMillis = millis();
  
  // Не пытаемся считывать значения датчиков каждый тик, а ждем истечения заданного Интервала
  if(currentMillis - TSensorsPreviousMillis > (long)TSensorsUpdateIntervalMinutes*1000*60) { // не забываем перевести Интервал из минут в миллисекунды
    // сохраняем время последнего обновления
    TSensorsPreviousMillis = currentMillis; 
 
    //DallasTemp.setWaitForConversion(false);
    DallasTemp.requestTemperatures();
    //DallasTemp.setWaitForConversion(true);
    LOG("Обновили значения датчиков!");
    LOG("------------");

    return true; // Значения датчиков обновились
  }
  return false; // Значения датчиков не изменялись
}


//---------------------------------------------------------------------
// Возвращаем температуру земли
float T_Sensors::GetTEarth()
{
	return (float)DallasTemp.getTempC(TSensorEarthAdr);
}

// Возвращаем температуру воздуха
float T_Sensors::GetTAir()
{
	return (float)DallasTemp.getTempC(TSensorAirAdr);
}



//---------------------------------------------------------------------
// Constructor
T_Sensors::T_Sensors(byte pin) 
	: oneWireTSensors(pin)
	, DallasTemp(&oneWireTSensors)
{
	TSensorsPreviousMillis = -60000;      // храним время последнего обновления значений датчиков
	TSensorsUpdateIntervalMinutes = 1;
}


//---------------------------------------------------------------------
// Begin
void T_Sensors::Begin()
{
	DallasTemp.begin();
}
