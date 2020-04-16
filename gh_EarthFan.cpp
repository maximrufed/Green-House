#include "gh_EarthFan.h"


//---------------------------------------------------------------------
// Constructor
Earth_Fan::Earth_Fan(byte pinFan) {
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = 0;
	ModeLedPin = 0;

	// ��������� ����������
	Off();
}

Earth_Fan::Earth_Fan(byte pinFan, byte pinFanLed) {
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = pinFanLed;
	pinMode(FanLedPin, OUTPUT);
	ModeLedPin = 0;

	// ��������� ����������
	Off();
}

Earth_Fan::Earth_Fan(byte pinFan, byte pinFanLed, byte pinFanModeLed){
	RelayPin = pinFan;
	pinMode(RelayPin, OUTPUT);
	FanLedPin = pinFanLed;
	pinMode(FanLedPin, OUTPUT);
	ModeLedPin = pinFanModeLed;
	pinMode(ModeLedPin, OUTPUT);

	// ��������� ����������
	Off();
}



//---------------------------------------------------------------------
// Begin
void Earth_Fan::Begin() {

	// ��������� ����������� ��������� ������ �����������
	TASettings.TAirStartCooling = 30;
	TASettings.TAirStopCooling = 25;
	TASettings.TAirStartHeating = 10;
	TASettings.TAirStopHeating = 14;
	TASettings.DTFanOn = 3;
	TASettings.DTFanOff = 1;

	// ������������� �������������� ����� ������ �����������
	SetManualMode(false);
	
	// ��������� ����������
	Off();

}

//---------------------------------------------------------------------
// ��������� ������ ������ ��������� ������������ �����
void Earth_Fan::TerraAccumulatorPoll(float TEarth, float TAir, bool IsNight) {
	float DT;

	// ���� ���������� �������� � ������ ������, �� ������� ��� ���������
	if(IsManualMode()) return;
	

	// ��������� ������� ���������� ������� � �����
	DT = TEarth > TAir ? TEarth-TAir : TAir-TEarth;

	// ���� ������� ���������� ������ ��������, �� ��������� ����������
	if(DT < TASettings.DTFanOff) {
		Off();
		return;
	}
	
	// ���� ������� ���������� ������������ ��� ����, ����� ������������� �����-�� ��������, �� ������ ������ ��-���������
	if(DT < TASettings.DTFanOn ) return;

	if (!IsNight){ //����
		// ������ �����. ���� ������ ���������� ������� � �������.
		if(TAir > TASettings.TAirStartCooling ) {
			On();
			return;
		}

		// ������ ��������. ���� ���������� ���������� ������� � �������
		if(TAir < TASettings.TAirStopCooling) {
			Off();
			return;
		}
	} else { // ����
		// ���������� �������. ���� ������ ������ ������� � �������
		if(TAir < TASettings.TAirStartHeating) {
			On();
			return;
		}

		// ���� ���������. ���� ���������� ������ ������� � �������
		if(TAir > TASettings.TAirStopHeating) {
			Off();
			return;
		}
	}


}

//---------------------------------------------------------------------
// ��������� ����������
void Earth_Fan::Off() {
	bIsOn = false;
	digitalWrite(RelayPin, HIGH);
	if(FanLedPin != 0) digitalWrite(FanLedPin, LOW);
}

//---------------------------------------------------------------------
// �������� ����������
void Earth_Fan::On() {
	bIsOn = true;
	digitalWrite(RelayPin, LOW);
	if(FanLedPin != 0) digitalWrite(FanLedPin, HIGH);
}

//---------------------------------------------------------------------
// �������� ��������� �����������
bool Earth_Fan::IsFanOn(){
	return bIsOn;
}

//---------------------------------------------------------------------
// ����������/����� ������ ����� ������ �����������
void Earth_Fan::SetManualMode(bool bMode){
	bIsManualMode = bMode;

	if(ModeLedPin != 0) digitalWrite(ModeLedPin, bMode ? HIGH : LOW);
	// ��������� ���������� ��� ����� ������ ������
	Off();
}

//---------------------------------------------------------------------
// ��������� ����� ������ �����������
bool Earth_Fan::IsManualMode(){
	return bIsManualMode;
}
