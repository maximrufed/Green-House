#ifndef GH_TSENSORS_H
#define GH_TSENSORS_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "globals.h"

class T_Sensors
{
  public:
	T_Sensors(byte pin);
	void Begin();
    bool UpdateSensorsOnBus();	// Возврат True если значения датчиков обновились
	float GetTEarth();
	float GetTAir();

	byte TSensorsUpdateIntervalMinutes;    // интервал между обновлениями датчиков температуры на шине

  private:
	long TSensorsPreviousMillis;      // храним время последнего обновления значений датчиков
	DeviceAddress TSensorEarthAdr = TS_EARTH_ADR;	// адрес датчика Т-ры Земли
	DeviceAddress TSensorAirAdr = TS_AIR_ADR;	// адрес датчика Т-ры Воздуха
	OneWire oneWireTSensors;
	DallasTemperature DallasTemp;

};

#endif