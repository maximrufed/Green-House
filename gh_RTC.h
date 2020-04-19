#ifndef GH_RTC_H
#define GH_RTC_H

#include <RTClib.h>
#include "globals.h"

class gh_RTC
{

  public:
	bool Begin();
    bool IsNight();	
	bool IsDay();	

  private:
	long IsNightPreviousMillis = 0;      // храним время последнего обновления значений датчиков
	RTC_DS3231 rtcDS3231;

};

#endif