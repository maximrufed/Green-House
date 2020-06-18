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
#include "gh_logger.h"
extern logger lg;

class T_Sensors
{
  public:
	T_Sensors(byte pin);
	void Begin(byte UpdateIntervalMinutes);
    bool UpdateSensorsOnBus();						// Возврат True если значения датчиков обновились
	bool TSensorsPoll();							// Возврат True если значения датчиков обновились
	float GetTEarth();
	float GetTAir();
  float GetTBoard();
  float GetTOut();
  float GetT_TAFanIn();
  float GetT_TAFanOut();
  float GetTEarth2();

  private:
	long TSensorsPreviousMillis;					// храним время последнего обновления значений датчиков
	byte TSensorsUpdateIntervalMinutes;				// интервал между обновлениями датчиков температуры на шине

	DeviceAddress TSensorEarthAdr = TS_EARTH_ADR;	// адрес датчика Т-ры Земли
  DeviceAddress TSensorAirAdr = TS_AIR_ADR;   // адрес датчика Т-ры Воздуха
  DeviceAddress TSensorBoardAdr = TS_BOARD_ADR;   // адрес датчика Т-ры Воздуха
  DeviceAddress TSensorOutAdr = TS_OUT_ADR;   // адрес датчика Т-ры Воздуха
  DeviceAddress TSensorEarth2Adr = TS_EARTH_ADR2;   // адрес датчика Т-ры Воздуха
  DeviceAddress TSensorTAFanInAdr = TS_TA_FAN_IN_ADR;   // адрес датчика Т-ры Воздуха
  DeviceAddress TSensorTAFanOutAdr = TS_TA_FAN_OUT_ADR ;   // адрес датчика Т-ры Воздуха
	float TEarth = 0;								// Последняя, успешно считанная с датчика Т-ры земли, температура
	float TAir = 0;									// Последняя, успешно считанная с датчика Т-ры воздуха, температура
  float TBoard = 0;
  float TOut = 0;
  float TEarth2 = 0;
  float T_TAFanIn = 0;
  float T_TAFanOut = 0;
  
	OneWire oneWireTSensors;
	DallasTemperature DallasTemp;

};

#endif
