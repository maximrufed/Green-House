// -----------------------------------------------------------------------
// Класс TSensors - Датчики температуры
//				Гирлянда датчиков на шине I2C 
// 
// Устройства:	датчик температуры воздуха
//				датчик температуры земли
//
// -----------------------------------------------------------------------

#ifndef GH_TSENSORS_H
#define GH_TSENSORS_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "globals.h"

class T_Sensors
{
  public:
	T_Sensors(byte pin);
	void Begin(byte UpdateIntervalMinutes);
    bool UpdateSensorsOnBus();						// Возврат True если значения датчиков обновились
	bool TSensorsPoll();							// Возврат True если значения датчиков обновились
	float GetTEarth();
	float GetTAir();

  private:
	long TSensorsPreviousMillis;					// храним время последнего обновления значений датчиков
	byte TSensorsUpdateIntervalMinutes;				// интервал между обновлениями датчиков температуры на шине

	DeviceAddress TSensorEarthAdr = TS_EARTH_ADR;	// адрес датчика Т-ры Земли
	DeviceAddress TSensorAirAdr = TS_AIR_ADR;		// адрес датчика Т-ры Воздуха
	float TEarth = 0;								// Последняя, успешно считанная с датчика Т-ры земли, температура
	float TAir = 0;									// Последняя, успешно считанная с датчика Т-ры воздуха, температура

	OneWire oneWireTSensors;
	DallasTemperature DallasTemp;

};

#endif