#ifndef GH_WINDOWS_H
#define GH_WINDOWS_H

#include <Arduino.h>
#include "globals.h"

// ��������� ��������� ����
#define OPENING			1		// ������ �����������
#define CLOSING			2		// ������ �����������
#define OPEN			3		// �������
#define	CLOSED			4		// �������
#define	OPENING_FAILED	5		// ���������� �� ����������� ���������, ������
#define CLOSING_FAILED	6		// ���������� �� ����������� ���������, ������


struct GHWindowSettings						// ��������� ��� �������� ���������� �������� ����
{
	int MotorMaxWorkMillis; 				// ����� ������ ������ � �������� ��� ������� ��������/�������� ����
};

struct GHWindowHardwareConfig				// ������������ ����� ������������ ����
{
	byte PinRelay1 = 0;						// ���� �1 ���������� ������� ����
	byte PinRelay2 = 0;						// ���� �2 ���������� ������� ����
	byte PinWindowMotorLed = 0;				// ��������� ������ ������
	byte PinWindowModeLed = 0;				// ��������� ��������� ������� ������ ���������� �����
	byte PinWindowAlarmLed = 0;				// ��������� ������� ��� ����������� ����
	byte PinLimitSwitchOpen = 0;			// �������� ����������� ��������� ����
	byte PinLimitSwitchClosed = 0;			// �������� ����������� ��������� ����
	GHWindowHardwareConfig () {}
};

class GHWindow
{
  public:
	bool Begin(GHWindowHardwareConfig HWConfig); // ������� false ���� �� ���������� ��� ���� ���������� �������
	void Open();							// ������� ����
	void Close();							// ������� ����
	void HaltMotor();						// ���������� ��������� ������, ��������� ������ �������
	void WindowPoll(float TEarth, float TAir, bool IsNight);			// ��������� ������ ������ ������ ����
	void SetManualMode(bool bMode);			// ����������/����� ������ ����� ������ ����
	bool IsMotorOn();						// ������� True ���� ����� �������
	bool IsManualMode();					// ��������� ����� ������ ����
	bool IsAlarmOn();						// ������� True ���� ���� ��������� � ������ �������
	GHWindowSettings WinSettings;			// ��������� ����

  private:
	GHWindowHardwareConfig WinCfg;			// ������������ �����
	byte WindowStatus;						// ��������� ����. ��������� ��������, ������������ ����� define ����
	bool bIsMotorOn;
	bool bIsAlarm;
	bool bIsManualMode;
	unsigned long millisInOperation;		// ���������� �����������, ������� ��� ������ �������� ��������/��������


	void StartMotorToOpen();				// �������� ����� �� ����������
	void StartMotorToClose();				// �������� ����� �� ����������
	void StopMotor();						// ��������� �����
	void SetAlarm(bool bAlarm);				// ����������/�������� ����� �������
	void CompleteOperationByTimerOrLS();	// ��������� �������� ��������/�������� �� ������� ��� ��������� �����������

};

#endif