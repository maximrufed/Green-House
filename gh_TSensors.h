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
    bool UpdateSensorsOnBus();	// ������� True ���� �������� �������� ����������
	float GetTEarth();
	float GetTAir();

	byte TSensorsUpdateIntervalMinutes;    // �������� ����� ������������ �������� ����������� �� ����

  private:
	long TSensorsPreviousMillis;      // ������ ����� ���������� ���������� �������� ��������
	DeviceAddress TSensorEarthAdr = TS_EARTH_ADR;	// ����� ������� �-�� �����
	DeviceAddress TSensorAirAdr = TS_AIR_ADR;	// ����� ������� �-�� �������
	OneWire oneWireTSensors;
	DallasTemperature DallasTemp;

};

#endif