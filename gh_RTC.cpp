#include "gh_RTC.h"


bool gh_RTC :: begin(){
	return(true);
}

bool gh_RTC :: IsNight(){

	unsigned long currentMillis = millis();
  
	// �� �������� ��������� �������� �������� ������ ���, � ������ ��� �� ���� 1 ���� � 5 �����
	if(currentMillis - IsNightPreviousMillis > (long)5*1000*60) { // �� �������� ��������� �������� �� ����� � ������������
    // ��������� ����� ���������� ����������
    TSensorsPreviousMillis = currentMillis; 
 

	DateTime nowTime = now();
	//int nowHour = nowTime.hour();
	//LOG(nowHour);
	int Hour = now().hour();
	if( Hour > 20 || Hour < 7 ) {
		LOG("Night");
		return(true);
	} else {
		LOG("Day");
		return(true);
	}
}

bool gh_RTC :: IsDay(){
	return(!IsNight());
}