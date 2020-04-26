#ifndef GH_EARTHFAN_H
#define GH_EARTHFAN_H

#include <Arduino.h>
#include "globals.h"

struct TerraAccumulatorSettings
{
	byte TAirStartCooling;			// Температура воздуха, при достижении которой включается вентилятор для охлаждения теплицы
	byte TAirStopCooling;			// Температура воздуха, при достижении которой охлаждение прекращается
	byte TAirStartHeating;			// Температура воздуха, при достижении которой включается вентилятор для нагревания теплицы
	byte TAirStopHeating;			// Температура воздуха, при достижении которой нагрев прекращается
	byte DTFanOn;					// Разница температур, при которой вентилятор может включаться
	byte DTFanOff;					// Разница температур, при которой вентилятор должен выключаться
};

class Earth_Fan
{
  public:
	Earth_Fan(byte pinFan);
	Earth_Fan(byte pinFan, byte pinFanLed);
	Earth_Fan(byte pinFan, byte pinFanLed, byte pinFanModeLed);
	void Begin();
	bool IsFanOn();							// Возврат True если вентилятор включен
	void On();								// Включить вентилятор
	void Off();								// Выключить вентилятор
	void TerraAccumulatorPoll(float TEarth, float TAir, bool IsNight);			// Обработка логики работы земляного аккумулятора тепла
	void SetManualMode(bool bMode);			// Установить/снять ручной режим работы вентилятора
	bool IsManualMode();					// Проверить режим работы вентилятора
	TerraAccumulatorSettings TASettings;	// Настройки земляного аккумулятора тепла

  private:
	byte RelayPin;
	byte FanLedPin;
	byte ModeLedPin;
	bool bIsOn;
	bool bIsManualMode;

};

#endif