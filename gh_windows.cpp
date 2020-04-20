#include "gh_windows.h"


//---------------------------------------------------------------------
// Begin
bool GHWindow::Begin(GHWindowHardwareConfig HWConfig) {
	WinCfg.PinRelay1			= HWConfig.PinRelay1;
	WinCfg.PinRelay2			= HWConfig.PinRelay2;
	WinCfg.PinWindowMotorLed	= HWConfig.PinWindowMotorLed;
	WinCfg.PinWindowModeLed		= HWConfig.PinWindowModeLed;
	WinCfg.PinWindowAlarmLed	= HWConfig.PinWindowAlarmLed;
	WinCfg.PinLimitSwitchOpen	= HWConfig.PinLimitSwitchOpen;
	WinCfg.PinLimitSwitchClosed	= HWConfig.PinLimitSwitchClosed;

	if( WinCfg.PinRelay1            != 0 ) pinMode(WinCfg.PinRelay1,            OUTPUT);
	if( WinCfg.PinRelay2            != 0 ) pinMode(WinCfg.PinRelay2,            OUTPUT);
	if( WinCfg.PinWindowMotorLed    != 0 ) pinMode(WinCfg.PinWindowMotorLed,    OUTPUT);
	if( WinCfg.PinWindowModeLed     != 0 ) pinMode(WinCfg.PinWindowModeLed,     OUTPUT);
	if( WinCfg.PinWindowAlarmLed    != 0 ) pinMode(WinCfg.PinWindowAlarmLed,    OUTPUT);
	if( WinCfg.PinLimitSwitchOpen   != 0 ) pinMode(WinCfg.PinLimitSwitchOpen,   INPUT_PULLUP);
	if( WinCfg.PinLimitSwitchClosed != 0 ) pinMode(WinCfg.PinLimitSwitchClosed, INPUT_PULLUP);

	// ������������� ����������
	WindowStatus = CLOSED;
	bIsMotorOn = false;
	bIsAlarm = false;
	bIsManualMode = false;
	millisInOperation = 0;

	WinSettings.MotorMaxWorkMillis = WINDOW_MOTOR_MAX_WORK_MILLIS;

	if(WinCfg.PinRelay1 == 0 || WinCfg.PinRelay2 == 0) { 
		// ����������� ������������ ������������ ��� ���������� ����� - ��� ���� ������
		SetAlarm(true);
		return false;
	}

	// �� ������ �������� ��������� �����
	StopMotor();

	return true;
}


//---------------------------------------------------------------------
// ������� ����
void GHWindow::Open() {
    LOG("GHWindow::Open()");
	LOG(digitalRead(WinCfg.PinLimitSwitchOpen));
	if( WindowStatus == OPEN || WindowStatus == OPENING ) return;
	
	LOG("Log 1");
	if( WinCfg.PinLimitSwitchOpen != 0 && digitalRead(WinCfg.PinLimitSwitchOpen) == LOW ) { 
	// ���� �������� ����������� ��������� ���� ��� �������, �� ������ �������� �����
		WindowStatus = OPENING_FAILED;
		// ������ ������ ������� � �������
		SetAlarm (true);
		LOG("2");
		return;
	}

	LOG("3");
	// ���������� ������ �������
	SetAlarm (false);

	LOG("4");

	WindowStatus = OPENING;

	StartMotorToOpen();
	LOG("5");

	// ���� ����� �������� ����� �� ����� ����-������ ���� "����������..."
}

//---------------------------------------------------------------------
// ������� ����
void GHWindow::Close() {
	if( WindowStatus == CLOSED|| WindowStatus == CLOSING) return;
	
	if( WinCfg.PinLimitSwitchClosed != 0 && digitalRead(WinCfg.PinLimitSwitchClosed) == LOW ) { 
	// ���� �������� ����������� ��������� ���� ��� �������, �� ������ �������� �����
		WindowStatus = CLOSING_FAILED;
		// ������ ������ ������� � �������
		SetAlarm (true);
		return;
	}

	// ���������� ������ �������
	SetAlarm (false);

	WindowStatus = CLOSING;
	StartMotorToClose();
	
	// ���� ����� �������� ����� �� ����� ����-������ ���� "����������..."
}

//---------------------------------------------------------------------
// ���������� ��������� ������, ��������� ���������� �������
void GHWindow::HaltMotor() {
	StopMotor();
	switch ( WindowStatus ) {
		case OPENING:
			WindowStatus = OPENING_FAILED;
			// �������� ����� �������
			SetAlarm(true);
			break;
		case CLOSING:
			WindowStatus = CLOSING_FAILED;
			// �������� ����� �������
			SetAlarm(true);
			break;
	}

}

