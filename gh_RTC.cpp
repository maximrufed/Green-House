#include "gh_RTC.h"


bool gh_RTC :: begin(){
	return(true);
}

bool gh_RTC :: IsNight(){

	unsigned long currentMillis = millis();
  
	// Не пытаемся считывать значения датчиков каждый тик, а делаем это не чаще 1 раза в 5 минут
	if(currentMillis - IsNightPreviousMillis > (long)5*1000*60) { // не забываем перевести Интервал из минут в миллисекунды
    // сохраняем время последнего обновления
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