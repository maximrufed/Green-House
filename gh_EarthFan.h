#ifndef GH_EARTHFAN_H
#define GH_EARTHFAN_H

#include <Arduino.h>
#include "globals.h"

struct TerraAccumulatorSettings
{
	byte TAirStartCooling;			// ����������� �������, ��� ���������� ������� ���������� ���������� ��� ���������� �������
	byte TAirStopCooling;			// ����������� �������, ��� ���������� ������� ���������� ������������
	byte TAirStartHeating;			// ����������� �������, ��� ���������� ������� ���������� ���������� ��� ���������� �������
	byte TAirStopHeating;			// ����������� �������, ��� ���������� ������� ������ ������������
	byte DTFanOn;					// ������� ����������, ��� ������� ���������� ����� ����������
	byte DTFanOff;					// ������� ����������, ��� ������� ���������� ������ �����������
};

class Earth_Fan
{
  public:
	Earth_Fan(byte pinFan);
	Earth_Fan(byte pinFan, byte pinFanLed);
	Earth_Fan(byte pinFan, byte pinFanLed, byte pinFanModeLed);
	void Begin();
	bool IsFanOn();							// ������� True ���� ���������� �������
	void On();								// �������� ����������
	void Off();								// ��������� ����������
	void TerraAccumulatorPoll(float TEarth, float TAir, bool IsNight);			// ��������� ������ ������ ��������� ������������ �����
	void SetManualMode(bool bMode);			// ����������/����� ������ ����� ������ �����������
	bool IsManualMode();					// ��������� ����� ������ �����������
	TerraAccumulatorSettings TASettings;	// ��������� ��������� ������������ �����

  private:
	byte RelayPin;
	byte FanLedPin;
	byte ModeLedPin;
	bool bIsOn;
	bool bIsManualMode;

};

#endif