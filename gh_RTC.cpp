#include "gh_RTC.h"


bool gh_RTC :: Begin(){
	return(rtcDS3231.begin());
}

bool gh_RTC :: IsNight(){

	unsigned long currentMillis = millis();
  
	// �� �������� ��������� �������� �������� ������ ���, � ������ ��� �� ���� 1 ���� � 5 ����� (300 000 �����������)
	if(currentMillis - IsNightPreviousMillis > (long)300000) { 
		// ��������� ����� ���������� ����������
		IsNightPreviousMillis = currentMillis; 
	}


	//DateTime nowTime = now();
	//int nowHour = nowTime.hour();
	//LOG(nowHour);
	int Hour = rtcDS3231.now().hour();
	if( Hour > 20 || Hour < 7 ) {
		//LOG("Night");
		return(true);
	} else {
		//LOG("Day");
		return(true);
	}
}

bool gh_RTC :: IsDay(){
	return(!IsNight());
}