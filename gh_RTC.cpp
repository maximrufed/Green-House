#include "gh_RTC.h"


bool gh_RTC :: Begin(){
	return(rtcDS3231.begin());
}

bool gh_RTC :: IsNight(){

	unsigned long currentMillis = millis();
  
	// Не пытаемся считывать значения датчиков каждый тик, а делаем это не чаще 1 раза в 5 минут (300 000 миллисекунд)
	if(currentMillis - IsNightPreviousMillis > (long)300000) { 
		// сохраняем время последнего обновления
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