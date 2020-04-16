#ifndef GH_SETTINGS_H
#define GH_SETTINGS_H

#include <Arduino.h>

struct TerraAccumulatorSettings
{
	byte TDiffStart;				// Разница температур земли и воздуха, при которой запускается вентилятор
	byte TDiffStop;					// Разница температур земли и воздуха, при которой вентилятор выключается
	byte TMaxAirAtNightStop;		// Ночная температура, выше которой вентилятор принудительно останавливается
	byte TMinAirAtNightStart;		// Ночная температура, ниже которой вентилятор принудительно включается
	byte TMaxAirAtDayStart;			// Дневная температура, выше которой вентилятор принудительно включается
	byte TMinAirAtDayStop;			// Дневная температура, ниже которой вентилятор принудительно останавливается
};

struct ControllerSettings
{
	//byte TSensorsUpdateIntervalMinutes;    // интервал между обновлениями датчиков температуры на шине
	TerraAccumulatorSettings TerraAccum;
};


extern ControllerSettings GHSettings;


void InitSettings();


#endif