//---------------------------------------------------------------------
// ��������� ������ ������ ������ ����
void GHWindow::WindowPoll(float TEarth, float TAir, bool IsNight) {
	// ���� ���� ������ ��������, ����� ������� ��������� �����
	if( WindowStatus == OPENING || WindowStatus == CLOSING ) {
		CompleteOperationByTimerOrLS();
	}

	// ��������� ��������������� ������
	if( !IsManualMode() ) {
	}

}

//---------------------------------------------------------------------
// ����������/����� ������ ����� ������ ����
void GHWindow::SetManualMode(bool bMode) {
	bIsManualMode = bMode;

	if(WinCfg.PinWindowModeLed != 0) digitalWrite(WinCfg.PinWindowModeLed, bMode ? HIGH : LOW);
}

//---------------------------------------------------------------------
// ������� True ���� ����� �������
bool GHWindow::IsMotorOn() {
	return bIsMotorOn; 
}

//---------------------------------------------------------------------
// ��������� ����� ������ ����
bool GHWindow::IsManualMode() {
	return bIsManualMode;
}

//---------------------------------------------------------------------
// ������� True ���� ���� ��������� � ������ �������
bool GHWindow::IsAlarmOn() {
	return bIsAlarm;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************
// PRIVATE FUNCTIONS

//---------------------------------------------------------------------
// ��������� �������� ��������/�������� �� ������� ��� ��������� �����������
void GHWindow::CompleteOperationByTimerOrLS() {

    LOG("GHWindow::CompleteOperationByTimerOrLS()");
	LOG(digitalRead(WinCfg.PinLimitSwitchOpen));
	// �������� �������� ����������� ��������� ����
	if( WinCfg.PinLimitSwitchOpen != 0 && digitalRead(WinCfg.PinLimitSwitchOpen) == LOW ) { 
		switch( WindowStatus ) {
			case OPENING:
				// ��� ���������, ���� ������� ���������
				WindowStatus = OPEN;
				StopMotor();
				return;
			case CLOSING:
				// ������������ �� ��� ��� �������� �������� �� ��������� ���������� ����������� ��������
				break;
			default:
				// ����� ������ ������ - ��� ���������, ��������
				HaltMotor();
				return;
		}
	}

	LOG("3");

	// �������� �������� ����������� ��������� ����
	if( WinCfg.PinLimitSwitchClosed != 0 && digitalRead(WinCfg.PinLimitSwitchClosed) == LOW ) { 
		switch( WindowStatus ) {
			case CLOSING:
				// ��� ���������, ���� ������� ���������
				WindowStatus = CLOSED;
				StopMotor();
				return;
			case OPENING:
				// ������������ �� ��� ��� �������� �������� �� ��������� ���������� ����������� ��������
				break;
			default:
				// ����� ������ ������ - ��� ���������, ��������
				HaltMotor();
				return;
		}
	}

	// ��������� ����� ������ ������ ���� ������. ���� �����������
	if( millis() - millisInOperation > WinSettings.MotorMaxWorkMillis ) {
		if( WindowStatus == OPENING ) {
			WindowStatus = OPEN;
		} else { // ����� ������ ������ ����� ���� ������ CLOSING
			WindowStatus = CLOSED;
		}

		StopMotor();
		return;

	}

}


//---------------------------------------------------------------------
// �������� ����� �� ����������
void GHWindow::StartMotorToOpen() {
	digitalWrite(WinCfg.PinRelay1, LOW);
	digitalWrite(WinCfg.PinRelay2, HIGH);
	bIsMotorOn = true;
	millisInOperation = millis();
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, HIGH);
}

//---------------------------------------------------------------------
// �������� ����� �� ����������
void GHWindow::StartMotorToClose() {
	digitalWrite(WinCfg.PinRelay1, HIGH);
	digitalWrite(WinCfg.PinRelay2, LOW);
	bIsMotorOn = true;
	millisInOperation = millis();
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, HIGH);
}

//---------------------------------------------------------------------
// ��������� �����
void GHWindow::StopMotor() {
	digitalWrite(WinCfg.PinRelay1, HIGH);
	digitalWrite(WinCfg.PinRelay2, HIGH);
	millisInOperation = 0;
	bIsMotorOn = false;
	if(WinCfg.PinWindowMotorLed != 0) digitalWrite(WinCfg.PinWindowMotorLed, LOW);
}

//---------------------------------------------------------------------
// ����������/�������� ����� �������
void GHWindow::SetAlarm(bool bAlarm) {
	
	bIsAlarm = bAlarm;

	// �������� ��� ��������� ��������� � ����������� �� �������� bAlarm
	if(WinCfg.PinWindowAlarmLed != 0) digitalWrite(WinCfg.PinWindowAlarmLed, bAlarm ? HIGH : LOW);
}
