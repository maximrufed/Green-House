#ifndef GH_SETTINGS_H
#define GH_SETTINGS_H

#include <Arduino.h>

struct TerraAccumulatorSettings
{
	byte TDiffStart;				// ������� ���������� ����� � �������, ��� ������� ����������� ����������
	byte TDiffStop;					// ������� ���������� ����� � �������, ��� ������� ���������� �����������
	byte TMaxAirAtNightStop;		// ������ �����������, ���� ������� ���������� ������������� ���������������
	byte TMinAirAtNightStart;		// ������ �����������, ���� ������� ���������� ������������� ����������
	byte TMaxAirAtDayStart;			// ������� �����������, ���� ������� ���������� ������������� ����������
	byte TMinAirAtDayStop;			// ������� �����������, ���� ������� ���������� ������������� ���������������
};

struct ControllerSettings
{
	//byte TSensorsUpdateIntervalMinutes;    // �������� ����� ������������ �������� ����������� �� ����
	TerraAccumulatorSettings TerraAccum;
};


extern ControllerSettings GHSettings;


void InitSettings();


#endif